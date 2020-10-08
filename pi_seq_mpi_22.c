#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define SEED     921
#define NUM_ITER 10000000000

long long binary_tree(int i, int j, long long count, int process_id) {
    long long target_count;

    // i inclusive, j inclusive
    if (i == j - 1) {
#ifdef DEBUG
        if (process_id == i)
            printf("SOURCE [%d] ~ TARGET %d\n", i, j);

        if (process_id == j)
            printf("SOURCE %d ~ TARGET [%d]\n", i, j);
#endif

        // Two left
        if (process_id == j)
            MPI_Send(&count, 1, MPI_LONG_LONG, i, 0, MPI_COMM_WORLD);

        if (process_id == i) 
        {
            MPI_Recv(&target_count, 1, MPI_LONG_LONG, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            return count + target_count;
        }

        return 0;

    } else {
        // More than two left
        int length = j - i + 1;

        // Head recursion
        long long left_count = binary_tree(i, i + length/2 - 1, count, process_id);
        long long right_count = binary_tree(i + length/2, j, count, process_id);

        int source_id = i + length/2;
        int target_id = i;

        if (process_id == source_id)
        {
#ifdef DEBUG
            printf("SOURCE [%d] ~ TARGET %d\n", source_id, target_id);
#endif

            MPI_Send(&right_count, 1, MPI_LONG_LONG, target_id, 0, MPI_COMM_WORLD);
        }

        if (process_id == target_id) 
        {
#ifdef DEBUG
            printf("SOURCE %d ~ TARGET [%d]\n", source_id, target_id);
#endif
            MPI_Recv(&target_count, 1, MPI_LONG_LONG, source_id, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            return left_count + target_count;
        }

        return 0;
    }

}

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

    // Send results
    long long global_count = binary_tree(0, num_processes - 1, count, process_id);

    // Estimate Pi and display the result
    if (process_id == 0)
    {
        pi = ((long double) global_count / (long double) NUM_ITER) * 4.0;
        t2 = MPI_Wtime(); 
        printf("RESULT=%f\tTIME=%f\tPROGRAM=22\tNPROC=%d\n", pi, t2 - t1, num_processes);
    }

    MPI_Finalize();
    return 0;
}

