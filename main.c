#include <stdio.h>
#include <getopt.h>
//#include "converter.c"

int main(int argc, char *argv[])
{
    int option;

    while ((option = getopt(argc, argv, ":c:o:p:")) != -1)
    {
        switch (option)
        {
        case 'c':
            printf("Entrou no c\n");
            printf("%s\n",optarg);
            break;
        case 'o':
            printf("entrou no o\n");
            printf("%s\n",optarg);
            break;
        case 'p':
            printf("entrou o p\n");
            printf("%s\n",optarg);
            break;
        default: // Invalid params
            printf("Usage: -c <ConfigFileName> -o <OutputFileName> -p <ProgramName>\n");
        }
    }
    return 0;
}