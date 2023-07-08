import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        return 0

    # Empty list
    names = []

    # TODO: Read database file into a variable, perhaps a dict with multiple values?
    with open(sys.argv[1], 'r') as database:
        csvreader = csv.DictReader(database)
        for line in csvreader:
            names.append(line)

    # TODO: Read DNA sequence file into a variable, perhaps a string?
    with open(sys.argv[2], 'r') as file:
        sequence = file.read()

    # TODO: Find longest match of each STR in DNA sequence, call longest match function

    # Extract the strs, excluding [0] as that is the name of the person
    subsequences = list(names[0].keys())[1:]

    # Create a dictionary to store the results of the sequence
    results = {}
    for sub in subsequences:
        results[sub] = longest_match(sequence, sub)

    # TODO: Check database for matching profiles
    n = len(subsequences)

    # Loop through each person's dictionary
    for name in names:
        count = 0

        # Loop through each str in each person
        for sub in subsequences:
            # Convert the string into an int for arithmetic comparision
            if int(name[sub]) == int(results[sub]):
               count += 1

        # If target is found print the name and terminate the program
            if count == n:
                print(name["name"])
                exit()

    # If no target is found print no match
    print("No match")
def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
