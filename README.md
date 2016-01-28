# FM-index

FMindex - A compressed full-text substring index based on the Burrows-Wheeler transform.

## Introduction:

A FM index implementation in C made by Zoran Krišto and Vinko Kodžoman. The index supports 3 versions of the FM index by using the different method for rank queries (Matrix, Wavelet Tree 8 byte code, Wavelet Tree bit code). The Suffix Array is constructed by using the libdivsufsort library.


FMindex creates a full-text compressed index which is used to search for the number of
given substrings.

Using the FMindex you can choose between three diferent data structures:

1. Occurence matrix - slow construction, memory heavy, fast query
2. Byte Wavelet tree - fast construction, memory heavy (better then 1.), slow query
3. Bit Wavelet tree - fast construction, memory efficient, slow query

The FMindex has integrated statistics, which are shown if opted for while running 
the program. Running the statistics part will give you the average build time and the 
average query time for the choosen method (1., 2. or 3.).

This program is constructed for the user to input their own examples. This is done 
simply by changing the contents of the file "ulaz.txt" to the wanted input.
The default contents of "ulaz.txt" are the first 1,000,000 characters of the 
file "sequence Ecoli.fasta" (without the header) and serve as an example.


License:
--------

FMindex is released under the MIT/X11 license. See the file LICENSE for more details.

### Contact

If you have any problems, questions or suggestions send me a mail at vinko.kodzoman@fer.hr
