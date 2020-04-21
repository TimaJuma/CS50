import csv
from sys import argv

# function to find the number of repeating sequences of nucleaus base in dna_line


def longest_seq(dna_line, seq):
    nseq_global = 0
    j = len(seq)
    for i, c in enumerate(dna_line):
        if seq == dna_line[i:i + j]:
            nseq_local = 1

            # this condition to check number of sequences
            while dna_line[i: i + j] == dna_line[i + j:i + 2*j]:
                i += j
                nseq_local += 1

            # check if it is the longest sequence
            if nseq_local > nseq_global:
                nseq_global = nseq_local

    return nseq_global


# ===============================MAIN======================================

if len(argv) != 3:
    print("Usage: dna [database] text")
    exit(0)

DATABASE = "databases/large.csv"
database = argv[1] if len(argv) == 3 else DATABASE

# read txt file (DNA)

text = argv[2] if len(argv) == 3 else argv[1]
with open(text, "r") as r_text:
    dna_line = r_text.readline()


# read names from cvs.database

with open(database, "r") as r_file:
    list_names = []
    read_line = csv.DictReader(r_file)
    for row in read_line:
        list_names.append(row)


# find the longest number of sequences for each nucleus base and store values in list

match = {}
for dna in list_names[0]:
    if dna != "name":
        # print(dna)
        match[dna] = str(longest_seq(dna_line, dna))


# iterate over list of dictionaries and compare with match_list

num_matches = 0
for row in list_names:
    if all(item in row.items() for item in match.items()) is True:
        print(row['name'])
        num_matches += 1
if num_matches == 0:
    print("No match")