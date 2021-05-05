'''
parse.py

this script is for parsing the html returned from the AOC server
for the puzzle directions

this script gets used the first time a user requests directions
for a certain day. it parses the html returned and saves the results
to the cache

further requests for the directions will be read from the cache
'''

import sys
from bs4 import BeautifulSoup

try:
    f = sys.argv[1]     # file to read from
    day = sys.argv[2]   # name to call the cache file
except:
    print(f'parse.py: problem opening provided file: {f}')
    sys.exit(1)

# open file to write to
try:
    cache_file = open(day, "a")
except:
    print(f'parse.py: problem opening provided cache file: {day}')
    sys.exit(1)


try:
    with open(f) as fp:
        soup = BeautifulSoup(fp, 'html.parser')
except:
    print('poo on you. file doesnt work')
    sys.exit(1)

body = soup.find_all('article')

for i in body:
    cache_file.write(i.get_text())
    cache_file.write('\n')

# if we have answers submitted already, this will print them out
answer = soup.find_all('p')

for i in answer:
    line = i.get_text()
    if not line.find('Your puzzle answer'):
        cache_file.write(line)
        cache_file.write('\n')

fp.close()
cache_file.close()