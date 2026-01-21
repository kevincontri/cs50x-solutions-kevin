from flask import Flask, render_template, request, redirect, session, url_for
from cs50 import SQL
from werkzeug.security import check_password_hash, generate_password_hash
from flask_session import Session

app = Flask(__name__)
app.secret_key = "your_secret_key"

app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

db = SQL("sqlite:///hospital.db")

roles = [
    'doctor',
    'nurse',
    'admin'
]

units = [
    'icu',
    'er',
    'ward',
    'admin'
]


@app.route("/")
def index():
    user_id = session.get("user_id")

    if not user_id:
        return redirect("/login")

    user = db.execute("SELECT * FROM users WHERE id = ?", user_id)[0]

    patients = db.execute("SELECT * FROM patients WHERE unit = ?", user["unit"])

    all_users = db.execute("SELECT id, username, unit, role FROM users ORDER BY unit")

    all_patients = db.execute("SELECT * FROM patients ORDER BY unit")

    return render_template("main_page.html", user=user, patients=patients, all_users=all_users, all_patients=all_patients)


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "GET":
        return render_template("register.html")
    else:
        role = request.form.get("role")
        username = request.form.get("username")
        password = request.form.get("password")
        confirmed_password = request.form.get("confirmed")
        unit = request.form.get("unit")

        if not role or not username or not password or not confirmed_password or not unit:
            return render_template("error.html", message="Fill in all the blanks!")

        if role not in roles:
            return render_template("error.html", message="Invalid role")

        if unit not in units:
            return render_template("error.html", message="Invalid unit")

        if password != confirmed_password:
            return render_template("error.html", message="Passwords don't match!")

        user_test = db.execute("SELECT username FROM users WHERE username = ?", username)

        if user_test:
            return render_template("error.html", message="Username already taken!")

        db.execute("INSERT INTO users (username, hash, role, unit) VALUES (?, ?, ?, ?)",
                   username, generate_password_hash(password), role, unit)

        return redirect("/login")


@app.route("/login", methods=["GET", "POST"])
def login():

    if request.method == "GET":
        logged = session.get("user_id")
        if logged:
            return redirect("/")
        else:
            return render_template("login.html")
    else:
        username = request.form.get("username")
        password = request.form.get("password")

        if not username or not password:
            return render_template("error.html", message="Fill in all the blanks!")

        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], password):
            return render_template("error.html", message="Invalid username and/or password")

        session["user_id"] = rows[0]["id"]

        return redirect("/")


@app.route("/logout")
def logout():
    session.clear()
    return redirect("/")


@app.route("/patients/<int:patient_id>", methods=["GET", "POST"])
def patient_detail(patient_id):
    user_id = session.get("user_id")
    if not user_id:
        return redirect("/login")

    user = db.execute("SELECT * FROM users WHERE id = ?", user_id)[0]

    patient = db.execute("SELECT * FROM patients WHERE id = ?", patient_id)

    if request.method == "POST":
        form_type = request.form.get("form_type")

        if form_type == "note":

            content = request.form.get("note")

            if not content:
                return render_template("error.html", message="Note cannot be empty")

            writer = f'{user["role"]} {user["username"]}'

            db.execute("INSERT INTO notes (patient_id, content, writer) VALUES (?, ?, ?)",
                       patient_id, content, writer)

        elif form_type == "vitals":
            systolic = request.form.get("systolic")
            diastolic = request.form.get("diastolic")
            heart_rate = request.form.get("heart_rate")
            respiratory_rate = request.form.get("respiratory_rate")
            oxygen_saturation = request.form.get("oxygen_saturation")
            temperature = request.form.get("temperature")

            if not systolic or not diastolic or not heart_rate or not respiratory_rate or not oxygen_saturation or not temperature:
                return render_template("error.html", message="Can't send an empty vital sign!")

            try:
                systolic = int(systolic)
                diastolic = int(diastolic)
                heart_rate = int(heart_rate)
                respiratory_rate = int(respiratory_rate)
                oxygen_saturation = int(oxygen_saturation)
                temperature = float(temperature)
            except ValueError:
                return render_teplate("error.html", message="Invalid number format")

            db.execute("""INSERT INTO vitals (patient_id, systolic, diastolic, heart_rate,
                       respiratory_rate, oxygen_saturation, temperature)
                        VALUES (?, ?, ?, ?, ?, ? ,?)""",
                       patient_id, systolic, diastolic, heart_rate,
                       respiratory_rate, oxygen_saturation, temperature)

    vitals = db.execute(
        "SELECT * FROM vitals WHERE patient_id = ? ORDER BY recorded_at DESC", patient_id)

    latest_vitals = db.execute(
        "SELECT * FROM vitals WHERE patient_id = ? ORDER BY recorded_at DESC LIMIT 1", patient_id)

    if not patient:
        return render_template("error.html", message="Patient not found")

    patient = patient[0]

    if not latest_vitals or not vitals:
        latest_vitals = []
        alerts = []
        exams = []
        vitals = []
    else:
        latest = latest_vitals[0]

        alerts = []

        exams = []

        if latest["systolic"] < 90 or latest["systolic"] > 140:
            alerts.append("Abnormal systolic pressure")
            exams.append("Blood tests (kidney function, electrolytes), ECG, Echocardiogram")

        if latest["diastolic"] < 60 or latest["diastolic"] > 90:
            alerts.append("Abnormal diastolic pressure")
            exams.append("Urinalysis, heart imaging, vasculas studies")

        if latest["heart_rate"] < 55 or latest["heart_rate"] > 100:
            alerts.append("Abnormal heart rate")
            exams.append("Electrocardiogram, Holter monitor, blood tests (thyroid, electrolytes)")

        if latest["respiratory_rate"] < 12 or latest["respiratory_rate"] > 20:
            alerts.append("Abnormal respiratory rate")
            exams.append("Chest X-ray, spirometry, arterial blood gas analysis")

        if latest["temperature"] < 35 or latest["temperature"] > 37.3:
            alerts.append("Abnormal body temperature")
            exams.append("Blood tests, urine test, cultures (blood/urine/sputum)")

        if int(latest["oxygen_saturation"]) < 95:
            alerts.append("Abnormal oxygen saturation")
            exams.append("Arterial blood gas, chest imaging (X-ray/CT), pulmonary function tests")

        latest["oxygen_saturation"] = int(latest["oxygen_saturation"])

        for v in vitals:
            v["oxygen_saturation"] = int(v["oxygen_saturation"])

    notes = db.execute(
        "SELECT * FROM notes WHERE patient_id = ? ORDER BY date_time DESC", patient_id)

    return render_template("patient_detail.html", user=user, patient=patient, vitals=vitals, latest_vitals=latest_vitals, alerts=alerts, exams=exams, notes=notes)


