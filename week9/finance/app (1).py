import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
import datetime
from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    stocksOwned = db.execute(
        "SELECT symbol, SUM(shares) AS shares FROM purchases JOIN users ON purchases.user_id = users.id WHERE users.id = ? GROUP BY symbol", session["user_id"])

    grandTotal = 0
    for stock in stocksOwned:
        price = lookup(stock["symbol"])["price"]
        stock["total_num"] = stock["shares"] * price
        stock["price_num"] = price
        grandTotal += stock["total_num"]

        # raw version
        stock["total"] = f"{stock['shares'] * price:.2f}"
        stock["price"] = f"{price:.2f}"

        # usd version
        stock["total_usd"] = usd(stock["shares"] * price)
        stock["price_usd"] = usd(price)

    userCash = db.execute("SELECT cash FROM users WHERE users.id = ?", session["user_id"])

    cash = userCash[0]["cash"]

    grandTotal += cash

    cash_usd = usd(cash)

    grandTotal_usd = usd(grandTotal)

    return render_template("portfolio.html", stocksOwned=stocksOwned, cashUsd=cash_usd, grandTotalUsd=grandTotal_usd)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        # Purchase the stock so long as the user can afford it
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol or not shares:
            return apology("Fill in all the blanks")

        try:
            shares = int(shares)
        except ValueError:
            return apology("Error handling the share number")

        lookedup = lookup(symbol)
        if lookedup == None:
            return apology("This symbol does not exist")

        if shares < 0:
            return apology("Invalid number of shares")

        shareQuote = lookedup["price"]

        userCash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

        userCash = userCash[0]["cash"]

        total = shares * shareQuote

        newCash = userCash - total

        if userCash < total:
            return apology("Not enough cash")

        db.execute("INSERT INTO purchases(user_id, symbol, shares, prices, date_time) VALUES(?, ?, ?, ?, ?)",
                   session["user_id"], symbol, shares, shareQuote, datetime.datetime.now())

        db.execute("UPDATE users SET cash = ? WHERE id = ?", newCash, session["user_id"])

        db.execute("INSERT INTO transactions (user_id, symbol, shares, price, date_time, action) VALUES (?, ?, ?, ?, ?, ?)",
                   session["user_id"], symbol, shares, shareQuote, datetime.datetime.now(), "Bought")

        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    history = db.execute(
        "SELECT symbol, shares, price, date_time, action FROM transactions WHERE user_id = ? ORDER BY date_time DESC", session["user_id"])

    for row in history:
        row["price"] = usd(row["price"])
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")

    else:
        symbol = request.form.get("symbol")
        lookedup = lookup(symbol)
        if lookedup == None:
            return apology("This symbol does not exist")
        else:
            lookedup["price"] = usd(lookup(symbol)["price"])
            return render_template("quoted.html", lookedup=lookedup)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":

        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not username or not password or not confirmation:
            return apology("Fill in all the blanks!")

        if password != confirmation:
            return apology("Passwords don't match!")

        sqlusername = db.execute("SELECT username FROM users WHERE username = ?", username)
        if sqlusername:
            return apology("Username already taken")
        else:
            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)",
                       username, generate_password_hash(password))

        return redirect("/login")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        symbols = db.execute("SELECT symbol FROM purchases WHERE user_id = ?", session["user_id"])
        return render_template("sell.html", symbols=symbols)

    else:
        symbolSelected = request.form.get("symbol")
        sharesSelected = request.form.get("shares")

        if not symbolSelected or not sharesSelected:
            return apology("Invalid symbol and/or shares number")

        sharesSelected = int(sharesSelected)

        if sharesSelected <= 0:
            return apology("Invalid number of shares")

        shares = db.execute(
            "SELECT SUM(shares) AS total_shares FROM purchases JOIN users ON users.id = purchases.user_id WHERE symbol = ? AND users.id = ?", symbolSelected, session["user_id"])

        if not shares or shares[0]["total_shares"] is None:
            return apology("Error handling the shares")

        if sharesSelected > shares[0]["total_shares"]:
            return apology("Too many shares")

        stockPrice = lookup(symbolSelected)["price"]

        remaining = shares[0]["total_shares"] - sharesSelected

        db.execute("DELETE FROM purchases WHERE user_id = ? AND symbol = ?",
                   session["user_id"], symbolSelected)

        if remaining > 0:
            db.execute("INSERT INTO purchases (user_id, symbol, shares, prices, date_time) VALUES (?, ?, ?, ?, ?)",
                       session["user_id"], symbolSelected, remaining, stockPrice, datetime.datetime.now())

        revenue = stockPrice * sharesSelected

        db.execute("INSERT INTO transactions (user_id, symbol, shares, price, date_time, action) VALUES (?, ?, ?, ?, ?, ?)",
                   session["user_id"], symbolSelected, sharesSelected, stockPrice, datetime.datetime.now(), "Sold")

        currentBalance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

        currentBalance = revenue + currentBalance[0]["cash"]

        db.execute("UPDATE users SET cash = ? WHERE users.id = ?",
                   currentBalance, session["user_id"])

        return redirect("/")
