#!/bin/bash
#SBATCH --job-name=brutN4
#SBATCH --partition=gpu
#SBATCH --gres=gpu:1
#SBATCH --cpus-per-task=1
#SBATCH --time=01:00:00
#SBATCH --output=brutN4_%j.out
#SBATCH --error=brutN4_%j.err

module load gcc

g++ -O3 potoki.cpp -o potoki

./potoki
