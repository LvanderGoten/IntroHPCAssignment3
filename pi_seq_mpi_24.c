#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define SEED     921
#define NUM_ITER 10000000000


int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
    double t1, t2; 
    t1 = MPI_Wtime();

    int process_id;
    int num_processes;

    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
#ifdef DEBUG
    printf("Initialized process %d/%d\n", process_id, num_processes);
#endif

    if (num_processes % 2 != 0 && process_id == 0) {
        printf("Number of processes needs to be a power of two!\n");
        MPI_Finalize();
        return 0;
    }

    long long count = 0;
    double x, y, z, pi;

    srand(process_id * SEED); // Important: Multiply SEED by "rank" when you introduce MPI!

    // Calculate PI following a Monte Carlo method
    long long iter_per_proc = (long long) NUM_ITER / (long long) num_processes;
    if (process_id == 0) {
        iter_per_proc += NUM_ITER % num_processes;
    } 

#ifdef DEBUG
    printf("Num iterations %ld [process-id %d]\n", iter_per_proc, process_id);
#endif

    for (long long iter = 0; iter < iter_per_proc; iter++)
    {
        // Generate random (X,Y) points
        x = (double)random() / (double)RAND_MAX;
        y = (double)random() / (double)RAND_MAX;
        z = x * x + y * y;        
        // Check if point is in unit circle
        if (z <= 1.0)
        {
            count++;
        }
    }

    // Estimate Pi and display the result
    if (process_id == 0)
    {
        long long global_count = 0;
        long long *counts = malloc(sizeof(long long) * num_processes);

        // Gather counts
        MPI_Gather(&count, 1, MPI_LONG_LONG, counts, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
        
        for (int p_id = 0; p_id < num_processes; p_id++) {
            global_count += counts[p_id];
        }

        pi = ((long double) global_count / (long double) NUM_ITER) * 4.0;
        t2 = MPI_Wtime(); 
        printf("RESULT=%f\tTIME=%f\tPROGRAM=24\tNPROC=%d\n", pi, t2 - t1, num_processes);

        free(counts);
    } else {
        // Gather counts
        MPI_Gather(&count, 1, MPI_LONG_LONG, NULL, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}

