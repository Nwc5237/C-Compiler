#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "parser.h"

int main(int argc, char *argv[])
{
    char *input = argv[1], *pos = input;
    char *i = argv[1];
    
    printf("Beginning parse...\n");
    parse(&pos);
    printf("Parsed:\n");

    while(i < pos)
    {
        printf("%c", *i);
        i++;
    }
    printf("\nDone.\n");
    
    exit(0);
}
