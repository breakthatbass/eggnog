# Santa
## Advent of Code puzzle input downloader

Santa is a tool that allows you to download AOC puzzle inputs through the command line for Linux and Mac OS.  

## Installation
```
make install
```

The first time you use it, it's going to prompt you for you session id.  
The get that, log in on the AOC website, inspect element with your browser tools, click the 'storage' tab on the top, then click the 'cookies' tab on the left. You will see you session id there.  

The program will save your session id in your home directory in a file called `.santa`.  

## Usage
```
santa <year> <day>   # something like: santa 2020 5  
```
Make use of pipes and redirection to use the puzzle inputs in any way you like.

## TODO
1. make a submit feature
2. add an option to save input to file without redirection
3. add an option to download puzzle directions
