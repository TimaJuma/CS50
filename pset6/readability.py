import re

# function to check # of words, letters, sentences


def count(text):

    # word = len(re.findall(r'\w+', text))

    # number of sentences
    sentence = len(re.findall("[!.?]", text))

    # number of words
    words = len(text.split())

    # number of letters
    letter = len(re.findall("[a-zA-Z]", text))

    return sentence, words, letter


# ===============MAIN===========================================================


article = input("Enter you text for analysis:")

num_sentence, num_words, num_letters = count(article)

# calculating index:= 0.0588 * L - 0.296 * S - 15.8
index = float(0.0588 * (num_letters / num_words * 100) - 0.296 * (100 * num_sentence / num_words) - 15.8)

# output of the results
if index < 1:
    print("Before Grade 1")
elif index > 16:
    print("Grade 16+")
else:
    print(f"Grade {round(index)}")
© 2020 GitHub, Inc.
