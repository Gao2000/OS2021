// hw9_34.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
int refer_len = 10;

int PF_FIFO(int *, int);
int  PF_LRU(int *, int);

int main(int argc, char *argv[]){
    long page_frame = strtol(argv[1], NULL, 10);
    // printf("%ld\n", page_frame);
    srand(time(NULL));
    int refer_str[10];
    // int refer_str[10] = {3,0,6,0,2,4,6,8,2,6};
    printf("Reference string:");
    for(int i = 0; i < 10; i++){
        refer_str[i] = rand() % 7 + 1;
        printf(" %d", refer_str[i]);
    }
    printf("\n");

    // FIFO
    int FIFO = PF_FIFO(refer_str, page_frame);
    printf("page fault under FIFO algorithm: %d\n", FIFO);
    // LRU
    int LRU = PF_LRU(refer_str, page_frame);
    printf("page fault under LRU algorithm: %d\n", LRU);
    return 0;
}

void print_array(int * in){
    for(int i = 0; i <= sizeof(in) / sizeof(in[0]); i++)
        printf(" %d", in[i]);
    printf("\n");
}

int PF_LRU(int *refer, int page_frame){
    int PF = 0;
    int page[page_frame];
    for(int i = 0; i < page_frame; i++) page[i] = refer[i];
    PF += page_frame;
    // int order[page_frame] = {};
    // for(int i = 0; i < page_frame; i++) order[i] = i;

    for(int i = page_frame; i < refer_len; i++){
        bool in = false;
        // check if refer[i] in the page
        int p = 0;
        for(int j = 0; j < page_frame; j++)
            if(page[j] == refer[i]){
                in = true;
                p  = j;
            }
        // in the page
        // update the order
        if(in){
            int temp[refer_len];
            for(int j = 0, k = 0; j < page_frame; j++){
                if(j != p){
                    temp[k] = page[j];
                    k++;
                }
            }
            temp[page_frame-1] = refer[i];
            for(int j = 0; j < page_frame; j++) page[j] = temp[j];
            // print_array(page);
        }
        else{
            PF++;
            int temp[refer_len];
            for(int j = 0; j < page_frame - 1; j++){
                    temp[j] = page[j+1];
            }
            temp[page_frame-1] = refer[i];
            for(int j = 0; j < page_frame; j++) page[j] = temp[j];
            // print_array(page);
        }
    }
    return PF;
}

int  PF_FIFO(int *refer, int page_frame){
    int PF = 0;
    int page[page_frame];
    for(int i = 0; i < page_frame; i++) page[i] = refer[i];
    PF += page_frame;

    for(int i = page_frame; i < refer_len; i++){
        bool in = false;
        // check if refer[i] in the page
        for(int j = 0; j < page_frame; j++)
            if(page[j] == refer[i])
                in = true;
        // not in the page
        // update the order
        if(!in){
            page[PF%page_frame] = refer[i];
            PF++;
        }
    }
    return PF;
}