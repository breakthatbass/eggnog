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
    html = sys.argv[1]     # file to read from
    day = sys.argv[2]   # name to call the cache file
except:
    print('parse.py: must provide html to parse')
    sys.exit(1)

# open file to write to
try:
    cache_file = open(day, "a")
except:
    print(f'parse.py: problem opening provided cache file: {day}')
    sys.exit(1)

soup = BeautifulSoup(html, 'html.parser')

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
        
cache_file.close()