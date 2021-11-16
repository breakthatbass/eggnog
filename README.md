# eggnog 🎄

`eggnog` is a tool that allows you to download [Advent of Code](https://adventofcode.com) puzzle inputs and submit your answers through the command line for Linux and Mac OS. ...And soon to download puzzle directions too.

## Installation
```
make install
```


## Usage
If Advent of Code is happening, year and day are not required. It automatically assumes the latest puzzle.
```
nog -y <YEAR> -d <DAY>

options:
    -i          -> puzzle input (default)
    -p          -> puzzle directions (coming soon)
    -s <ANSWER> -> submit an answer
    -l <LEVEL>  -> indicate part one or part two (default 1)

    note: if submitting an asnwer for part 2 `-l 2` is required.
```


`eggnog` creates a local cache in the `$HOME` directory called `.eggnog`.
