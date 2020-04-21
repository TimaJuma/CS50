# program to count number of coins

def greedy(change):
    quarters, dimes, nickels, pennies = 0, 0, 0, 0

    while change > 24: # for quarters
        change -= 25
        quarters += 1
    while change > 9:  # for dimes
        change -= 10
        dimes += 1
    while change > 4:  # for nickels
        change -= 5
        nickels += 1

    while change > 0:  # for pennies
        change -= 1
        pennies += 1

    total_coins = quarters + dimes + nickels + pennies
    print(total_coins)
    return total_coins

# ===================MAIN=====================================

cash = -1
while cash < 0:
    try:
        cash = float(input("How much change is owed: "))
    except:
        print("Only numbers")

change = round(cash*100)
greedy(change)
