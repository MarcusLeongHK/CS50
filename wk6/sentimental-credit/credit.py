import re
from cs50 import get_string

# write function to check brand name
def main ():
    card_no = get_string("Credit Card Number: ")
    if valid(card_no) == False:
        print("INVALID\n")
        return 0;
    brand(card_no)

# write function to check brand name
def brand(card):
    visa = re.compile(r'[4]{1}\d{12,15}')
    mastercard = re.compile(r'[5]{1}[1,5]{1}\d{14}')
    amex = re.compile(r'[3]{1}[4|7]{1}\d{13}')
    if visa.match(card):
        print("VISA\n")
    elif mastercard.match(card):
        print("MASTERCARD\n")
    elif amex.match(card):
        print("AMEX\n")
    else:
        print("INVALID\n")

# write function to check validity
def valid(card):
    checksum = 0
    # function to make a string of nums into a list of ints
    def digits_of(n):
        return[int(d) for d in str(n)]
    # convert the string of numbers to a list of ints
    digits = digits_of(card)
    odd_digits = digits[-1::-2]
    even_digits = digits[-2::-2]
    #addition for odd digits
    checksum += sum(odd_digits)
    #addition for even digits
    for d in even_digits:
        checksum += sum(digits_of(d*2))
    if checksum % 10 != 0:
        return False
if __name__=="__main__":
    main()
