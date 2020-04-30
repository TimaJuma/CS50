import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, lookup_rate

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")



@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    portfolio = db.execute("SELECT symbol, shares FROM stocks WHERE user_id = :user_id", user_id = session["user_id"])
    cash = round(db.execute("SELECT cash FROM users WHERE id = :user_id", user_id = session["user_id"])[0]['cash'], 2)
    total = 0
    for row in portfolio:
        row.update(lookup(row['symbol']))
        row['total'] = row['shares'] * row['price']
        total += row['total']

    return render_template("index.html", portfolio = portfolio, cash = cash, total = total+cash)



@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        #ensure data has been entered
        if not request.form.get("buy_stock"):
            return apology("missing symbol", 400)
        elif not request.form.get("shares"):
            return apology("missing shares", 400)

        buy_stock = lookup(request.form.get("buy_stock"))
        shares = int(request.form.get("shares"))
        given_symbol = request.form.get("buy_stock")
        #ensure proper stock symbol has been entered
        if buy_stock == None:
            return apology("invalid symbol", 400)

        cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id = session["user_id"])

        # check if enough funds
        buy_value = buy_stock['price']*shares
        if buy_value > cash[0]['cash']:
            return apology("can't afford", 400)


        new_cash = float(cash[0]['cash'] - buy_value)
        # update cash amount and record transaction
        db.execute("UPDATE users SET cash = :new_cash WHERE id = :user_id", new_cash = new_cash, user_id = session["user_id"])
        db.execute("INSERT INTO history(user_id, symbol, shares, price) VALUES(:user_id, :symbol, :shares, :price)",
                    user_id = session["user_id"],
                    symbol = given_symbol,
                    shares = shares,
                    price = buy_stock['price'])


        # update shares number as per transaction
        existing_shares = db.execute("SELECT shares FROM stocks WHERE user_id = :user_id AND symbol= :symbol", user_id = session["user_id"], symbol = given_symbol)
        if not existing_shares:
            db.execute("INSERT INTO stocks(user_id, symbol, shares) VALUES(:user_id, :symbol, :shares )",
                        user_id = session["user_id"],
                        symbol = given_symbol,
                        shares = shares)
        else:
            new_shares = shares + existing_shares[0]['shares']
            db.execute("UPDATE stocks SET shares = :new_shares WHERE user_id = :user_id AND symbol= :symbol",
                        new_shares = new_shares,
                        user_id = session["user_id"],
                        symbol = given_symbol)

        flash("Bought!")
        return redirect("/")


    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")



@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    portfolio = db.execute("SELECT symbol, shares, price, transacted FROM history WHERE user_id = :user_id", user_id = session["user_id"])

    return render_template("history.html", portfolio = portfolio)



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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

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

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure user typed symbol to get quote
        quote_stock = request.form.get("quote_stock")
        if not quote_stock:
            return apology("missing symbols", 400)
        # not existing stock entered
        if lookup(quote_stock) == None:
            return apology("INVALID SYMBOL", 400)

        # We render to same page with adding stock share info. Redirecting to new page is not user friendly, as you have to go back for new quote
        else:
            return render_template("quote.html", quote_stock = lookup(quote_stock))


    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")



