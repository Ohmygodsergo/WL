#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <omp.h> 

using namespace std;

#define N 3

int main() 
{   
    int maxEnergy = 6 * N * N + 20;
    int maxMag = 2 * N * N;
    int elementsE = 2 * maxEnergy + 1;
    int elementsM = maxMag + 1;

    vector<vector<long long>> Energy(elementsE, vector<long long>(elementsM, 0));
    long long score = 1LL << (2 * N * N);

    #pragma omp parallel
    {
        
        vector<vector<long long>> local_Energy(elementsE, vector<long long>(elementsM, 0));

        #pragma omp for
        for(long long mask = 0; mask < score; mask++)
        {   
            // 1. НАМАГНИЧЕННОСТЬ (Считается за 1 такт процессора!)
            int k = __builtin_popcountll(mask); 
            int M = abs(2 * k - 2 * N * N); 

            // 2. ЭНЕРГИЯ
            int E = 0;

            // Смещения битов: spin_h занимают биты 0..24, spin_v занимают 25..49
            const int v_offset = N * N; 

            for(int i = 0; i < N; i++) 
            {
                // Заранее считаем индексы соседей с учетом периодических границ (без %)
                int i_next = (i + 1 == N) ? 0 : i + 1;

                for(int j = 0; j < N; j++)
                {
                    int j_next = (j + 1 == N) ? 0 : j + 1;

                    // Линейные индексы текущего узла и его соседей справа/снизу
                    int idx_curr = i * N + j;
                    int idx_R    = i * N + j_next;
                    int idx_D    = i_next * N + j;

                    // Извлекаем биты (0 или 1)
                    int h_curr = (mask >> idx_curr) & 1;
                    int h_R    = (mask >> idx_R)    & 1;
                    int v_curr = (mask >> (v_offset + idx_curr)) & 1;
                    int v_D    = (mask >> (v_offset + idx_D))    & 1;

                    // Переводим биты {0, 1} в значения спинов {-1, +1}
                    int s_h_curr = h_curr * 2 - 1;
                    int s_h_R    = h_R    * 2 - 1;
                    int s_v_curr = v_curr * 2 - 1;
                    int s_v_D    = v_D    * 2 - 1;

                    // Считаем вклад в энергию от взаимодействия соседних спинов
                    E += (s_h_curr * s_h_R) + (s_v_curr * s_v_D) + (s_h_curr * s_v_curr) + (s_h_curr * s_v_D) + (s_h_R * s_v_curr) + (s_h_R * s_v_D);
                }
            }

            local_Energy[E + maxEnergy][M]++;
        }

        #pragma omp critical
        {
            for(int i = 0; i < elementsE; i++) {
                for(int j = 0; j < elementsM; j++) {
                    Energy[i][j] += local_Energy[i][j];
                }
            }
        }
    }

    long long G = 0;
    ofstream out;
    out.open("fullresults.txt");
    if (out.is_open())
    {
        for(int i = 0; i < elementsE; i++) 
        {
            for(int j = 0; j < elementsM; j++) 
            {
                long long g = Energy[i][j];
                if(g > 0) 
                {
                    G += g;
                    out << "E = " << i - maxEnergy << ", M = " << j << ", g = " << g << "\n"; 
                }
            }
        }
        out << "\nscore = " << score << "\nmax_g = " << G << "\n";
    }
    out.close();

    out.open("resultsE(g).txt");
    if (out.is_open())
    {
        for(int i = 0; i < elementsE; i++) 
        {   
            long long g = 0;
            for(int j = 0; j < elementsM; j++)
            {
                g += Energy[i][j];
            }

            if(g > 0) 
            {
                out << "E = " << i - maxEnergy << ", g = " << g << "\n"; 
            }
        }
        out << "\nscore = " << score << "\nmax_g = " << G << "\n";
    }
    out.close();

    cout << "end\n";
    return 0;
}