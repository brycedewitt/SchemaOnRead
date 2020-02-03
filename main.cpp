#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;

#include "filemanager.h"
#include "filereader.h"

///////////// Testing
// Test print_col_type
void test1() {
  FileReader * f = new FileReader("testfile3.txt", 0, 310);
  if (f->print_col_type(0) != "STRING") {
    std::cout << "Error in T1.1" << '\n';
    exit(1);
  }
  if (f->print_col_type(1) != "BOOL") {
    std::cout << "Error in T1.2" << '\n';
    exit(1);
  }
  if (f->print_col_type(2) != "FLOAT") {
    std::cout << "Error in T1.3" << '\n';
    exit(1);
  }
  if (f->print_col_type(3) != "INT") {
    std::cout << "Error in T1.4" << '\n';
    exit(1);
  }
  delete f;
}
void test2() {
  FileReader * f = new FileReader("testfile3.txt", 0, 310);
  if (strcmp(f->get_value(1,0), "1")) {
    std::cout << "Error in T2.1" << '\n';
    exit(1);
  }
  if (strcmp(f->get_value(1,1), "1")) {
    std::cout << "Error in T2.2" << '\n';
    exit(1);
  }
  if (strcmp(f->get_value(2, 2), "2.2")) {
    std::cout << "Error in T2.3" << '\n';
    exit(1);
  }
  if (!strcmp(f->get_value(3, 2), "Invalid index")) {
    std::cout << "Error in T2.4" << '\n';
    exit(1);
  }
  delete f;
}
void test3() {
  FileReader * f = new FileReader("testfile3.txt", 0, 310);
  if (f->is_empty(0,0) != 1) {
    std::cout << "Error in T3.1" << '\n';
    exit(1);
  }
  if (f->is_empty(1,1) != 1) {
    std::cout << "Error in T3.2" << '\n';
    exit(1);
  }
  if (f->is_empty(2,2) != 1) {
    std::cout << "Error in T3.3" << '\n';
    exit(1);
  }
  if (f->is_empty(3,3) != 1) {
    std::cout << "Error in T3.4" << '\n';
    exit(1);
  }
  delete f;
}

void runTests() {
  test1();
  test2();
  test3();
}



int main(int argc, char* argv[]) {
  char* filepath;
  size_t from;
  size_t length;
  size_t print_col_type;
  size_t print_col_idx_x;
  size_t print_col_idx_y;
  size_t is_missing_idx_x;
  size_t is_missing_idx_y;

  bool i1, i2, i3, type, index, missing, t = false;

  for (size_t i = 0; i < argc; i++) {
    if (strcmp("-f", argv[i]) == 0) {
      filepath = argv[i + 1];
      i1 = true;
    }
    if (strcmp(argv[i], "-from") == 0) {
      from = atoi(argv[i + 1]);
      i2 = true;
    }
    if (strcmp(argv[i], "-len") == 0) {
      length = atoi(argv[i + 1]);
      i3 = true;
    }

    if (strcmp(argv[i], "-print_col_type") == 0) {
      print_col_type = atoi(argv[i + 1]);
      type = true;
    }
    if (strcmp(argv[i], "-print_col_idx") == 0) {
      print_col_idx_x = atoi(argv[i + 1]);
      print_col_idx_y = atoi(argv[i + 2]);
      index = true;

    }
    if (strcmp(argv[i], "-is_missing_idx") == 0) {
      is_missing_idx_x = atoi(argv[i + 1]);
      is_missing_idx_y = atoi(argv[i + 2]);
      missing = true;
    }
    if (strcmp(argv[i], "-t") == 0) {
      runTests();
      i1 = true;
      i2 = true;
      i3 = true;
      filepath = "testfile3.txt";
      from = 0;
      length = 400;
      std::cout << "Tests Completed" << '\n';
    }
  }
  if (!i2) {
    from = 0;
  }
  if (!i3) {
    length = SIZE_MAX - 5;
  }
  if (!(i1)) {
    std::cout << "This program requires the -f, -from, and -len flags to be set.  Please try running this again with the required inputs." << '\n';
    exit(1);
  }
  FileReader * f = new FileReader(filepath, from, length);

  if (type) {
    std::cout << f->print_col_type(print_col_type) << endl;
  }
  else if (index) {
    std::cout << f->get_value(print_col_idx_x, print_col_idx_y) << endl;
  }
  else if (missing) {
    std::cout << f->is_empty(is_missing_idx_x, is_missing_idx_y) << endl;
  }
  delete f;
  return 0;
}
