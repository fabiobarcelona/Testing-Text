# Testing-Text
Simple C program to count the occurrences of a given string from command line or file, numbers... among other functions.

## Setup
Run the following command to compile
`gcc -g -o main.exe lib.c main.c`

## Commands
- main (or your chosen executable name)
    prints all the available commands and their description

- main info
    prints author info

- main [filename.txt] cuenta [word1] [word2] ... [wordN]
    Counts every occurrence found in the file [filename.txt] for the given words. At least one word is required

- main [filename.txt] busca [source.txt]
    Counts every occurrence found in the file [filename.txt] for the given words in the [source.txt] file. The source file can't be empty

- main [filename.txt] numeros [sum]
    Counts each number with a correct syntax (discarding numbers like --12, 3r2, etc) contained in the [filename.txt] file. The optional parameter [sum] can be indicated if the total sum of the found numbers wants to be displayed. Decimal separator is the dot (.)

- main [filename.txt] fechas

    Counts each date with a correct syntax (DD/MM/AAAA)

- main [filename.txt] remplazar [WordToReplace] [NewWord]
    Replaces [WordToReplace] to [NewWord] in the [filename.txt] file. Result will be stored in a new file named [filename_new.txt]. If file already exists, name will be [filename_new(1).txt] and so

- main [filename.txt] palabras
    Function that counts each word on a given file that is not a number or a date

- main [sourcefile.txt] extracto num [targetfile.txt]
    Function that extracts a given number of lines from [sourcefile.txt] and generates a new file [targetfile.txt] with the selected lines ([sourcefile.txt] must exist and num >= 1)
