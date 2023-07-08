import csv
import sys

names = {}
with open(sys.argv[1], 'r') as database:
        csvreader = csv.DictReader(database)
        reader = csv.reader(database)
        items = list(next(reader))
        for row in csvreader:
            for i in range(len(items)):
                names["name"] = {items[i+1] : items[i+1]}
        print(names)