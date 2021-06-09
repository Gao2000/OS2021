#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_NUMBER 0xFFFFF000
#define OFFSET 0x00000FFF

int 
main(int argc, char *argv[]) 
{
    int pageNumber, offset;
    unsigned int entry;
    entry = (unsigned int)atoi(argv[1]);

    if (argc != 2) {
        fprintf(stderr,"Please enter: ./assign4_1 followed by the virtual address in decimal\n");
        return -1;
    }

    printf("The address %d contains:\n",entry);

    //mask the page number 
    pageNumber = (entry & PAGE_NUMBER) >> 12; // 4KB = 2^12B
    offset = entry & OFFSET;
    printf("page number = %d\n",pageNumber);
    printf("offset = %d\n",offset);

    return 0;
}