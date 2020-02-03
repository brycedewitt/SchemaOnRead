# CS4500_Assn1_Part1

#### To Run/Test
From the root directory:
`make comp` to compile
`make all` to run the standard tests


**The Purpose**<br>
This repository implements a data adapter for a specific data format. The program reads in a file in 
the given data format and converts it to a new columnized data format. The command line call requires
three inputs: the path to the file, the starting byte and the end byte. These bytes determine where
the program will read from the file. There are 3 optional flags: -print_col_type returns the data type of
each of the values in the given column, -print_col_idx returns the value at the two indices given,
-is_missing_idx returns whether or not the value at the given indices is empty. The second and third
flags require two indicies, first the column and then the offset with in the column.
<br>
Data from the file is read in line by line. It will find the longest line in the first 500 lines and
that will determine the schema. All other lines in the file will follow that schema. The data will be
held in columns where each value in the column is the same data type. The data type 
(STRING, FLOAT, INT, BOOL) of each column is determined by the longest line. In order to follow the
schema, each line must have values with the same types in the same order as the base line.
If a value does not match the base line, an empty value with be placed in that position. If the
user attempts to print that position, the program will print "Empty Value";
<br><br>
**The Code**<br>
There are 3 classes that are used in this code, FileReader, File, and Column.
<br>
_FileReader_<br>
This class is used to read in the contents of the given file. In the class' constructor, it runs the
function get_stats(). This initializes variables that describe the file, such as the number of rows,
columns, starting byte, ending byte etc. These will be saved as public variables in the class. The 
class will also hold a pointer to the File class. Command line flags will be run in FileReader functions,
but these functions use the File instance to reference the data, which is now saved in columns.
<br>
_File_<br>
This class holds all of the contents of from the given file now stored in columns.
When initialized the class is given the number of rows, columns, and an array of the each of the values
in the longest line of the file. The number of rows and columns are set up as public variables of the
class. The class hold the number of rows and columns and has two arrays. The first is an array of the enum types, called 
column_setup. This will hold the data type of the volumes in each column. This is initialized
in the constructor using longest line in the file. The second is an array of the class Columns. The array will be 
the same size as the longest line in the file. This class is where all of the content of the file is held.
<br>
There are 3 functions in the class that are used for command line flags. The first, called column_type,
references the variable setup_column to determine the type at the given index and returns the type as
string. The second and third, get_value and is_empty, call a function on the Column instance at the
given index of the public variable named contents. 

There is a function called populate_columns, which is called by the FileReader for each line in the
file. It parses through each value in the file and adds it to the columns based on the schema. If
a value does not match the schema it will replace it with "Empty Value" and this will be printed out
if the user tries to reference the value.
<br><br>
_Column_<br>
This class will hold the actual values from the file. There will be a instances of this
class in the File class but each instance will only have values from one column. This class has a setter
function which places all the value for files in a column. It also has two getter functions used for
command line flags. Based on the command line inputs the correct column should already be selected,
these functions only need the row index to return a value.
