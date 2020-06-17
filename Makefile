compiler: driver.c parser.c lexer.c data_structures.c
	gcc -o compiler lexer.c parser.c driver.c data_structures.c -ggdb
