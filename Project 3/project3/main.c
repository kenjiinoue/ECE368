//
//  main.c
//  project3
//
//  Created by Kenji Inoue on 4/19/15.
//  Copyright (c) 2015 Kenji Inoue. All rights reserved.
//  butt 2.0

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct Profile{
    int id;
    int age;
    int gender;
    int mStat;
    int race;
    int bPlace;
    int lang;
    int occ;
    int inc;
}Vertex;

typedef struct DistanceID{ //Used to store an ID and Distance for query 1
    int id;
    int distance;
}Dist;

char * * explode(const char * str, const char * delims, int * arrLen);
double findDistance(Vertex v1, Vertex v2);
void createMatrix(int length, int blankarr[][length], double distArr[][length], double delta);
void printMatrix(int length, int matrix[][length]);
void query1(int length, int dense[][length], double distarr[][length], int user1, int ds);
void query2(int length, int dense[][length],double distarr[][length],int user1, double alpha, int ds);
void query3(int length, int dense[][length], double distarr[][length], int user1, int ds);
void query4(int length, int dense[][length], int hop2d[][length], int user1, int ds);
void query5(int length, int dense[][length], int ds);
void query6(int length, int dense[][length], int hop2d[][length]);
void q2(int length, int alpha, int id, int dense[][length], double distarr[][length], int ids[], int index);
int sort_dist(const void *a, const void *b);

int main(int argc, const char * argv[]) {

    FILE * fp;
    int numIDs;
    double d1;
    double d2;
    int user1;
    double alpha;

    int i;
    
    const char * filename = argv[1];
    
    fp = fopen (filename, "r");
    
    if(fp == NULL) {
        printf("bad file. file pointer is null\n");
        return(0);
    }
    char line [1024];
    fgets(line, sizeof(line), fp);
    int length = 1024;
    const char * delims = " ,\0\n";
    
    
    char ** lineArr = explode(line, delims, &length);
    numIDs = atoi(lineArr[0]);
    d1 = atof(lineArr[1]);
    d2 = atof(lineArr[2]);
    user1 = atoi(lineArr[3]);
    alpha = atof(lineArr[4]);
    i = 0;
    Vertex * users = malloc(sizeof(Vertex) * numIDs); //array of users. This will be where the program fetches different parameters.
    while(!feof(fp)){
        fgets(line, sizeof(line), fp);
        lineArr = explode(line, delims, &length);
        users[i].age = atoi(lineArr[1]);
        users[i].gender = atoi(lineArr[2]);
        users[i].mStat = atoi(lineArr[3]);
        users[i].race = atoi(lineArr[4]);
        users[i].bPlace = atoi(lineArr[5]);
        users[i].lang = atoi(lineArr[6]);
        users[i].occ = atoi(lineArr[7]);
        users[i].inc = atoi(lineArr[8]);
        i++;
        
        //fprintf(stdout,"%s",line); //print the file contents on stdout.
    }
    
    double dMatrix[numIDs][numIDs];
    i = 0;
    int j = 0;
    double distTmp;
    double UdMax = 0;
    for(i = 0; i < numIDs; i++) {
        for(j = i; j < numIDs; j++) {
            distTmp = findDistance(users[i], users[j]);
            if(distTmp > UdMax) {
                UdMax = distTmp;
            }
            if(i == j) {
                dMatrix[i][j] = 0.00000;
            }
            else{
                dMatrix[i][j] = distTmp;
                dMatrix[j][i] = distTmp;
                //printf("D from %d to %d: %f\n", i+1, j+1, distTmp);
            }
        }
    }
    //printf("\n");

    for(i = 0; i < numIDs; i++) {              //Normalizes Euclidian Distance
        for(j = 0; j < numIDs; j++) {
            //printf("%f ",dMatrix[i][j]);
        }
        //printf("\n");
    }
    //printf("\n");

    for(i = 0; i < numIDs; i++) {              //Normalizes Euclidian Distance
        for(j = 0; j < numIDs; j++) {
            if(i != j){
                distTmp = 1 - (dMatrix[i][j]/UdMax);
                dMatrix[i][j] = (int)(distTmp * 100);
                //printf("Normalized D from %d to %d: %f\n", i, j, dMatrix[i][j]);
                
            }
            else{
                dMatrix[i][j] = -1;
            }
            //printf("%.0f ",dMatrix[i][j]);
        }
        //printf("\n");
    }
    
    int matx1[numIDs][numIDs];                     //Edge Matrix for delta1;
    createMatrix(numIDs, matx1, dMatrix, d1);
    int hop2d[numIDs][numIDs];                     //2 hop matrix
    //printMatrix(numIDs, matx1);
    
    int ds = 1;
    //QUERY ONE
    query1(numIDs, matx1,dMatrix, user1, ds);
    //QUERY TWO
    query2(numIDs, matx1,dMatrix, user1, alpha, ds);
    //QUERY THREE
    query3(numIDs, matx1,dMatrix, user1, ds);
    //QUERY FOUR

    query4(numIDs, matx1, hop2d, user1, ds);
    //QUERY FIVE
    query5(numIDs, matx1, ds);
    //QUERY SIX
    query6(numIDs, matx1, hop2d);
    printf("\n");
    //SPARSE
    ds = 0;
    int matx2[numIDs][numIDs];                     //Edge Matrix for delta2;
    createMatrix(numIDs, matx2, dMatrix, d2);
    int hop2s[numIDs][numIDs];
    //QUERY ONE
    query1(numIDs, matx2,dMatrix, user1, ds);
    //QUERY TWO
    query2(numIDs, matx2,dMatrix, user1, alpha, ds);
    //QUERY THREE
    query3(numIDs, matx2,dMatrix, user1, ds);
    //QUERY FOUR
    query4(numIDs, matx2, hop2s, user1, ds);
    //QUERY FIVE
    query5(numIDs, matx2, ds);
    //QUERY SIX
    query6(numIDs, matx2, hop2s);
    
    fclose(fp);
    return 0;
}

