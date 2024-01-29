#include <stdio.h>
#include <stdlib.h>
#include <valarray>
#include <mpi.h>
#include <omp.h>

void printresult(double *arr, int size, double elapsed_time){
    
    /* To check the result, 
    printf("ARRAY SIZE : %d\n", chunksize);
    printf("Result array :\n");
    for(int i = 0; i < size; i++){
        printf("%lf ", arr[i]);
    }
    printf("\n");
    */
    printf("Elapsed time : %lf\n", elapsed_time);
}

void quickSort(double *arr, int low, int high){
    if(low < high){
        double pivot = arr[high];
        int i = low - 1;

        for(int j = low; j <= high - 1; j++){
            if(arr[j] < pivot){
                i++;
                std::swap(arr[i], arr[j]);
            }
        }

        std::swap(arr[i + 1], arr[high]);

        quickSort(arr, low, i);
        quickSort(arr, i + 2, high);
    }
}

double *merge(double *local, int localsize, double *recv, int recvsize){
    /* TODO : merge two arrays */ 
}

/* 
Argument 1 = array size, Argument 2 = inputfile name, Argument 3 = outputfile name
This code can only handle a process count that is a power of 2.

To run : mpirun -np <process count> <executable> <array size> <input file> <output file>
To test, use a smaller array size (50, 100...)
To check with testcases, qsub -N <jobname> -v MPI_LIBRARY=<OMPI/IMPI> /work1/trkk1224/homework_verified.sh
By default, homework_verified.sh will use OMPI if MPI_LIBRARY not specified
*/
int main(int argc, char *argv[]){

    int rank, size;
    int ARRAY_SIZE = atoi(argv[1]);
    char *input_filename = argv[2], *output_filename = argv[3];
    MPI_File input_file, output_file;
    double start_time, end_time;

    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 

    if(size & (size - 1)){
        if(rank == 0) printf("Please use a process count that is a power of 2.\n");
        MPI_Finalize(); 
        return 0; 
    } 

    double *chunk;
    /* TODO : read a chunk of input to each process
    int chunksize = ...
    int display = ...
    chunk = (double*)malloc...  
    */

    MPI_File_open(MPI_COMM_WORLD, input_filename, MPI_MODE_RDONLY, MPI_INFO_NULL, &input_file);
    MPI_File_read_at(input_file, sizeof(double) * display, chunk, chunksize, MPI_DOUBLE, MPI_STATUS_IGNORE);
    MPI_File_close(&input_file);

    // start parallel merge sort
    start_time = MPI_Wtime();
    // first, sort the local array before iteration
    // you may replace qsort with faster sorting algorithm
    quickSort(chunk, 0, chunksize - 1);

    int recv_chunksize;
    double *result;
    // iterate for log(size) times
    for(int step = 1; step < size; step *= 2){
        if(rank % (2 * step) != 0){
            /* TODO : Send the chunksize and chunk to the pair process 
            MPI_Send(...)   // send chunksize
            MPI_Send(...)   // send chunk
            */
            break;
        }

        double *chunk_received;
        /* TODO : Recv the chunksize and chunk from the pair process, and allocate space for received chunk
        MPI_Recv(...)   // send chunksize
        *chunk_received = (double*)malloc...
        MPI_Recv(...)   // send chunk
        */

        result = merge(chunk, chunksize, chunk_received, recv_chunksize);

        chunksize += recv_chunksize;
        free(chunk);
        free(chunk_received);
        chunk = result;
    }

    end_time = MPI_Wtime();

    /* TODO : Output the sorted array with MPI-IO
    MPI_File_open(...)
    MPI_File_write_at(...)
    MPI_File_close(...)
    */

    if(rank == 0) printresult(chunk, chunksize, end_time - start_time); 

    MPI_Finalize(); 
    return 0; 
}
