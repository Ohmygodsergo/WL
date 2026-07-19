#include<iostream>
#include<map>
#include<fstream>
#include<cmath>

using namespace std;
#define N 3

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
    int spin_h[N][N];
    int spin_v[N][N];

    map<long long, map<long long, long long>> E_M_g;
    map<long long, long long> Energy;
    map<long long, long long> Mag;

    long long score = pow(2, 2 * N * N); // 2^18 = 262144

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
        
        E_M_g[E][M]++;
        Energy[E]++;
        Mag[M]++;

        // if(E == -18 && a <= 3)
        // {
        //     for(int i = 0; i < N; i++)
        //     {
        //         for(int j = 0; j < N; j++)
        //         {
        //             cout << (spin_h[i][j] > 0 ? " +1" : " -1");
        //         }
        //         cout << "   ";
        //         for(int j = 0; j < N; j++)
        //         {
        //             cout << (spin_v[i][j] > 0 ? " +1" : " -1");
        //         }
        //         cout << endl;
        //     }
        //     cout << endl;
        //     cout << endl;
        //     a++;
        // }
    }

    ofstream out;
    out.open("resultsbrutN3.txt");
    if (out.is_open())
    {   
        long long G = 0;
        out << "Energy,M,g" << "\n";
        for(auto const& [en, M]: E_M_g)
        {
            for(auto const& [mag, g]: M)
            {
                out << en << "," << mag << "," << g << "\n";
                G += g;
            }
        }

        out << "\n";
        out << "score = " << score << "\n";
        out << "max_g = " << G << "\n";
    }
    
    out.close();
    return 0;
}