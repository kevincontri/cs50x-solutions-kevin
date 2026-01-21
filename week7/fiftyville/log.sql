-- Checking description for that specific crime scene--
SELECT description
FROM crime_scene_reports
WHERE month = 7
AND day = 28
AND street = 'Humphrey Street';

-- Reading transcriptions of interviews from that day
SELECT transcript FROM interviews WHERE month = 7 AND day = 28;

                                    -- Within ten minutes of the theft (10:15 - 10:25) The thief was saw getting into a car in the bakery parking lot and drove away.
                                    -- Witnessed earlier in the morning (< 10am) at an ATM on Leggett Street the thief withdrawing some money.
                                    -- The thief phone called someone after they left the bakery (> 10:24) and talked for < 1 min, planning to take the earliest flight
                                    -- out of Fiftyville tomorrow (July 29th). The person on the other end (the compliance) would purchase the flight ticket.

-- People who left the bakery at that time
SELECT id, name, phone_number, passport_number
FROM people
WHERE license_plate IN (
    SELECT license_plate FROM bakery_security_logs
    WHERE year = 2024
    AND month = 7
    AND day = 28
    AND hour = 10
    AND minute BETWEEN 15 AND 25
    AND activity = 'exit'
);

-- From those who left the bakery, just the ones who has made bank transactions that hour.
SELECT people.id, people.name, people.phone_number, people.passport_number
FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
WHERE people.license_plate IN (
    SELECT license_plate FROM bakery_security_logs
    WHERE year = 2024
    AND month = 7
    AND day = 28
    AND hour = 10
    AND minute BETWEEN 15 AND 25
    AND activity = 'exit')
AND atm_transactions.year = 2024
AND atm_transactions.month = 7
AND atm_transactions.day = 28
AND atm_transactions.atm_location = 'Leggett Street'
AND atm_transactions.transaction_type = 'withdraw';

-- From those who made transactions, only the ones who called at that time for a less than a minute duration
SELECT people.name, people.phone_number, phone_calls.receiver
FROM phone_calls
JOIN people ON people.phone_number = phone_calls.caller
WHERE phone_calls.year = 2024
AND phone_calls.month = 7
AND phone_calls.day = 28
AND phone_calls.duration < 60
AND people.name IN (
    SELECT people.name
    FROM people
    JOIN bank_accounts ON bank_accounts.person_id = people.id
    JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
    WHERE people.license_plate IN (
        SELECT license_plate FROM bakery_security_logs
        WHERE year = 2024
        AND month = 7
        AND day = 28
        AND hour = 10
        AND minute BETWEEN 15 AND 25
        AND activity = 'exit')
    AND atm_transactions.year = 2024
    AND atm_transactions.month = 7
    AND atm_transactions.day = 28
    AND atm_transactions.atm_location = 'Leggett Street'
    AND atm_transactions.transaction_type = 'withdraw'
);

-- Found the thief by cheking the passenger's number for the earliest flight
SELECT people.name
FROM people
JOIN passengers ON people.passport_number = passengers.passport_number
JOIN flights ON passengers.flight_id = flights.id
JOIN airports ON flights.origin_airport_id = airports.id
WHERE flights.year = 2024
AND flights.month = 7
AND flights.day = 29
AND flights.hour = 8
AND flights.minute = 20
AND people.name IN (
    SELECT people.name
    FROM phone_calls
    JOIN people ON people.phone_number = phone_calls.caller
    WHERE phone_calls.year = 2024
    AND phone_calls.month = 7
    AND phone_calls.day = 28
    AND phone_calls.duration < 60
    AND people.name IN (
        SELECT people.name
        FROM people
        JOIN bank_accounts ON bank_accounts.person_id = people.id
        JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
        WHERE people.license_plate IN (
            SELECT license_plate FROM bakery_security_logs
            WHERE year = 2024
            AND month = 7
            AND day = 28
            AND hour = 10
            AND minute BETWEEN 15 AND 25
            AND activity = 'exit')
        AND atm_transactions.year = 2024
        AND atm_transactions.month = 7
        AND atm_transactions.day = 28
        AND atm_transactions.atm_location = 'Leggett Street'
        AND atm_transactions.transaction_type = 'withdraw'
    )
);

-- Thief's accomplice (The one called earlier)
SELECT people.name
FROM people
WHERE people.phone_number IN (
    SELECT phone_calls.receiver
FROM phone_calls
JOIN people ON people.phone_number = phone_calls.caller
WHERE phone_calls.year = 2024
AND phone_calls.month = 7
AND phone_calls.day = 28
AND phone_calls.duration < 60
AND people.name = 'Bruce'
);

-- City where the thief landed (Destination)
SELECT airports.full_name, airports.city
FROM flights
JOIN airports ON flights.destination_airport_id = airports.id
JOIN passengers ON flights.id = passengers.flight_id
JOIN people ON passengers.passport_number = people.passport_number
WHERE people.name = 'Bruce'
AND flights.day = 29
AND flights.month = 7
AND flights.year = 2024
AND flights.hour = 8
AND flights.minute = 20;

