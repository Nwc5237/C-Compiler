#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "parser.h"

int main(int argc, char *argv[])
{
    printf("Beginning parse...\n");
    char *input = argv[1], *pos = input;

    if(parse(&pos))
        printf("Parse terminated successfully.\n");
    else
        printf("Parse terminated with an error.\n");
    
    exit(0);
}
