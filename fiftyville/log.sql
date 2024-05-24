-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Check the crime scene reports for Humphrey Street on the 28/07/2021
SELECT *
FROM crime_scene_reports
WHERE
    year = 2021 AND
    month = 07 AND
    day = 28 AND
    street = 'Humphrey Street';
-- RESULTS -> theft took place at 10:15am at the bakery. Statements taken on the day of three witnesses mentioned the bakery. Time to look at their statements!!

-- Let's look at police interviews taken during the same date. There should be three about the bakery (giving us the names of the witnesses)
SELECT *
FROM interviews
WHERE
    year = 2021 AND
    month = 07 AND
    day = 28;
-- RESULTS
-- -> Ruth saw the thief getting in a car in the bakery parking lot. Need to check security footage!
-- -> Euguene saw the thief getting cash from Leggett Street ATM earlier that morning. Need to check for ATM transactions during that morning
-- -> Raymond overheard the thief on the phone with someone after the theft. He was asking the other person to buy them a plane ticket for the earliest flight the day after.

-- The thick plottens!!! Let's go for the ATM.
-- Let's take all columns at first just to see what kind of possible values we are dealing with
SELECT * FROM atm_transactions;
-- Looks like transaction_type can be either 'deposit' or 'withdraw' but we don't have hours.
-- We can still grab the account number and cross reference it with the bank_accounts table to find the name and hope it matches the one on the plane ticket
SELECT *
FROM atm_transactions
    JOIN bank_accounts ON atm_transactions.account_number = bank_accounts.account_number
WHERE
    year = 2021 AND
    month = 07 AND
    day = 28 AND
    transaction_type = 'withdraw' AND
    atm_location = 'Leggett Street';

-- Hmm... I wonder if the person_id would match the id in the people table...
SELECT bank_accounts.account_number, person_id, people.name
FROM atm_transactions
    JOIN bank_accounts ON atm_transactions.account_number = bank_accounts.account_number
    JOIN people ON people.id = bank_accounts.person_id
WHERE
    year = 2021 AND
    month = 07 AND
    day = 28 AND
    transaction_type = 'withdraw' AND
    atm_location = 'Leggett Street';
-- RESULTS
--+----------------+-----------+---------+
--| account_number | person_id |  name   |
--+----------------+-----------+---------+
--| 49610011       | 686048    | Bruce   |
--| 26013199       | 514354    | Diana   |
--| 16153065       | 458378    | Brooke  |
--| 28296815       | 395717    | Kenny   |
--| 25506511       | 396669    | Iman    |
--| 28500762       | 467400    | Luca    |
--| 76054385       | 449774    | Taylor  |
--| 81061156       | 438727    | Benista |
--+----------------+-----------+---------+
-- They can't escape our super forensic skills now!!! One of these is our thief.

-- Let's see what we can find from the nakery security footage now
SELECT *
FROM bakery_security_logs
WHERE
    year = 2021 AND
    month = 07 AND
    day = 28;
-- Too much info. Let's try to cross reference with the phonecalls table to check who made a phonecall right after leaving the bakery
-- Let's start by seeing what the table itself looks like
SELECT *
FROM phone_calls
WHERE
    year = 2021 AND
    month = 07 AND
    day = 28;
-- That doesn't help either but I just realised the people table has license plates!!
-- We can use our previous query (slightly modified) as a subquery to match license plates with the bakery security footage
SELECT bank_accounts.account_number, person_id, people.name
FROM atm_transactions
    JOIN bank_accounts ON atm_transactions.account_number = bank_accounts.account_number
    JOIN people ON people.id = bank_accounts.person_id
WHERE
    year = 2021 AND
    month = 07 AND
    day = 28 AND
    transaction_type = 'withdraw' AND
    atm_location = 'Leggett Street' AND
    people.license_plate IN(
        SELECT bakery_security_logs.license_plate
        FROM bakery_security_logs
            JOIN people ON people.license_plate = bakery_security_logs.license_plate
        WHERE
            year = 2021 AND
            month = 07 AND
            day = 28
    );
-- RESULTS
--+----------------+-----------+--------+
--| account_number | person_id |  name  |
--+----------------+-----------+--------+
--| 49610011       | 686048    | Bruce  |
--| 26013199       | 514354    | Diana  |
--| 25506511       | 396669    | Iman   |
--| 28500762       | 467400    | Luca   |
--| 76054385       | 449774    | Taylor |
--+----------------+-----------+--------+

