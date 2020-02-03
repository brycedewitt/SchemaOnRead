#include <iostream>
#include <stdio.h>

class FileReader {
public:
  size_t max_columns;
  size_t length;
  size_t start;
  size_t end;
  File* f;
  char* longest_line_text;
  size_t longest_line_length;
  char filepath[30];
  char** longest_line_array;

  FileReader(char* fp, size_t s, size_t e) {
    for (size_t i = 0; i < strlen(fp); i++) {
      filepath[i] = fp[i];
    }
    length = 0;
    start = s;
    end = e;
    max_columns = 0;
    get_stats();
    longest_line_array = new char*[max_columns];
    for (size_t i=0; i < max_columns; i++) {
       longest_line_array[i] = new char[longest_line_length];
       for (size_t j=0; j < longest_line_length; j++) {
         longest_line_array[i][j] = '\0';
       }
    }
    strip_tags(longest_line_text);
    make_file();
  }

  ~FileReader() {
    for (size_t i=0; i < max_columns; i++) {
       delete [] longest_line_array[i];
    }
    delete [] longest_line_array;
    delete [] longest_line_text;
    delete f;
  }

  void get_stats() {
    size_t buffer = 1000;
    FILE* src;
    char stream[buffer];


    // Try opening the file
    src = fopen(filepath, "r");
    if (src == NULL){
        cout << "Malformed input doc name.  Please try again." << endl;
    }

    // Check if previous character of starting byte is a newline or start of document
    bool ignore_first_line = false;
    if (start != 0) {
      fseek(src, start - 1, SEEK_SET);
      if (fgetc(src) != 10) {
        ignore_first_line = true;
      }
    }
    // Return starting position indicator to start
    fseek(src, start, SEEK_SET);

    // Run this until reaching new line character (up to 500 char)
    size_t previous_position = 0;
    while ((fgets(stream, buffer, src) != NULL) && (ftell(src) < end))  {
      if (!ignore_first_line) {
        // Save all the words from stream into this list
        if (strlen(stream) > longest_line_length) {
          longest_line_length = strlen(stream);
        }
        if (count_atoms(stream) > max_columns) {
          max_columns = count_atoms(stream);
        };
        length = length + 1;
        previous_position = ftell(src);
      }
      ignore_first_line = false;
    }

    // Check if previous character of starting byte is a newline or start of document
    ignore_first_line = false;
    if (start != 0) {
      fseek(src, start - 1, SEEK_SET);
      if (fgetc(src) != 10) {
        ignore_first_line = true;
      }
    }
    // Return starting position indicator to start
    fseek(src, start, SEEK_SET);

    // Run this until reaching new line character (up to 500 char)
    previous_position = 0;
    size_t acc = 0;
    longest_line_text = new char[longest_line_length];
    while ((fgets(stream, buffer, src) != NULL) && (ftell(src) < end) && acc < 500)  {
      if (!ignore_first_line) {
        // Save all the words from stream into this list
        if (strlen(stream) == longest_line_length) {
          for (size_t i = 0; i < strlen(stream); i++) {
            longest_line_text[i] = stream[i];
          }
        }
        previous_position = ftell(src);
      }
      ignore_first_line = false;
      acc = acc + 1;
    }

    longest_line_text[longest_line_length] = '\0';

    fclose(src);
  }

  size_t count_atoms(char* text) {
    size_t open = 0;
    size_t close = 0;

    for (size_t i = 0; i < strlen(text); i++) {
      if (text[i] == '<') { open++; }
      if (text[i] == '>') { close++; }
    }

    if (open == close) {
      return open;
    } else {
      return 0;
    }
  }

  void make_file() {
    size_t buffer = 1000;
    FILE* src;
    char stream[buffer];
    strip_tags(longest_line_text);
    f = new File(length, max_columns, longest_line_array);

    // Try opening the file
    src = fopen(filepath, "r");

    // Check if previous character of starting byte is a newline or start of document
    bool ignore_first_line = false;
    if (start != 0) {
      fseek(src, start - 1, SEEK_SET);
      if ((fgetc(src) == 10) || (start == 1)) {
        ignore_first_line = true;
      }
    }
    // Return starting position indicator to start
    fseek(src, start, SEEK_SET);

    while ((fgets(stream, buffer, src) != NULL) && (ftell(src) < end))  {
      if (!ignore_first_line) {
        strip_tags(stream);
        f->populate_columns(longest_line_array);
      }
      ignore_first_line = false;
    }
    fclose(src);
  }

  void strip_tags(char* str) {
    for (size_t i=0; i < max_columns; i++) {
       for (size_t j=0; j < longest_line_length; j++) {
         longest_line_array[i][j] = '\0';
       }
    }
    size_t acc = 0;

    // Breaks out the string into seperate arrays
    for (size_t i = 0; i < strlen(str); i++) {
      if (str[i] == '\n') { break; }
      if (str[i] == '<') {
        size_t x = i + 1;
        while (str[x] == ' ') { x = x + 1;} // Ignore preceeding spaces
        size_t y = 0;
        while (str[x] != '>') {
          if (str[x] == '\n') {
            // do nothing here
          } else {
            longest_line_array[acc][y] = str[x];
          }
          x = x + 1;
          y = y + 1;
        }
        // longest_line_array[acc][y] = '>';
        acc = acc + 1;
      }
    }

    // // Cleanup the space on the back
    // for (size_t x = 0; x < max_columns; x++) {
    //   for (size_t y = strlen(str); y > 0; y--) {
    //     if (longest_line_array[x][y] == '>') {
    //       longest_line_array[x][y] = '\0';
    //     }
    //   }
    // }

    // DEBUG
    // for (size_t i = 0; i < max_columns; i++) {
    //   for (size_t x = 0; x < strlen(str); x++) {
    //     if (longest_line_array[i][x] == '\0') { break; }
    //     // if (longest_line_array[i][x] == ' ') {std::cout << "fuck" << '\n';}
    //     else { std::cout << longest_line_array[i][x]; }
    //   }
    //   cout << "\n";
    // }
  }

  char* print_col_type(size_t i) {
    return f->column_type(i);
  }

  char* get_value(size_t column, size_t row) {
    return f->get_value(column, row);
  }

  bool is_empty(size_t column, size_t row) {
    return f->is_empty(column, row);
  }
};
