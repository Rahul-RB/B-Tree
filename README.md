#BTree implementation in Array and Files, as per CLRS Textbook algorithms.

### Requirements: 
- GCC 5.4.x +

### To Run: 
#### In either my_btrees_array or my_btrees_files, do the following: 

```$ make t=[VALUE]
[VALUE] is the value of 't' which is the degree of an internal node in BTree.
```
#### Then run: 
````$ ./run [OPTIONS] [VALUES]
[OPTIONS] is : 
   "-b" -- to build the tree.
   "-s [VALUE]" -- to search for key: [VALUE] in the tree.
   "-d [VALUE]" -- to delete a key: [VALUE] from thr tree.
````
#### Example: 
````$ make t=4
$ ./run -b
$ ./run -s 1078
$ ./run -d 1078
````

# Folder Structure: 
- my_btrees_arrays
- my_btrees_files

### Both the folders have the following files: 
- run.c
	-a driver program, which takes command line arguments as specified above.

- implementation.c
	-has all the implementations, i.e., necessary functions like insert, delete, search and other helper functions.

- headers.h
	-has the structure for BTree, BTreeNode and for a Record.

- plotAndTest.py in my_btrees_files 
	- ignore this file.

- ignore any other unspecified files.

### Note: in file implementation, the tree is stored in "tree.dat". But due to some personal issues and lack of time, I have not been able to read a tree from existing data file. So, everytime it searches for an element, it builds the tree from scratch. I regret this inconvenience caused.

I would like to thank our professor Mr. NS Kumar for providing the valuable file write and read functions, which helped reduce conversion of array implementation to file implementation very less.
I would also like to mention a couple of friends of mine: Rutha Prasad (https://github.com/RuthaPrasad) and Ganesh K (https://github.com/DarkFate13), who helped me in the time of need. 