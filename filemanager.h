
#include <stdio.h>
#include <stdlib.h>
#include <string>


// Enum that holds each type used in input files
enum types { STRING, FLOAT, INT, BOOL};

//Todo: this class does not know what type each of the columns is, it works but feels weird
//holds one (1) column
class Column {
public:
    size_t num_rows;
    char** contents; // list of list of strings that holds that values from the file
    size_t size; // list of ints that describe the size of each corresponding column


    //Constructor for ColumnObject, takes in a number of rows and columns and initializes a
    // char** with the that number of columns and rows
    // Also creates an int array, sizes, that keeps track of the size of each column
    Column() {
        num_rows = 0;
        size = 0;
    }

    ~Column() {
        for (size_t i = 0; i < num_rows; i++)
            contents[i] = nullptr;
        delete this;
    }

    void add_rows(size_t rows) {
        num_rows = rows;
        contents = new char*[999999]();
    }

    // add c to the end of contents
    // type check
    // resize array if needed
    void add(char* c) {
        char* value = new char[strlen(c)];
        for (size_t i = 0; i < strlen(c); i++) {
          value[i] = c[i];
        }
        contents[size] = value;
        size++;
    }

    void addStr(char* c) {
        char* value = new char[strlen(c) + 2];
        value[0] = (char)34;
        for (size_t i = 0; i < strlen(c); i++) {
          value[i+1] = c[i];
        }
        value[strlen(c) + 1] = (char)34;
        contents[size] = value;
        size++;
    }

    char* get_string(size_t row) {
        return contents[row];
    }

    bool is_empty(size_t row) {
        return get_string(row) == nullptr ? 1 : 0;
    }

};

/*
 * This class is to hold and operate on the contents of the file. It will be the middle ground
 * after the file is read in, but before it is redistributed into the column data format
 */
class File {
public:
    size_t num_rows; //number of rows in the input file
    size_t num_cols; //number of columns in the longest line in the first 500 lines of the input file
    Column* columns; // pointer to new column data format that will hold file contents
    types* column_setup; // array of types, in order of longest line in file, which will be the setup for new column data format

    File (size_t rows, size_t cols, char** longest_line) {
        num_rows = rows;
        num_cols = cols;
        columns = new Column[num_cols]();

        for (size_t i = 0; i < num_cols; i++) {
            columns[i].add_rows(num_rows);
        }

        column_setup = new types[num_cols];
        setup_columns(longest_line);
    }

    // takes in string and determines which type it belongs to
    // regex for alphanumeric strings founds: https://stackoverflow.com/questions/7607260/check-non-numeric-characters-in-string
    types determine_type(char* value) {
        types type;
        size_t size = strlen(value);
        int ascii_val;

        // if the value starts and ends with quotes, it's a string. If it does not and it contains a space, it's invalid
        if ((int)value[0] == 34 && (int)value[size - 1] == 34) {
            type = STRING;
            return type;
        } else if (strstr(value, " ") != nullptr) {
            //TODO: remove row
            type = STRING;
            return type;
        }

        // if the size is 1 and the value is 0 or 1, it is a bool
        if ((size == 1 && ((int)value[0] == 48 || (int)value[0] == 49)) || size == 0) {
            type = BOOL;
            return type;
        }

        for (size_t i = 0; i < size; i++) {
            ascii_val = (int)value[i];

            //if the value is a number, +, -, or . then continue on, it could be a float or int, if not, it's a string
            if (!((ascii_val >= 48 && ascii_val <= 57) || ascii_val == 43 || ascii_val == 45 || ascii_val == 46)) {
                type = STRING;
                return type;
            }

        }

        //TODO: could be more than one of these values and it would be a stirng not float
        if (strstr(value, "+") != nullptr || strstr(value, "-") != nullptr || strstr(value, ".") != nullptr) {
            type = FLOAT;
            return type;
        } else {
            type = INT;
            return type;
        }
        type = STRING;
        return type;
    }

    // Function that is used in the constructor. It determines the type of each value in the longest
    // line in the file, and puts th type in an array of enums for later reference
    void setup_columns(char** line) {
        for (size_t i = 0; i < num_cols; i++) {
            column_setup[i] = determine_type(line[i]);
         }
    }


    void populate_columns(char** line) {
        types type;
        for (size_t j = 0; j < num_cols; j++) {
            type = determine_type(line[j]);

            if (strlen(line[j]) == 0) {
              char* x = new char[1];
              x[0] = '\n';
              columns[j].add(x);
            }
             if (type == column_setup[j]) {
              columns[j].add(line[j]);
            } else {
               //All empty values will be entered as string Empty Value
              columns[j].addStr((char*)line[j]);
            }

        }
    }

    void check_input_validity(size_t col, size_t row) {
        if (col > num_cols - 1 || row > num_rows - 1 || col < 0 || row < 0) {
            std::cout << "0" << std::endl;
            exit(0);
        }
    }

    char* get_value(size_t col, size_t row) {
        check_input_validity(col, row);
        return columns[col].get_string(row);
    }

    bool is_empty(size_t col, size_t row) {
        check_input_validity(col, row);
        return strcmp(columns[col].get_string(row), "");
    }

    //returns type of column at index
    char* column_type(size_t index) {
        check_input_validity(index, 0);
        types type = column_setup[index];
        if (column_setup[index] > 4) { return (char*)"BOOL"; };
        switch(type) {
            case STRING:
                return (char*)"STRING";
                break;
            case FLOAT:
                return (char*)"FLOAT";
                break;
            case INT:
                return (char*)"INT";
                break;
            case BOOL:
                return (char*)"BOOL";
                break;
        }
    }
};


//TODO: deal with nullptr being passed (it's just of type whatever column its in)
