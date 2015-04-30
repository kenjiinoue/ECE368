//
//  main.c
//  project2
//
//  Created by Kenji Inoue on 3/12/15.
//  Copyright (c) 2015 Kenji Inoue. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h> 
#include <math.h>
#include <time.h>

#define SIZE_ARR 1000000
#define SIZE_ARRM1 999999

//typedef int (*compfn)(const void*, const void*);
void sort(int list[], int n);
int compar(const void * i, const void * j);


int main(int argc, const char * argv[]) {
    int arr[SIZE_ARR] = {0};
    int i;
    for(i = 0; i < SIZE_ARRM1; i++) {
        arr[i] = rand()%10000;
    }
    
    
    clock_t begin;
    clock_t end;
    
    double time_spent;
    
    begin = clock();
    sort(arr,SIZE_ARR);
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;


    
    for(i = 0; i < SIZE_ARRM1; i++) {
        arr[i] = i;
    }
    
    begin = clock();
    sort(arr,SIZE_ARR);
    end = clock();
    double time_spent_sortedinput = (double)(end-begin)/CLOCKS_PER_SEC;
    
    for(i = 0; i < SIZE_ARRM1; i++) {
        arr[i] = 10000-i;
    }
    
    begin = clock();
    sort(arr,SIZE_ARR);
    end = clock();
    double time_spent_backwardsinput = (double)(end-begin)/CLOCKS_PER_SEC;
    
    printf("Runtime Random: %f\n", time_spent);
    printf("Runtime Sorted: %f\n", time_spent_sortedinput);
    printf("Runtime Reverse: %f\n", time_spent_backwardsinput);
    return 0;
}


void sort(int list[], int n){
    int i;
    int sorted = 1;
    for(i = 0; i < n; i++) {
        if (list[i] >= list[i+1]){
            i = n+1;
            sorted = 0;
        }
    }
    if(sorted == 0) {
        qsort(list, n, sizeof(int), compar);
    }
    return;
}

int compar(const void * i, const void * j) {
    const int *ia = (const int *)i;
    const int *ib = (const int *)j;
    return (*ia  - *ib);
}


