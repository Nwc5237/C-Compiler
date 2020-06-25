compiler: driver.c parser.c lexer.c utilities.c
	gcc -o compiler lexer.c parser.c driver.c utilities.c -ggdb