void query6(int length, int dense[][length], int hop2d[][length]){
    int i;
    int j;
    int count = 0;
    for(i = 0; i < length; i++) {
        for(j = 0; j < length; j++) {
            if(hop2d[i][j]){
                count++;
            }
        }
    }

    float avgNodes = ((float)count)/((float)length);

    printf("%.2f\n", avgNodes);
    return;
}

void query5(int length, int dense[][length], int ds){
    int i;
    int j;
    float count = 0;
    for(i = 0; i < length; i++) {
        for(j = 0; j < length; j++) {
            if(dense[i][j] == 1){
                count++;
            }
        }
    }
    count /= (float)length;
    printf("\n%.2f\n", count);
    return;
}

void query4(int length, int dense[][length], int hop2d[][length], int user1, int ds){
    int i;
    int j;
    int k;
    user1 = user1-1;
    Dist * dists = malloc(sizeof(Dist) * length);

    for(i = 0; i < length; i++) {
        for(j = 0; j < length; j++){
            hop2d[i][j] = 0;
        }
    }
    
    for(i = 0; i < length; i++) {
        for(j = 0; j < length; j++){
            for(k = 0; k < length; k++) {
                if((i != j) && (i != k) && dense[i][j] && dense[j][k]){
                    hop2d[i][k] = 1;
                }
            }
        }
    }
    int distsCount = 0;
    //printf("\n");
    //printMatrix(length, dense);
    //printMatrix(length, hop2d);
    for(i = 0; i < length; i++){
        if(hop2d[user1][i]){
            dists[distsCount].id = i+1;
            distsCount++;
        }
    }
    printf("\n%d,", distsCount);
    for(i = 0; i < distsCount; i++){
        printf("%d",dists[i].id);
        if(i < distsCount-1) {
            printf(",");
        }
    }

    free(dists);
    return;
}

void query3(int length, int dense[][length], double distarr[][length], int user1, int ds){
    int i;
    Dist * dists = malloc(sizeof(Dist) * length);
    int distsCount = 0;
    
    for(i = 0; i < length; i++){
        if(dense[user1 - 1][i] == 1){
            dists[distsCount].id = i+1;
            dists[distsCount].distance = distarr[user1-1][i];
            distsCount++;
        }
    }
    
    printf("%d,", distsCount);
    for(i = 0; i < distsCount; i++){
        printf("%d",dists[i].id);
        if(i < distsCount-1) {
            printf(",");
        }
    }
        free(dists);
}
int sortInt(const void *a, const void *b){
    return ( *(int*)a - *(int*)b );
}
void q2(int length, int alpha, int id, int dense[][length], double distarr[][length], int ids[], int index){
    if(alpha<0){
        return;
    }
    ids[index] = id+1;
    int i;
    for(i = 0; i < length; i++) {
        if(i != id && dense[id][i]==1) {
            q2(length, alpha-distarr[id][i], i, dense, distarr, ids, ++index);
        }
    }
    return;
}
void query2(int length, int dense[][length],double distarr[][length],int user1, double alpha, int ds) {
    int ialpha = (int)(alpha * 100);
    int i;
    int count = 0;
    int j;
    j = length * length;
    int index = 0;
    int ids[j];
    user1--;
    for(i = 0; i < j; i++) {
        ids[i] = 0;
    }
    q2(length, ialpha, user1, dense, distarr, ids, index);

    qsort(ids, j, sizeof(int), sortInt);
//    printf("\n");
//    for(i = 0; i < j; i++) {
//        printf("%d ", ids[i]);
//    }
    int k = 0;
    int prev = 0;
    count = 0;
    //int asdf = user1;
    while(k < j) {
        if(ids[k]!= user1+1 && (ids[k] > prev)) {
            count++;
        }
        prev = ids[k];
        k++;
    }
    
    printf("\n%d\n", count);
    return;
}

