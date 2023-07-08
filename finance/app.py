import os
import datetime

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

#default method is get there is no form in this
@app.route("/", methods=["GET", "POST"])
@login_required
def index():
    """Show portfolio of stocks"""
    if request.method == "GET":
        stocks = db.execute("SELECT stock, SUM(qty) FROM purchases GROUP BY user_id, stock HAVING user_id=?;",session["user_id"])
        equity = 0
        for stock in stocks:
            stock["price"] = float(lookup(stock["stock"])["price"])
            stock["value"] = stock["price"] * stock["SUM(qty)"]
            equity = equity + stock["value"]
        user = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        balance = int(user[0]["cash"])
        net_assets = equity + balance
        return render_template("index.html", stocks=stocks, net_assets=net_assets, balance=balance)
    # buy via index page
    if request.method == "POST":
        if request.form.get("transaction") == "buy":
            quantity = request.form.get("quantity")
            shares = request.form.get("stock")
            return render_template("buy.html", quantity=quantity, shares=shares)
        if request.form.get("transaction") == "sell":
            quantity = request.form.get("quantity")
            shares = request.form.get("stock")
            return render_template("sell.html", quantity=quantity, shares=shares)
        return apology("gay")

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    symbol = request.form.get("symbol")
    shares = request.form.get("shares")
    if request.method == "GET":
        return render_template("buy.html")
    else:
        # if qty is blank
        if not shares:
            return apology("Blank Quantity")
        # if symbol is invalid/blank
        if lookup(symbol) == None:
            return apology("Blank/Invalid Symbol")
        # if less than 1 share is inputed
        if int(shares) <= 0:
            return apology("Quantity Must Be A Positive Integer")
        # sum the transaction value
        price = float(lookup(symbol)["price"])
        txn_value = price * int(shares)
        # this is the cash balance of the user before transaction
        balance = float(db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"])
        # if balance is insufficent decline transaction
        if txn_value > balance:
            return apology("Insufficent Funds To Complete Transaction")
        else:
            name = symbol.upper()
            # using now() to get current time
            current_time = datetime.datetime.now()
            balance = balance - txn_value
            db.execute("UPDATE users SET cash = ? WHERE id = ?", balance, session["user_id"])
            db.execute("INSERT INTO purchases(user_id,stock,qty,price,value,date) VALUES(?,?,?,?,?,?)", session["user_id"], name, shares, price, txn_value, current_time)

        # reroute to homepage after transaction is complete
        return redirect("/")
@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    if request.method == "GET":
        purchases = db.execute("SELECT * FROM purchases WHERE user_id=?", session["user_id"])
        return render_template("history.html", purchases=purchases)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
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
    ticker = request.form.get("symbol")
    if request.method == "GET":
        return render_template("quote.html")
    else:
        quote = lookup(ticker)
        if not quote:
            return apology("Blank/Invalid Ticker")
        return render_template("quoted.html", quote=quote)

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # submit input via post
    if request.method == "POST":
        # get a list of the usernames in the database
        names = db.execute("SELECT username FROM users")

        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        # if username is blank
        if not username:
            return apology("Blank Username")
        # Username already exists
        for name in names:
            if username == name["username"]:
                return apology("Username Already Exits")
        # require twice password input; if password is blank or no match return apology
        if not password or password != confirmation:
            return apology("Blank Password/Passwords Do Not Match")
        # insert into users.db, hash the password
        hashpw = generate_password_hash(password, salt_length=8)
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username , hashpw)
        return redirect("/")
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    symbol = request.form.get("symbol")
    shares = request.form.get("shares")
    stocks = db.execute("SELECT stock, SUM(qty) FROM purchases GROUP BY user_id,stock HAVING user_id=?",session["user_id"])
    if request.method == "GET":
        return render_template("sell.html", stocks=stocks)

    if request.method == "POST":
        for stock in stocks:
            # if stock is owned by user
            if not symbol:
                apology("No Stock Was Selected")
            # if in the correct symbol dict
            if symbol == stock["stock"]:
                # if the qty exceeds the balance return apology
                if int(shares) > int(stock["SUM(qty)"]):
                    return apology("Insufficent Number Of Shares To Sell")
                # if shares chosen is <= 0
                if int(shares) <= 0:
                    apology("Quantity must be a positive integer greater than 0")
                # if there are no violations execute sell order
                #update cash balance
                balance = int(db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"])
                price = float(lookup(symbol)["price"])
                txn_value = int(shares) * price
                balance = balance + txn_value
                db.execute("UPDATE users SET cash = ? WHERE id = ?", balance, session["user_id"])
                #update shares balance
                current_time = datetime.datetime.now()
                db.execute("INSERT INTO purchases(user_id,stock,qty,price,value,date) VALUES(?,?,?,?,?,?)",session["user_id"],stock["stock"],-int(shares),-price,-txn_value, current_time)
                return redirect("/")
        return apology("This Stock Is Not Owned By The User")