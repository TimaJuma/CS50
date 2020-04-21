from cs50 import SQL
import csv
from sys import argv

# get SQL an access to database
db = SQL("sqlite:///students.db")

# check for proper command-line argument
if len(argv) != 2:
    print("Usage: python import.py [database]")
    exit(0)

# open CSV file, read through and write to db usinf SQL in needed format
with open(argv[1], "r") as r_file:
    read_line = csv.DictReader(r_file)
    for row in read_line:
        # get values from row dictionary
        name = row['name'].split()
        house = row['house']
        birth = int(row['birth'])
        nothing = None

        # write values to db file without middle name
        if len(name) == 2:
            db.execute("INSERT INTO students(first, middle, last, house, birth) VALUES(?,?,?,?,?)",
                    name[0], nothing, name[1], house, birth)

        # write values to db file with middle name
        elif len(name) == 3:
            db.execute("INSERT INTO students(first, middle, last, house, birth) VALUES(?,?,?,?,?)",
                        name[0], name[1], name[2], house, birth)