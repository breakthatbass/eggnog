# eggnog 🎄
`eggnog` is a tool that allows you to download [Advent of Code](https://adventofcode.com) puzzle inputs and submit your answers through the command line for Linux and Mac OS.
#
<img src="eggnog.gif" height="200" alt="Clark Griswold"> 

#

`eggnog` creates a local cache in the `$HOME` directory called `.eggnog`.

## Installation
```
make install
```

## Usage
```
nog [--day=<day> --year=<year>] [OPTION] 
```

Day & year are required unless Advent is happening - latest puzzle will automatically
be returned.
```
    -y <year>, --year=<year>
	The year of the puzzle you want to do.

    -d <day>, --day=<day>
	The day of the puzzle you want to do.
```

**Options**:
If no options are supplied, input is assumed.
```
    -i, --input
	Download puzzle input.

    -s, --submit=<answer>
	submit an answer to a puzzle. If no answer is supplied, read from
	stdin. Using '-s' automatically reads from stdin.

    -l <puzzle part>, --level=<puzzle part>
	Indicate whether submitting an answer to part 1 or part 2 of a puzzle.
	If left out, part 1 is assumed. Required for submitting part 2 answers.
```

### Example usage
Get input for year 2020 day 1:  
- `nog -y 2020 -d 1`  

Submit an answer for year 2020 day 1 part 1:  
- `nog -y 2020 -d 1 --submit=1234`  

Or, read from `stdin` to provide your answer:  
- `nog -y 2020 -d 1 -s`  

Submit an answer for part 2:  
- `nog -y 2020 -d 1 --part=2 -s`