void query1(int length, int dense[][length], double distarr[][length], int user1, int ds){
    int i;
    Dist * dists = malloc(sizeof(Dist) * length);
    int distsCount = 0;
    int minDist = 1000;
    
    for(i = 0; i < length; i++){
        if(dense[user1 - 1][i] == 1){
            dists[distsCount].id = i+1;
            dists[distsCount].distance = distarr[user1-1][i];
            if(dists[distsCount].distance < minDist){
                minDist = dists[distsCount].distance;
            }
            distsCount++;
        }
    }
    
    //qsort(dists, distsCount, sizeof(Dist), sort_dist);
    printf("%.2f",(float)minDist/100);
    for(i = 0; i < distsCount; i++){
        if(dists[i].distance == minDist) {
            printf(",%d",dists[i].id);
        }

    }
    
    free(dists);
    return;
}

int sort_dist(const void *a, const void *b){
    Dist *ia = (Dist *)a; // casting pointer types
    Dist *ib = (Dist *)b;
    return (ib->distance - ia->distance);
}

void createMatrix(int length, int blankarr[][length], double distArr[][length], double delta){
    int i;
    int j;
    delta = delta * 100;
    for(i = 0; i < length; i++){
        for(j = i; j < length; j++) {
            if(distArr[i][j] > delta) {
                blankarr[i][j] = 1;
                blankarr[j][i] = 1;
            }
            else{
                blankarr[i][j] = 0;
                blankarr[j][i] = 0;
            }
            
        }
    }
    //printMatrix(length, blankarr);
    return;
}
void printMatrix(int length, int matrix[][length]){
    int i;
    int j;
    for(i = 0; i < length; i++){
        for(j = 0; j < length; j++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    return;
}

double findDistance(Vertex v1, Vertex v2) {
    double ud;
    ud = pow(v1.age-v2.age, 2) + pow(v1.gender-v2.gender, 2) + pow(v1.mStat-v2.mStat, 2) + pow(v1.race-v2.race, 2) + pow(v1.bPlace-v2.bPlace, 2) +pow(v1.lang-v2.lang, 2)+pow(v1.occ-v2.occ, 2)+pow(v1.inc-v2.inc, 2);
    ud = sqrt(ud);
    return(ud);
}

char * * explode(const char * str, const char * delims, int * arrLen){
    int N = 0;
    int pos = 0;
    while(str[pos] != '\0'){
        if(strchr(delims,str[pos]) != NULL){
            N++;
        }
        pos++;
    }
    char * * strArr = malloc((N+1) * sizeof(char *));
    pos = 0;
    int arrInd = 0; // this is the next position where we'll create a string
    int last = 0; // 1 + the last index we saw a delimiter. Init to 0.
    int diff = 0;
    while(str[pos] != '\0'){
        if(strchr(delims,str[pos]) != NULL){
            diff = pos - last;
            if(diff != 0){
                strArr[arrInd] = malloc((diff) * sizeof(char *));
                memcpy(strArr[arrInd], &str[last] , (diff)*sizeof(char));
                strArr[arrInd][diff] = '\0';
            }
            else{
                strArr[arrInd] = malloc((1) * sizeof(char *));
                strArr[arrInd][diff] = '\0';
            }
            last = pos + 1;
            arrInd++;
        }
        pos++;
    }
    
    diff = strlen(str) - last;
    
    if(diff != 0){
        strArr[arrInd] = malloc((diff) * sizeof(char *));
        memcpy(strArr[arrInd], &str[last] , (diff)*sizeof(char));
        strArr[arrInd][diff] = '\0';
    }
    else{
        strArr[arrInd] = malloc((1) * sizeof(char *));
        strArr[arrInd][diff] = '\0';
    }
    
    *arrLen = N+1;
    
    return (char **)strArr;
}



