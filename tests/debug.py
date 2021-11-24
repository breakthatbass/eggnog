#!/usr/bin/env python3

# this script changes the DEBUG constant in `eggnog.h`
# so we can have it in production mode but test in debug mode

import sys

if len(sys.argv) != 2:
	print('usage: ./debug.py COMMAND')
	print("available commands: [debug, undebug]")
	sys.exit(1)

file = '../src/eggnog.h'

cmd = sys.argv[1]

fp = open(file, 'rt')
data = fp.read()

if cmd == 'debug':
	data = data.replace('#define DEBUG 0', '#define DEBUG 1')
elif cmd == 'undebug':

	data = data.replace('#define DEBUG 1', '#define DEBUG 0')
else:
	sys.exit(1)

fp.close()
fp = open(file, 'wt')
fp.write(data)
fp.close()
