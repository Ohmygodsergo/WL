#include <iostream>
#include <vector>
#include <map>  
#include<fstream> 

using namespace std;

int main() 
{
    int N = 18;
    vector<vector<int> > neighbors=
    {
        {3,  5, 6, 12, 15, 17},
        {3, 4, 7, 13, 15, 16},
        {4, 5, 8, 14, 16, 17},
        {6, 7, 4, 5}, 
        {5, 7, 8}, 
        {8, 6},
        {9,  11, 12},
        {9, 10, 13},
        {10, 11, 14},
        {10, 11, 12, 13}, 
        {11, 13, 14}, 
        {12, 14},
        {15, 17},
        {15, 16},
        {16, 17},
        {16, 17},
        {17},
        {}
    };

    int spin[N];

    map<int, map<int, long long>> E_M_g;
    map<int, long long> Energy;
    map<int, long long> Mag;
    

    long long score = 262144;// 2^18

    int a = 0;

    for (long long mask = 0; mask < score; mask++) 
    {

        for (int i = 0; i < N; i++) {
            
            if((mask >> i) & 1)            
            {
                spin[i] = 1;
            }
            else
            {
                spin[i] = -1;
            }
        }

        int E = 0;
        int M = 0;
        for(int i = 0; i < N; i++) 
        {
            for(int j = 0; j < neighbors[i].size(); j++) 
            {    
                E += spin[i] * spin[neighbors[i][j]];
            }
            M = M + spin[i];
        }

        E_M_g[E][M]++;
        Energy[E]++;
        Mag[M]++;

        if(E == -18 && a <= 3)
        {   
            cout << "Energy = -18" << endl;
            for(int i = 0; i < N; i++) 
            {   
                cout << spin[i] << " ";
                // cout << (spin[i] == 1 ? "+1 " : "-1 ");
                // if((i + 1) % 3 == 0)
                // {
                //     cout << endl;
                // }
            }   
            cout << endl;
            cout << endl;

            a++;
        }
    }

    ofstream out;
    out.open("resultsNeiborN3.txt");
    if (out.is_open())
    {   
        long long G = 0;
        out << "Energy,g" << "\n";
        for(auto const& [en, g]: Energy)
        {
            out << en << "," << g << "\n";
            G += g;
        }

        out << "\n";
        out << "score = " << score << "\n";
        out << "max_g = " << G << "\n";
    }
    
    out.close();

    return 0;
}