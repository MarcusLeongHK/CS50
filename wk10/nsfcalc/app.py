import os
import datetime
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, jsonify
from flask_session import Session
from tempfile import mkdtemp

from helpers import apology, login_required, sgd
# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["sgd"] = sgd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///nsfcalc.db")

@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

@app.route("/login", methods=["GET", "POST"])
def login():
    # Forget any user_id
    session.clear()

    """Register user"""
    # submit input via post
    if request.method == "POST":
        username = request.form.get("username").capitalize()
        if not username:
            return apology("Name Cannot Be Empty")
        db.execute("INSERT INTO handles (username) VALUES (?)",username)
        rows = db.execute("SELECT * FROM handles WHERE username=?", username)
        session["user_id"] = rows[0]["id"]
        return redirect("/home")
    if request.method == "GET":
        return render_template("login.html")

@app.route ("/home")
@login_required
def home():
    return render_template("home.html")


@app.route ("/update_gen_values", methods=["POST"])
@login_required
def update_gen_values():
    duration = request.form.get("duration")
    pathway = request.form.get("pathway")
    voc = request.form.get("vocation")
    return jsonify('', render_template("updated_gen_value.html", duration=duration, pathway=pathway, voc=voc))

@app.route ("/update_rank_values", methods=["GET","POST"])
@login_required
def update_rank_values():
    highest_rank = request.form.get("highest_rank")
    return jsonify('', render_template("updated_rank_value.html", highest_rank=highest_rank))


@app.route ("/", methods=["GET", "POST"])
@login_required
def index():
    if request.method == "POST":
        direction = {}
        durations = ["22","24"]
        paths = ["MAN", "SGT", "OFFICER"]
        vocs = ["SERVICE/TECHNICAL","SUPPORT COMBATANT","AIRCREW, ARMOUR, GUARDS, INFANTRY, COMBAT MEDICS, SEAGOING","CBRD/EOD, COMMANDO, NAVAL DIVER"]
        duration = request.form.get("duration")
        pathway = request.form.get("pathway")
        voc = request.form.get("vocation")
        if not duration or not pathway or not voc:
            return apology("No Field Can Be Left Empty")
        if pathway not in paths:
            return apology("Invalid Pathway Is Selected")
        if duration not in durations:
            return apology("Invalid Duration Is Selected")
        if voc not in vocs:
            return apology("Invalid Vocation Is Selected")

        username = db.execute("SELECT username FROM handles WHERE id=?", session["user_id"])[0]["username"]
        # return ranks of man
        if pathway == "MAN":
            pay = {
                "REC/PTE": 580,
                "LCP": 600,
                "CPL": 650,
                "CFC": 690
            }
            direction["MAN"] = pay
            db.execute("INSERT INTO users(user_id,username,duration,pathway,vocation) VALUES (?,?,?,?,?)",session["user_id"],username, duration, pathway, voc)
            return render_template("pay.html", pay=pay, duration=duration, voc=voc, pathway=pathway, username=username)
        # return ranks of sgt
        if pathway == "SGT":
            pay = {
                "REC": 580,
                "SCT": 670,
                "3SG": 900,
                "2SG": 1000,
                "1SG": 1070
            }
            direction["sgt"] = pay
            db.execute("INSERT INTO users(user_id,username, duration, pathway, vocation) VALUES (?,?,?,?,?)",session["user_id"],username, duration, pathway, voc)
            return render_template("pay.html", pay=pay, duration=duration, voc=voc, pathway=pathway,username=username)
        # return ranks of officer...
        if pathway == "OFFICER":
            pay = {
                "REC": 580,
                "OCT": 860,
                "2LT": 1100,
                "LTA": 1280
            }
            direction["officer"] = pay
            db.execute("INSERT INTO users(user_id,username, duration, pathway, vocation) VALUES (?,?,?,?,?)",session["user_id"],username, duration, pathway, voc)
            return render_template("pay.html", pay=pay, duration=duration, voc=voc, pathway=pathway,username=username)
    if request.method == "GET":
        username = db.execute("SELECT username FROM handles WHERE id=?", session["user_id"])[0]["username"]
        return render_template("pay.html", username=username)

