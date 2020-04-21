# program to filter

from cs50 import SQL
from sys import argv

# get SQL an access to database
db = SQL("sqlite:///students.db")

# check for proper command-line argument
if len(argv) != 2:
    print("Usage: python poster.py [house]")
    exit(0)

data = db.execute("SELECT first, middle, last, birth FROM students WHERE house = (?) ORDER BY last ASC, first ASC", argv[1])

# iterate for each line and print last name in alphabetic order, and first name if same
for row in data:
    if row['middle'] == None:
        print(f"{row['first']} {row['last']}, born {row['birth']}")
    else:
        print(f"{row['first']} {row['middle']} {row['last']}, born {row['birth']}")