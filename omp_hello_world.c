#include <stdio.h>
#include <omp.h>

int main() {
 #pragma omp parallel
     {
         int thread_id = omp_get_thread_num();
         int num_threads = omp_get_max_threads();
         printf("Hello World from rank %d from %d processes!\n", thread_id, num_threads);
     }
}