@app.route ("/calculate", methods=["GET", "POST"])
@login_required
def calculate():
    if request.method == "GET":
        return redirect("/")
    if request.method == "POST":
        username = db.execute("SELECT username FROM handles WHERE id=?", session["user_id"])[0]["username"]
        # find vocation pay
        risk_pay = {
            "SERVICE/TECHNICAL": 50,
            "SUPPORT COMBATANT": 175,
            "AIRCREW, ARMOUR, GUARDS, INFANTRY, COMBAT MEDICS, SEAGOING": 225,
            "CBRD/EOD, COMMANDO, NAVAL DIVER": 400
        }
        voc_pay = risk_pay[db.execute("SELECT vocation FROM users WHERE user_id=?", session["user_id"])[-1]["vocation"]]
        # bmt duration and pay
        bmt = request.form.get("bmt")
        bmt_rank = request.form.get("bmt_rank")

        # training school duration and pay
        school = request.form.get("school")
        school_rank = request.form.get("school_rank")

        # unit duration and pay
        unit = request.form.get("unit")
        unit_rank = request.form.get("unit_rank")

        # 2nd unit duration and pay
        unit_2 = request.form.get("unit_2")
        unit2_rank = request.form.get("unit2_rank")
        if not bmt or not bmt_rank or not school or not school_rank or not unit or not unit_rank:
            return apology("All Fields Except For Unit 2 Must Be Filled")

        # actual vs inputted duration
        total_duration = int(bmt) + int(school) + int(unit) + int(unit_2)
        actual_duration = int(db.execute("SELECT duration FROM users WHERE user_id=?", session["user_id"])[-1]["duration"])
        # if durations do not match delete record and restart process
        if total_duration != actual_duration:
            return apology("The Inputted Duration Is Not Equal To Actual Service Duration")
        # if durations match then calculate pay
        else:
            bmt_pay = int(bmt)*int(bmt_rank)
            school_pay = int(school)*int(school_rank)
            unit_pay = int(unit)*(int(unit_rank)+voc_pay)
            if not unit2_rank:
                unit2_pay = 0
            else:
                unit2_pay = int(unit_2)*(int(unit2_rank)+voc_pay)
            total_pay = (bmt_pay + school_pay + unit_pay + unit2_pay) + (total_duration-12)*20
            ave_pay = total_pay / total_duration
            db.execute("INSERT INTO salary (total_pay,user_id,ave_pay) VALUES (?,?,?)",total_pay,session["user_id"],ave_pay)
            return render_template("calculate.html", total_pay=total_pay, ave_pay=ave_pay, username=username)

@app.route ("/savings", methods=["GET", "POST"])
@login_required
def savings():
    if request.method == "GET":
        return redirect("/")
    if request.method =="POST":
        username = db.execute("SELECT * FROM handles WHERE id=?", session["user_id"])[0]["username"]
        actual_duration = int(db.execute("SELECT duration FROM users WHERE user_id=?", session["user_id"])[-1]["duration"])
        total_pay = db.execute("SELECT total_pay FROM salary WHERE user_id=?", session["user_id"])[-1]["total_pay"]
        ave_pay = db.execute("SELECT ave_pay FROM salary WHERE user_id=?", session["user_id"])[-1]["ave_pay"]
        savings_nominal = request.form.get("savings_nominal")
        savings_percentage = request.form.get("savings_percentage")
        if not savings_nominal and not savings_percentage:
            return apology("Both Saving Targets Cannot Be Empty")
        if savings_nominal and savings_percentage:
            return apology("Only One Saving Target Can Be Inputted")
        else:
            if savings_nominal:
                if int(savings_nominal) > ave_pay:
                    return apology("Monthly Savings Must Be Less Than Average Salary")
                else:
                    total_savings = int(savings_nominal)*actual_duration
                    print(total_savings)
                    print(type(actual_duration))
                    print(type(total_savings))
                    proportion = float((total_savings/total_pay)*100)
                    total_expenditure = total_pay-total_savings
                    db.execute("INSERT INTO savings (total_savings,user_id,proportion) VALUES (?,?,?)",total_savings,session["user_id"],proportion)
                    session.clear()
                    return render_template("savings.html",total_expenditure=total_expenditure,total_savings=total_savings,proportion=proportion,monthly_savings=int(savings_nominal),username=username,total_pay=total_pay,ave_pay=ave_pay)
            if savings_percentage:
                if int(savings_percentage) > 90 or int(savings_percentage) < 10:
                    return apology("Savings Percentage Must Be Within The Specified Range")
                else:
                    total_savings = total_pay * int(savings_percentage)/100
                    monthly_savings = total_savings/actual_duration
                    total_expenditure = total_pay-total_savings
                    db.execute("INSERT INTO savings (total_savings,user_id,monthly_savings) VALUES (?,?,?)",total_savings,session["user_id"],monthly_savings)
                    session.clear()
                    return render_template("savings.html",total_expenditure=total_expenditure,total_savings=total_savings,monthly_savings=monthly_savings,proportion=savings_percentage,username=username,total_pay=total_pay,ave_pay=ave_pay)

@app.route("/logout")
def logout():
    """Log user out"""
    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")

@app.route ("/ord")
@login_required
def ord():
    username = db.execute("SELECT * FROM handles WHERE id=?", session["user_id"])[0]["username"]
    return render_template("ord.html",username=username)

@app.route ("/ord_date")
@login_required
def days():
    ord_date = datetime.datetime.strptime(request.args.get("ord_date"), "%Y-%m-%d")
    today = datetime.datetime.now()
    days_left = (ord_date - today).days
    if days_left:
        return render_template("updated_ord_values.html", days_left=days_left)
    else:
        days_left = NULL
        return render_template("updated_ord_values.html", days_left=days_left)