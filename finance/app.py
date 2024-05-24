#pk_22392e62c3a5473fb74a5d84a67bdf18
import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

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


@app.route("/", methods=["GET", "POST"])
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]

    if request.method == "GET":
        # Get stocks table + user current cash (as float)
        stocks = db.execute("""
                            SELECT symbol, name, SUM(shares) as amount
                            FROM stocks
                            WHERE user_id = ?
                            GROUP BY symbol""", user_id)
        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

        # Variable to count up total value of stock + cash
        totalValue = cash

        for row in stocks:
            iexInfo = lookup(row["symbol"])

            if not iexInfo:
                return apology("Lookup failed. Is token expired?")

            row["name"] = iexInfo["name"]
            row["price"] = iexInfo["price"]
            row["total"] = row["price"] * row["shares"]

            totalValue += row["total"]

        return render_template(("index.html"), stocks=stocks, cash=usd(cash), totalValue=usd(totalValue))

    else:
        try:
            cashAdd = request.form.get("cashAdd")
            db.execute("UPDATE users SET cash = ? WHERE id = ?", cash[0]["cash"] + cashAdd, user_id)
        except:
            return apology("Could not update user cash!!")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    """ Use user_id to manipulate transactions """

    if request.method == "POST":

        user_id = session["user_id"]
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Ensure valid inputs
        if not shares or not isinstance(shares, int) or int(shares)< 1:
            return apology("Number of shares must be a positive number")
        elif not symbol:
            return apology("No symbol given")

        symbol = symbol.upper()
        shares = int(shares)


        iexDict = lookup(symbol)
        # Check symbol
        if not iexDict:
            return apology("Symbol not valid.")

        # Check if user has enough money
        finalPrice = float(shares) * iexDict["price"]
        userCash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        if finalPrice > userCash[0]["cash"]:
            return apology("Not enough cash!")

        # Insert purchase into database
        try:
            db.execute("""INSERT INTO stocks (user_id, symbol, name, shares, price)
            VALUES (?, ?, ?, ?, ?);""", user_id, symbol, iexDict["name"], shares, iexDict["price"])
        except:
            return apology("An error occured. No purchase was made")

        # If successful, deduct spent cash from users total
        newCash = userCash[0]["cash"] - finalPrice
        try:
            db.execute("UPDATE users SET cash = ? WHERE id = ?", newCash, user_id)
        except:
            return apology("Could not update user cash!!")

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]

    # Get stocks table + user current cash (as float)
    stocks = db.execute("""
                        SELECT symbol, name, shares, price
                        FROM stocks
                        WHERE user_id = ?
                        GROUP BY symbol""", user_id)

    # Variable to count up total value of stock + cash
    return render_template(("index.html"), stocks=stocks)

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
    if request.method == "POST":
        symbol = request.form.get('symbol')
        if not symbol:
            return apology("You didn't write a symbol!")

        iexDict = lookup(symbol)
        if not iexDict:
            return apology("Symbol not valid")

        return render_template("quoted.html", iexDict=iexDict, usdFormat=usd)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    else:
        username = request.form.get('username')
        password = request.form.get('password')
        confirmation = request.form.get('confirmation')

        if not username:
            return apology("No username given")
        elif not password or not confirmation:
            return apology("Password missing")
        elif password != confirmation:
            return apology("Passwords do not match")

    """ Hashing password """
    pwHash = generate_password_hash(password)

    """ Insert query will return error if username already exists. """
    try:
        db.execute("INSERT INTO users (username, hash) VALUES (?,?)", username, pwHash)
    except:
        return apology("Username already exists")

    """ Back to homepage """
    return redirect('/')


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    """ Use user_id to manipulate transactions """

    if request.method == "POST":

        user_id = session["user_id"]
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        # Ensure valid inputs
        if not shares or int(shares) < 1:
            return apology("Number of shares must be a positive number")
        elif not symbol:
            return apology("No symbol given")

        iexDict = lookup(symbol)
        # Check symbol
        if not iexDict:
            return apology("Symbol not valid.")

        # Calculate cash difference
        finalPrice = float(shares) * iexDict["price"]
        userCash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)

        # Insert sale into database
        try:
            db.execute("""INSERT INTO stocks (user_id, symbol, name, shares, price)
            VALUES (?, ?, ?, ?, ?);""", user_id, symbol, iexDict["name"], (shares * -1), iexDict["price"])
        except:
            return apology("An error occured. No sale was made")

        # If successful, increase cash to users total
        newCash = userCash[0]["cash"] + finalPrice
        try:
            db.execute("UPDATE users SET cash = ? WHERE id = ?", newCash, user_id)
        except:
            return apology("Could not update user cash!!")

        return redirect("/")
    else:
        return render_template("buy.html")