-- We managed to file it down a bit. Let's try to find some more things with these people's personal data. We can use both phone and passport numbers present in the people table
SELECT *
FROM people
WHERE id IN(686048, 514354, 396669, 467400, 449774);

-- And now we grab the calls they made during the day of the theft and check if they flew the next day
SELECT people.name, phone_calls.caller, phone_calls.receiver, people.license_plate
FROM phone_calls
    JOIN people ON people.phone_number = phone_calls.caller
WHERE   year = 2021 AND
        month = 07 AND
        day = 28 AND
        caller IN(
    SELECT phone_number
    FROM people
    WHERE id IN(686048, 514354, 396669, 467400, 449774)
);
-- RESULTS -> We shrank it down to three people that withdrew money at the ATM that day, were at the bakery and called someone during that day
--+--------+----------------+----------------+---------------+
--|  name  |     caller     |    receiver    | license_plate |
--+--------+----------------+----------------+---------------+
--| Bruce  | (367) 555-5533 | (375) 555-8161 | 94KL13X       |
--| Bruce  | (367) 555-5533 | (344) 555-9601 | 94KL13X       |
--| Bruce  | (367) 555-5533 | (022) 555-4052 | 94KL13X       |
--| Taylor | (286) 555-6063 | (676) 555-6554 | 1106N58       |
--| Diana  | (770) 555-1861 | (725) 555-3243 | 322W7JE       |
--| Taylor | (286) 555-6063 | (310) 555-8568 | 1106N58       |
--| Bruce  | (367) 555-5533 | (704) 555-5790 | 94KL13X       |
--+--------+----------------+----------------+---------------+

-- We know one of the people they called bought a ticket the earliest flight out of Fiftyville the day after
-- We might be able to use the passport_number for these 3 people
SELECT *
FROM flights
    JOIN passengers ON flights.id = passengers.flight_id
    JOIN people ON passengers.passport_number = people.passport_number
WHERE
    year = 2021 AND
    month = 07 AND
    day = 29 AND
    people.passport_number
    IN(
        SELECT passport_number
        FROM people
        WHERE name IN('Bruce', 'Taylor', 'Diana')
    );
-- RESULTS
--+----+-------------------+------------------------+------+-------+-----+------+--------+-----------+-----------------+------+--------+--------+----------------+-----------------+---------------+
--| id | origin_airport_id | destination_airport_id | year | month | day | hour | minute | flight_id | passport_number | seat |   id   |  name  |  phone_number  | passport_number | license_plate |
--+----+-------------------+------------------------+------+-------+-----+------+--------+-----------+-----------------+------+--------+--------+----------------+-----------------+---------------+
--| 18 | 8                 | 6                      | 2021 | 7     | 29  | 16   | 0      | 18        | 3592750733      | 4C   | 514354 | Diana  | (770) 555-1861 | 3592750733      | 322W7JE       |
--| 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     | 36        | 5773159633      | 4A   | 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |
--| 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     | 36        | 1988161715      | 6D   | 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       |
--+----+-------------------+------------------------+------+-------+-----+------+--------+-----------+-----------------+------+--------+--------+----------------+-----------------+---------------+
-- So Diana is off the hook cause her flight is not the earliest one. But Bruce and Taylor can both still be our thief.


-- Going through and running queries on both Bruce and Taylor they were both at the ATM, the bakery, left in a car and called someone that day and got tickets for the first flight of the day to La Guardia Airport in New York
-- I'm not sure what I'm missing here...
-- *** It's the time frame. The thief made a sub-minute call and left in 10 minutes which makes it Bruce! I know he flew to NYC but not who helped him yet. Let's get the receiver of the sub-minute call
SELECT *
FROM people
    JOIN phone_calls ON people.phone_number = phone_calls.receiver
WHERE
    receiver = '(375) 555-8161' AND
    caller = '(367) 555-5533';
-- RESULTS
--+--------+-------+----------------+-----------------+---------------+-----+----------------+----------------+------+-------+-----+----------+
--|   id   | name  |  phone_number  | passport_number | license_plate | id  |     caller     |    receiver    | year | month | day | duration |
--+--------+-------+----------------+-----------------+---------------+-----+----------------+----------------+------+-------+-----+----------+
--| 864400 | Robin | (375) 555-8161 |                 | 4V16VO0       | 233 | (367) 555-5533 | (375) 555-8161 | 2021 | 7     | 28  | 45       |
--+--------+-------+----------------+-----------------+---------------+-----+----------------+----------------+------+-------+-----+----------+

-- LADIES N GENTLEMEN WE GOT'EM
