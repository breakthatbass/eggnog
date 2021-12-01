<img src="eggnog.gif" height="200" alt="Clark Griswold"> 

# eggnog 🎄
Yet another Advent of Code CLI tool...

`eggnog` is a tool that allows you to download [Advent of Code](https://adventofcode.com) puzzle inputs, puzzle directions, and submit answers in the terminal. 

It caches everything - even when a wrong answer is submitted.
#

## Installation
`eggnog` works on MacOS and Linux.  

Set the `NOG_INSTALL_PATH` environment variable.
```
# an installation option...
export NOG_INSTALL_PATH=/usr/local/bin
```
Then run
```
make install
```
### Dependencies
It relies on the [`libcurl`](https://curl.se/libcurl/) C library for http requests. Chances are it's already installed and you don't have to do anything. But if it doesn't work you may have to download it.  

**MacOS**  
```
brew install curl
```
**Linux**  
```
sudo apt update
sudo apt-get install liburl4-gnutls-dev
# or whatever packager you use
```
#
## Run Tests
```
make tests
```
#
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

    -p, --directions
	Get puzzle directions.

    -s, --submit=<answer>
	submit an answer to a puzzle. If no answer is supplied, read from
	stdin. Using '-s' automatically reads from stdin.

    -l <puzzle part>, --level=<puzzle part>
	Indicate whether submitting an answer to part 1 or part 2 of a puzzle.
	If left out, part 1 is assumed. Required for submitting part 2 answers.
```
Getting the puzzle input will print it to the terminal in the same way it would with `curl`.
The purpose of that is because many advent of code solutions read from `stdin` rather than a hardcoded file name in the code.


### Example usage
Get input for year 2020 day 1:  
- `nog -y 2020 -d 1`
- or save to a file --> `nog -y 2020 -d 1 > input.txt`
- or pipe it into your program -->   `nog -y 2020 -d 1 | ./myprogram`

Submit an answer for year 2020 day 1 part 1:  
- `nog -y 2020 -d 1 --submit=1234`  

Or read from `stdin` to provide your answer:  
- `./myprogram | nog -y 2020 -d 1 -s`  

Submit an answer for part 2:  
- `nog -y 2020 -d 1 --part=2 -s`

## Cache
`eggnog` creates a local cache in the `$HOME` directory called `.eggnog`.  

Puzzle directions, inputs, and all right and wrong answer submission attempts are saved. The cache is always checked before querying Advent of Code servers.

Unless the cache is deleted, any query to the servers happens only once.

## More Info
There are a lot of these tools. And personally, I feel the only feature really worth having is being able to grab input from the terminal which can easily be done in a short and simple home Python/Ruby/Bash script. But I wanted to learn a bit more about using the [`libcurl`](https://curl.se/libcurl/) C library for http requests.  

If you would like to contribute, feel free to make a pull request.