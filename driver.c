#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "parser.h"

int main(int argc, char *argv[])
{
    printf("Beginning parse...\n");
    char *input = argv[1], *pos = input;
    parse(&pos);
    exit(0);
}
