#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generateRandomArray(double array[], int size){
    srand(time(NULL));  
    for (int i = 0; i < size; i++) {
        //array[i] = ((double)rand() / RAND_MAX) * 100. - 100.;
        array[i] = drand48() * 100. - 100.;
        //printf("%lf ", array[i]);
    }
}

int main(int argc, char *argv[]){

    int ARRAY_SIZE = atoi(argv[1]);
    char *filename = argv[2];

    double array[ARRAY_SIZE];

    generateRandomArray(array, ARRAY_SIZE);

    FILE *file = fopen(filename, "wb");
    if(file == NULL){
        fprintf(stderr, "Error opening the file.\n");
        return 1;
    }

    fwrite(array, sizeof(double), ARRAY_SIZE, file);
    fclose(file);

    printf("Array generated and written to %s.\n", filename);

    return 0;
}
