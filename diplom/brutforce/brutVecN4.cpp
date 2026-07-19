#include<iostream>
#include<vector>
#include<cmath>
#include<fstream>
#include <omp.h> 
using namespace std;
#define N 4

void e(int spin_h[N][N], int spin_v[N][N], int &E, int &M)
{
    E = 0;
    M = 0;
    for(int i = 0; i < N; i++) 
    {
        for(int j = 0; j < N; j++)
        {
            int h_r = spin_h[i][j];
            int h_l = spin_h[i][(j - 1 + N) % N];
            int v_d = spin_v[i][j];
            int v_u = spin_v[(i - 1 + N) % N][j];

            E += (h_l * h_r) + (v_u * v_d) + (h_l * v_u) + (h_l * v_d) + (h_r * v_u) + (h_r * v_d);
            M += spin_h[i][j] + spin_v[i][j];
        }
    }
    M = abs(M);
}

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
        int spin_h[N][N];
        int spin_v[N][N];

        #pragma omp for
        for(long long mask = 0; mask < score; mask++)
        {       

            int bit = 0;
            for(int i = 0; i < N; i++)
        {
            for(int j = 0; j < N; j++)
            {
                if((mask >> bit) & 1)
                {
                    spin_h[i][j] = 1;
                }
                else
                {
                    spin_h[i][j] = -1;
                }
                bit++;
            }
        }

            for(int i = 0; i < N; i++)
        {
            for(int j = 0; j < N; j++)
            {
                if((mask >> bit) & 1)
                {
                    spin_v[i][j] = 1;
                }
                else
                {
                    spin_v[i][j] = -1;
                }
                bit++;
            }
        }

            int E = 0, M = 0;
            e(spin_h, spin_v, E, M);
            local_Energy[E + maxEnergy][M]++;
        }

        #pragma omp critical
        {
            for(int i = 0; i < elementsE; i++)
            {
                for(int j = 0; j < elementsM; j++)
                {
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
        out << "\n";
        out << "score = " << score << "\n";
        out << "max_g = " << G << "\n";
    }

    out.close();
    cout << "End\n";

    out.open("resultsE(g).txt");
    if (out.is_open())
    {
        out << "Energy,g" << "\n";
        for(int i = 0; i < elementsE; i++)
        {   
            long long g = 0;
            for(int j = 0; j < elementsM; j++)
            {
                g += Energy[i][j];
            }
            
            if(g > 0)
            {
                out << i - maxEnergy << "," << g << "\n"; 
            }
        }
        out << "\n";
        out << "score = " << score << "\n";
        out << "max_g = " << G << "\n";
    }

    out.close();
    

    return 0;
}