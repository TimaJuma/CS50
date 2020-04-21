import math

#since PYTHON is flexible I tried different methods of accessing value of digits in each function

# function 1 - to check validity of card
def validity(card):
    cash = str(card)
    sum_even = 0
    sum_odd = 0

    # iterate from end as char and convert to digit
    for i in cash[-2::-2]:
        s = int(i)*2
        if s > 9:
            s = str(s)
            sum_even += int(s[0]) + int(s[1])
        else:
            sum_even += s

    # iterate from end as char and convert to digit
    for i in cash[-1::-2]:
        sum_odd += int(i)
    sum = sum_even + sum_odd

    if (sum % 10) == 0:
        return True
    else:
        return False
#====================================================================================
# function 2 - identify type of card
def card_type(card, length, digit_two, digit_one):
    length = int(length)
    if digit_one == 4 and length >= 13 and length <= 16:
        print("VISA")
    elif digit_two >= 51 and digit_two <= 55 and length == 16:
        print("MASTERCARD")
    elif (digit_two == 34 or digit_two == 37) and length == 15:
        print("AMEX")
    else:
        print("INVALID")
    return card
#=====================================================================================
#===============================MAIN==================================================
card_num = 0
while len(str(card_num)) < 15:
    try:
        card_num = int(input("Enter credit card number: "))
    except:
        print("Only numbers")

length = len(str(card_num))

digit_12 = math.floor(card_num/(10**(length - 2)))
digit_1 = math.floor(card_num/(10**(length - 1)))

# tune variables to functions
valid = validity(card_num)
if valid is True:
    card_type(card_num, length, digit_12, digit_1 )
else:
    print("INVALID")
