-- Keep a log of any SQL queries you execute as you solve the mystery.
-- Query the report
SELECT description
FROM crime_scene_reports
WHERE year = 2021 AND month = 7 AND day = 28
AND street = 'Humphrey Street';


--Filter through interviews for bakery reports
SELECT name , transcript
FROM interviews
WHERE year = 2021 AND month = 7 AND day = 28;

--Check exiting cars at time of theft
SELECT license_plate
FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28
AND hour = 10 AND minute BETWEEN 15 AND 25;

--Check the littering activity
SELECT activity, license_plate
FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28
AND hour = 16 AND minute BETWEEN 0 AND 40;

--Check outgoing phonecalls from 1015 onwwards which are <1min
--Duration is in seconds
SELECT DISTINCT caller, receiver
FROM phone_calls
WHERE year = 2021 AND month = 7 AND day = 28
AND duration < 60;

--Check first flight id out of fiftyville on 29/7/21
--Flight id = 36
SELECT flights.id
FROM flights
JOIN airports
ON flights.origin_airport_id = airports.id
WHERE airports.full_name LIKE 'Fiftyville%'
AND year = 2021 AND month = 7 AND day = 29
ORDER BY hour
LIMIT 1;

--Check where the thief went
--The ON is an intersection of two sets hence only destination airport is queried
SELECT airports.full_name
FROM airports
JOIN flights
ON flights.destination_airport_id = airports.id
WHERE flights.id =
(SELECT flights.id
FROM flights
JOIN airports
ON flights.origin_airport_id = airports.id
WHERE airports.full_name LIKE 'Fiftyville%'
AND year = 2021 AND month = 7 AND day = 29
ORDER BY hour
LIMIT 1);

--Check who is taking the first flight out of fiftyville tmr
SELECT people.name
FROM people
    --Corresponding passport numbers
    WHERE people.passport_number IN
    (SELECT passengers.passport_number
    FROM passengers
    JOIN flights
    ON passengers.flight_id = flights.id
    WHERE flights.id =
        --Flight id of the first flight
        (SELECT flights.id
        FROM flights
        JOIN airports
        ON flights.origin_airport_id = airports.id
        WHERE airports.full_name LIKE 'Fiftyville%'
        AND year = 2021 AND month = 7 AND day = 29
        ORDER BY hour
        LIMIT 1));

--Check amongst those who are on the first flight who have the same car plate
SELECT people.name, people.license_plate, people.phone_number
FROM people
WHERE people.license_plate IN
(SELECT license_plate
FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28
AND hour = 10 AND minute BETWEEN 15 AND 25)
AND people.name IN
    --Corresponding passport number
    (SELECT people.name
    FROM people
    WHERE people.passport_number IN
    (SELECT passengers.passport_number
    FROM passengers
    JOIN flights
    ON passengers.flight_id = flights.id
    WHERE flights.id =
        --Passengers on the first flight
        (SELECT flights.id
        FROM flights
        JOIN airports
        ON flights.origin_airport_id = airports.id
        WHERE airports.full_name LIKE 'Fiftyville%'
        AND year = 2021 AND month = 7 AND day = 29
        ORDER BY hour
        LIMIT 1)));

--Check amongst the suspects who has a corresponding outgoing number
SELECT people.name, people.phone_number
FROM people
WHERE people.phone_number IN
    --Corresponding outgoing number
    (SELECT DISTINCT caller
    FROM phone_calls
    WHERE year = 2021 AND month = 7 AND day = 28
    AND duration < 60)
    AND people.name IN
        --Corresponding name of license plate holder
        (SELECT people.name
        FROM people
        WHERE people.license_plate IN
            --License plates exiting
            (SELECT license_plate
            FROM bakery_security_logs
            WHERE year = 2021 AND month = 7 AND day = 28
            AND hour = 10 AND minute BETWEEN 15 AND 25)
            AND people.name IN
                --Passengers on the first flight
                (SELECT people.name
                FROM people
                WHERE people.passport_number IN
                    --Corresponding passport number
                    (SELECT passengers.passport_number
                    FROM passengers
                    JOIN flights
                    ON passengers.flight_id = flights.id
                    WHERE flights.id =
                        --Flight id of the first flight
                        (SELECT flights.id
                        FROM flights
                        JOIN airports
                        ON flights.origin_airport_id = airports.id
                        WHERE airports.full_name LIKE 'Fiftyville%'
                        AND year = 2021 AND month = 7 AND day = 29
                        ORDER BY hour
                        LIMIT 1))));

--Query who withdraw money from ATM
SELECT name
FROM people
JOIN bank_accounts
ON people.id = bank_accounts.person_id
WHERE bank_accounts.account_number IN
    --Corresponding account no.s who withdraw
    (SELECT atm_transactions.account_number
    FROM atm_transactions
    WHERE year = 2021 AND month = 7 AND day = 28
    AND atm_location = 'Leggett Street'
    AND transaction_type = 'withdraw');

