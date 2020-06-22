#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lexer.h"
#include "parser.h"

int main(int argc, char *argv[])
{
    char *input = malloc(sizeof(char) * strlen(argv[1]) + 2);
    char *i;
    token *tok;

    input = strcat(strdup(argv[1]), "$");
    i = input;

    printf("Beginning parse...\n");
    parse(&input);
    printf("Parsed:\n");

    while(i < input)
    {
        printf("%c", *i);
        i++;
    }
    printf("\nDone.\n");

    exit(0);
}
