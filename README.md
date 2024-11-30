# DirStats
This tool will allow you to check folder's extension statistics (in percentage) with various options.

# Compile
Compiling with Makefile:
```bash
make
```
# Install
Clone the repo:
```
git clone https://github.com/yak6/DirStats
```
cd into it and type:
```bash
sudo make install
```
It will move the executable into /usr/local/bin/ directory.
# Usage
After making an proper install, the executable should be available in the `/usr/local/bin/` which means you can run it everywhere using `dstat`

## Basic usage
Try using help: 
```bash
dstat --help
```

To check file percentage of a directory simply type:
```bash
dstat <directory path>
```

## Sort the percentage
Sort the ouput, ascending or descending.

Arguments: `--sort <increasing/decreasing>`, short version: `-s <i/d>` 

Example usage:
```bash
dstat exampledirectory/ --sort decreasing
```

## Mode
The program will calculate the percentage result in lines or characters, by default in characters.

Arguments: `--mode <lines/characters>`, short version: `-m <l/c>`

Example usage:
```bash
dstat exampledirectory/ --mode lines
```

## Range after decimal point
Range of numbers after the decimal point in the percentage.
The range must be a [natural number](https://en.wikipedia.org/wiki/Natural_number).

Arguments: `--range <range>`, short version: `r <range>`

Example usage:
```bash
dstat exampledirectory/ --range 3
```

## Amount of lines or characters
Include the amount of characters or lines next to the extension's percentage.

Arguments: `--amount`, short version: `-a`
## Total lines or characters
Include total lines or characters in the output.

Arguments: `--total` or `-t` 