@app.route("/admin", methods=["POST"])
def admin():
    form_type = request.form.get("form_type")
    user_id = session.get("user_id")

    user = db.execute("SELECT * FROM users WHERE id = ?", user_id)[0]

    if user["role"] != "admin":
        return render_template("error.html", message="Access denied")
    if form_type == 'add_patient':
        name = request.form.get("name")
        age = request.form.get("age")
        sex = request.form.get("sex")
        unit = request.form.get("unit")

        if not name or not age or not sex or not unit:
            return render_template("error.html", message="Fill in all the blanks!")

        db.execute("INSERT INTO patients (name, age, sex, unit) VALUES (?, ?, ?, ?)",
                   name, age, sex, unit)

        return redirect("/")

    elif form_type == 'delete_patient':
        patient_id = request.form.get("patient")

        db.execute("DELETE FROM patients WHERE id = ?", patient_id)

        return redirect("/")

    elif form_type == 'change_unit':
        unit = request.form.get("unit")
        user_to_change = request.form.get("user")

        current_unit = db.execute("SELECT unit FROM users WHERE id = ?", user_to_change)

        if current_unit[0]["unit"] == unit:
            return render_template("error.html", message="Can't change to the same unit as before!")

        if unit not in units:
            return render_template("error.html", message="Invalid unit")

        db.execute("UPDATE users SET unit = ? WHERE id = ?", unit, user_to_change)

        return redirect("/")

    elif form_type == 'delete_user':
        user_to_delete = request.form.get("user")

        db.execute("DELETE FROM users WHERE id = ?", user_to_delete)

        return redirect("/")

    elif form_type == "change_patient_unit":
        patient_to_change = request.form.get("patient")
        unit = request.form.get("unit")

        current_unit = db.execute("SELECT unit FROM patients WHERE id = ?", patient_to_change)

        if unit == current_unit[0]["unit"]:
            return render_template("error.html", message="Can't change to the same unit as before!")

        if unit not in units:
            return render_template("error.html", message="Invalid unit")

        db.execute("UPDATE patients SET unit = ? WHERE patients.id = ?", unit, patient_to_change)

        return redirect("/")


@app.route("/users/<int:user_id>", methods=["POST"])
def user_detail(user_id):
    user = db.execute("SELECT * FROM users WHERE id = ?", user_id)[0]

    writer = f'{user["role"]} {user["username"]}'

    notes = db.execute("""SELECT patients.name, notes.content, notes.writer, notes.date_time
                       FROM notes
                       JOIN patients ON patients.id = notes.patient_id
                       WHERE notes.writer = ?""", writer)
    return render_template("user_detail.html", user=user, notes=notes)
