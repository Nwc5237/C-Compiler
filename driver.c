#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

int main(int argc, char *argv[])
{
    printf("beginning parse ...\n");
    char *input = argv[1], *pos = input;
    parse(&pos);
    printf("parse reached null.\n");
    exit(0);
}
