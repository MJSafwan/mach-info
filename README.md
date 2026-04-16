# Intro

A simple Mach-O file parser made for educational purposes.

**Note: This project is still in devolopement.**

# Build

First, you need to clone the repo:
```sh
git clone git@github.com:MJSafwan/mach-info.git
```

After that, you can run the command below:
```sh
make
```

A binary called *mach_info* should appear. You can run it to see all the command line options.

# Usage

```
Usage: ./mach_info <filename>

Options:
-h or -help                   : Displays this
-head                         : Only shows header
-seg64                        : Only shows segments
-symtab                       : Only shows the symbol table
-no-head                      : Hides the header
-no-sec                       : Hides sections
-no-sym                       : Hides the symbol table entries
-seg64-info <seg64 name>      : Shows information about <seg64 name>
-dump <seg64 name> <sec name> : Outputs the binary data of <sec name> in <seg64 name> to stdout
```
