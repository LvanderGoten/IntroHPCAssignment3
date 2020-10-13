#! /bin/bash
#SBATCH -J assignment_3
#SBATCH -t 1:00:00
#SBATCH -A edu20.FDD3258
#SBATCH --nodes=2
#SBATCH --cpus-per-task=1
#SBATCH -e error_file.e

cc -o ping_pong ping_pong.c
srun -n 2 -N 1 ping_pong > ping_pong_single_node.dat
srun -n 2 -N 2 ping_pong > ping_pong_dual_node.dat