--Check who amongst the suspects who withdrew money
SELECT people.name, people.phone_number
FROM people
WHERE people.phone_number IN
    --Corresponding outgoing number
    (SELECT DISTINCT caller
    FROM phone_calls
    WHERE year = 2021 AND month = 7 AND day = 28
    AND duration < 60)
    AND people.name IN
        --Corresponding name of license plate holder
        (SELECT people.name
        FROM people
        WHERE people.license_plate IN
            --License plates exiting
            (SELECT license_plate
            FROM bakery_security_logs
            WHERE year = 2021 AND month = 7 AND day = 28
            AND hour = 10 AND minute BETWEEN 15 AND 25)
            AND people.name IN
                --Passengers on the first flight
                (SELECT people.name
                FROM people
                --Corresponding passport number
                WHERE people.passport_number IN
                    (SELECT passengers.passport_number
                    FROM passengers
                    JOIN flights
                    ON passengers.flight_id = flights.id
                    WHERE flights.id =
                        --Flight id of the first flight
                        (SELECT flights.id
                        FROM flights
                        JOIN airports
                        ON flights.origin_airport_id = airports.id
                        WHERE airports.full_name LIKE 'Fiftyville%'
                        AND year = 2021 AND month = 7 AND day = 29
                        ORDER BY hour
                        LIMIT 1))))
                        -- Name of person who simultaneously withdrew money
                        AND people.name IN
                            (SELECT name
                            FROM people
                            JOIN bank_accounts
                            ON people.id = bank_accounts.person_id
                            WHERE bank_accounts.account_number IN
                                --Corresponding account no.s who withdraw
                                (SELECT atm_transactions.account_number
                                FROM atm_transactions
                                WHERE year = 2021 AND month = 7 AND day = 28
                                AND atm_location = 'Leggett Street'
                                AND transaction_type = 'withdraw'));

--Bruce is the thief
--Robin is the accomplice

--Check who is the accomplice by analysing phone calls which Bruce made
SELECT people.name
FROM people
WHERE people.phone_number =
    --Phone number of the accomplice
    (SELECT phone_calls.receiver
    FROM phone_calls
    JOIN people
    ON people.phone_number = phone_calls.caller
    WHERE year = 2021 AND month = 7 AND day = 28
    AND duration < 60
    AND people.name = 'Bruce');


--List of suspects car plates
+----------+---------------+
| activity | license_plate |
+----------+---------------+
| exit     | 5P2BI95       |
| exit     | 94KL13X       |
| exit     | 6P58WS2       |
| exit     | 4328GD8       |
| exit     | G412CB7       |
| exit     | L93JTIZ       |
| exit     | 322W7JE       |
| exit     | 0NTHK55       |
+----------+---------------+

--List of suspected culprit(caller) & accomplice(reciever)
+----------------+----------------+
|     caller     |    receiver    |
+----------------+----------------+
| (130) 555-0289 | (996) 555-8899 |
| (499) 555-9472 | (892) 555-8872 |
| (367) 555-5533 | (375) 555-8161 |
| (499) 555-9472 | (717) 555-1342 |
| (286) 555-6063 | (676) 555-6554 |
| (770) 555-1861 | (725) 555-3243 |
| (031) 555-6622 | (910) 555-3251 |
| (826) 555-1652 | (066) 555-9701 |
| (338) 555-6650 | (704) 555-2131 |
+----------------+----------------+

--List of people who took the flight
+--------+---------------+
|  name  | license_plate |
+--------+---------------+
| Kenny  | 30G67EN       |
| Sofia  | G412CB7       |
| Taylor | 1106N58       |
| Luca   | 4328GD8       |
| Kelsey | 0NTHK55       |
| Edward | 130LD9Z       |
| Bruce  | 94KL13X       |
| Doris  | M51FA04       |
+--------+---------------+

--People leaving bakery at time of crime
+---------+---------------+
|  name   | license_plate |
+---------+---------------+
| Vanessa | 5P2BI95       |
| Barry   | 6P58WS2       |
| Iman    | L93JTIZ       |
| Sofia   | G412CB7       |
| Luca    | 4328GD8       |
| Diana   | 322W7JE       |
| Kelsey  | 0NTHK55       |
| Bruce   | 94KL13X       |
+---------+---------------+

--List of names and license plates of suspected theives
+--------+---------------+
|  name  | license_plate |
+--------+---------------+
| Sofia  | G412CB7       |
| Luca   | 4328GD8       |
| Kelsey | 0NTHK55       |
| Bruce  | 94KL13X       |
+--------+---------------+