#! /bin/bash
#SBATCH -J assignment_3
#SBATCH -t 1:00:00
#SBATCH -A edu20.FDD3258
#SBATCH --nodes=128
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH -e error_file.e
module load cray-libsci
module load cray-mpich

NUM_REPS=10
NUM_PROCESSES="2\n4\n8\n16\n32\n64\n128"
SRC_PATH="/cfs/klemming/nobackup/l/lavdg/3"

# Compile .c files
find $SRC_PATH -name 'pi_seq_mpi_*.c' | xargs -I {} sh -c 'cc -o `basename {} .c` {}'

# Execute and vary the number of processes
for n in $(seq 1 $NUM_REPS)
do
    for ver in 21 22 23 24 25
    do
        echo -e $NUM_PROCESSES | xargs -I {} sh -c "srun -n {} `realpath pi_seq_mpi_$ver` >> elapsed_times.tsv"
    done
done

# Convert to CSV
cat elapsed_times.tsv | awk -F'[=\t]' -v OFS=',' '{print $1, $3, $5, $7, $9}' | head -n 1 > elapsed_times.csv
cat elapsed_times.tsv | awk -F'[=\t]' -v OFS=',' '{print $2, $4, $6, $8, $10}' >> elapsed_times.csv
