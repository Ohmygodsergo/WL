#!/bin/bash
#SBATCH --job-name=brutVecTN5
#SBATCH --partition=gpu
#SBATCH --gres=gpu:1
#SBATCH --nodes=1                 
#SBATCH --ntasks=1                
#SBATCH --cpus-per-task=64        
#SBATCH --time=72:00:00           
#SBATCH --output=nolic_%j.out     
#SBATCH --error=nolic_%j.err      

module load gcc

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

g++ -O3 -fopenmp brutVecTest.cpp -o brutVecTest

./brutVecTest