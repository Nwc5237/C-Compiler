compiler: driver.c parser.c lexer.c
	gcc -o compiler lexer.c parser.c driver.c -ggdb
