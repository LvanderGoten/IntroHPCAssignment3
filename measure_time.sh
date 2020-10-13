#! /bin/bash
NUM_REPS=10
NUM_PROCESSES="2\n4\n8\n16\n32"

# Compile .c files
ls -1 | grep -E "pi_seq_mpi_.*.c" | xargs -I {} sh -c ' mpicc -o `basename {} .c` {}'

# Execute and vary the number of processes
for n in $(seq 1 $NUM_REPS)
do
    for ver in 21 22 23 24 25
    do
        echo -e $NUM_PROCESSES | xargs -I {} sh -c "mpirun -np {} `realpath pi_seq_mpi_$ver` >> elapsed_times.tsv"
    done
done

# Convert to CSV
cat elapsed_times.tsv | awk -F'[=\t]' -v OFS=',' '{print $1, $3, $5, $7}' | head -n 1 > elapsed_times.csv
cat elapsed_times.tsv | awk -F'[=\t]' -v OFS=',' '{print $2, $4, $6, $8}' >> elapsed_times.csv



