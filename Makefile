comp:
	g++ -O0 -o sorer -g main.cpp

docker:
	g++ -Wall -o sorer -std=c++11 main.cpp

run:
	./sorer -f testfile3.sor -from 0 -len 100 -print_col_type 0

clean:
	rm -rf a.out
	rm -rf a.out.dSYM\

debug:
	g++ -std=c++11 -Wall -O0 -o sorer -g main.cpp && lldb ./sorer -f 3.sor -print_col_type 4 core

valgrind:
	clang++ -std=c++11 -Wall -g -O0 main.cpp && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./a.out -f testfile3.sor -from 0 -len 110


all:
	-./sorer -f 0.sor -print_col_type 0
	-./sorer -f 0.sor -is_missing_idx 0 0
	-./sorer -f 1.sor -print_col_type 0
	-./sorer -f 1.sor -is_missing_idx 0 1
	-./sorer -f 1.sor -print_col_idx 0 3
	-./sorer -f 2.sor -print_col_type 0
	-./sorer -f 2.sor -print_col_type 1
	-./sorer -f 2.sor -print_col_type 2
	-./sorer -f 2.sor -print_col_type 3
	-./sorer -f 2.sor -is_missing_idx 1 0
	-./sorer -f 2.sor -is_missing_idx 1 1
	-./sorer -f 2.sor -print_col_idx 3 0
	-./sorer -f 2.sor -print_col_idx 3 1
	-./sorer -f 2.sor -print_col_idx 2 0
	-./sorer -f 2.sor -print_col_idx 2 1
	-./sorer -f 3.sor -print_col_type 4
	-./sorer -f 3.sor -print_col_idx 2 10
	-./sorer -f 3.sor -print_col_idx 2 384200
	-./sorer -f 4.sor -print_col_idx 0 1
	-./sorer -f 4.sor -print_col_idx 0 2
	-./sorer -f 4.sor -print_col_idx 1 1
	-./sorer -f 4.sor -print_col_idx 1 2
	-./sorer -f 1.sor -from 1 -len 74 -print_col_type 0
	-./sorer -f 1.sor -from 1 -len 74 -is_missing_idx 0 0
	-./sorer -f 1.sor -from 1 -len 74 -print_col_idx 0 6
