# xmas

### xmas is a tool that allows you to download [AOC](https://adventofcode.com) puzzle inputs and submit your answers through the command line for Linux and Mac OS.  

## Installation
```
make install
```
The first time you use it, it's going to prompt you for your session id.  
The get that, log in on the AOC website, inspect element with your browser tools, click the 'storage' tab, then click the 'cookies' tab. You will see you session id there.  

The program saves all inputs in a local cache directory (`~/.xmas`) so that it only contacts the servers once per puzzle input. The session id is also stored in that directory in a file called `sessionid`.  


## Usage
### getting puzzle inputs
By default `xmas` requires a year and a day as arguments. This will retrieve the puzzle input for that day and year. You can also use the `-i` flag to indicate you want the input. 
```
xmas -y <year> -d <day>   # something like: santa -y 2020 -d 5  
```
Make use of pipes and redirection to use the puzzle inputs in any way you like.  

### submitting an answer to a puzzle
To submit an answer use the `-s` flag with your answer as an argument. By default, it will attempt to answer part 1 of any puzzle unless indicated with the `-l` flag. Use `-l 2` to answer part 2 of any puzzle.
```
xmas -y <year> -d <day> -s <answer> -l <part>

# examples
xmas -y 2020 -d 5 -s 12345
xmas -y 2020 -d 5 -s 6789 -l 2
```
### getting usage
For now just use `xmas` by itself and it will print the usage.

## Tests
Run tests from root directory. The tests use files to test the functions rather than hitting up the AOC servers a bunch.
```
make tests
```
## TODO
1. add an option to download puzzle directions
2. add '-h' flag to print usage
