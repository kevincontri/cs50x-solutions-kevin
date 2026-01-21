import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: dna.py <file.csv> <dna_sequence>")
        sys.exit(1)

    # TODO: Read database file into a variable
    csv_file = sys.argv[1]
    try:
        with open(csv_file) as file:
            reader = csv.DictReader(file)
            rows = list(reader)
            fieldnames = reader.fieldnames
    except FileNotFoundError:
        print(f"'{csv_file}' was not found")
        sys.exit(1)

    # TODO: Read DNA sequence file into a variable
    sequence_file = sys.argv[2]
    try:
        with open(sequence_file) as file:
            dna_sequence = file.read()
    except FileNotFoundError:
        print(f"'{sys.argv[2]}' was not found")

    # TODO: Find longest match of each STR in DNA sequence
    str_names = fieldnames[1:]
    str_counts = {}
    for i in str_names:
        longest_run = longest_match(dna_sequence, i)
        str_counts[i] = longest_run

    # TODO: Check database for matching profiles
    for row in rows:
        match = True

        for i in str_counts:
            if int(row[i]) != str_counts[i]:
                match = False
                break
        if match:
            print(row["name"])
            return

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