@app.route("/register", methods=["GET", "POST"])
def register():

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username and password was submitted
        if not request.form.get("username"):
            return apology("you must provide username", 403)
        elif not request.form.get("password"):
            return apology("you must provide password", 403)
        elif not request.form.get("password_confirm"):
            return apology("you must provide password confirmation", 403)

        #ensure password mathes
        elif request.form.get("password_confirm") != request.form.get("password"):
            return apology("your password must match", 403)

        hash_value = generate_password_hash(request.form.get("password"))
        try:
            new_user = db.execute("INSERT INTO users(username, hash) VALUES(:username, :hash_value)", username=request.form.get("username"), hash_value = hash_value)
        except:
            return apology("username already taken", 403)

        # Remember which user has logged in
        session["user_id"] = new_user

        # Redirect user to home page
        return redirect("/")


    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

     # User reached route via POST (as by submitting a form via POST)
    portfolio = db.execute("SELECT symbol FROM stocks WHERE user_id = :user_id", user_id = session["user_id"])
    if request.method == "POST":

        #ensure data has been entered
        if not request.form.get("sell_stock"):
            return apology("missing symbol", 400)
        elif not request.form.get("shares"):
            return apology("missing shares", 400)

        sell_symbol = request.form.get("sell_stock")
        shares = int(request.form.get("shares"))
        cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id = session["user_id"])

        # check if more shares to be sold than owned
        shares_owned = db.execute("SELECT shares FROM stocks WHERE user_id = :user_id AND symbol = :symbol", user_id = session["user_id"], symbol = sell_symbol)
        if shares > shares_owned[0]['shares']:
            return apology("too many shares", 400)


        price = lookup(sell_symbol)['price']
        new_cash = float(cash[0]['cash']) + shares * price
        # update cash amount and record transaction
        db.execute("UPDATE users SET cash = :new_cash WHERE id = :user_id",
                    new_cash = new_cash,
                    user_id = session["user_id"])
        db.execute("INSERT INTO history(user_id, symbol, shares, price) VALUES(:user_id, :symbol, :shares, :price)",
                    user_id = session["user_id"],
                    symbol = sell_symbol,
                    shares = shares*-1,
                    price = price)


        # update shares number as per transaction
        existing_shares = db.execute("SELECT shares FROM stocks WHERE user_id = :user_id AND symbol= :symbol", user_id = session["user_id"], symbol = sell_symbol)
        new_shares = existing_shares[0]['shares'] - shares
        db.execute("UPDATE stocks SET shares = :new_shares WHERE user_id = :user_id AND symbol= :symbol",
                    new_shares = new_shares,
                    user_id = session["user_id"],
                    symbol = sell_symbol)

        flash("Sold!")
        return redirect("/")


    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("sell.html", portfolio = portfolio)


""" PERSONAL TOUCH"""

@app.route("/add_cash", methods=["GET", "POST"])
@login_required
def add_cash():
    """Add cash of any currency"""

    if request.method == "POST":
        # on POST get amount user has given and currecny
        new_cash = float(request.form.get("cash"))
        cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id = session["user_id"])

        exchange_rates = lookup_rate()['rates']
        for currency, rate in exchange_rates.items():
            if currency == request.form.get("sell_stock"):
                new_cash = float(cash[0]['cash']) + new_cash/rate

        # update cash amount
        db.execute("UPDATE users SET cash = :new_cash WHERE id = :user_id",
                    new_cash = new_cash,
                    user_id = session["user_id"])


        flash("Cash added!")
        return redirect("/")


    # User reached route via GET (as by clicking a link or via redirect)
    else:
        exchange_rates = lookup_rate()['rates']
        return render_template("cash.html", exchange_rates = exchange_rates)



@app.route("/password", methods=["GET", "POST"])
@login_required
def change_password():
    """Change password if needed"""

    if request.method == "POST":
        # ensure user provided password
        if not request.form.get("password"):
            return apology("you must provide Old password", 403)
        elif not request.form.get("password_new"):
            return apology("you must provide New password", 403)
        elif not request.form.get("password_confirm"):
            return apology("you must provide password confirmation", 403)

        # query old password and check if it matches
        data = db.execute("SELECT hash FROM users WHERE id = :user_id", user_id=session["user_id"])
        if len(data) != 1 or not check_password_hash(data[0]["hash"], request.form.get("password")):
            return apology("typed password is invalid", 403)

        # ensure password mathes
        elif request.form.get("password_confirm") != request.form.get("password_new"):
            return apology("your new password must match", 403)

        # update password
        new = generate_password_hash(request.form.get("password_new"))
        db.execute("UPDATE users SET hash = :hash WHERE id = :user_id", user_id=session["user_id"], hash=new)


        flash("Password changed successfully")
        return redirect("/")

    else:
        return render_template("password.html")



def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)



# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
