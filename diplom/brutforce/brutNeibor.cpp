#include <iostream>
#include <vector>
#include<fstream>
#include <map>   

using namespace std;

int main() 
{   
    int L = 4;
    int N = 2 * L * L;
    vector<vector<int> > neighbors=
    {
        {4,7,8,24,28,31}, //да
        {4,5,9,25,28,29}, //da
        {5,6,10,26,29,30}, //da
        {6,7,11,27,30,31}, //da
        {5,7,8,9},//da
        {6,9,10}, //da
        {7,10,11},//da
        {8,11}, //da
        {12,15,16}, //da
        {12,13,17},//da
        {13,14,18}, //da
        {14,15,19}, //da 11
        {15,16,17,13}, // da
        {14,17,18}, //da
        {15,18,19},  //da
        {16,19}, //da 15
        {20,23,24}, //da
        {20,21,25}, //da
        {21,22,26}, //da
        {22,23,27}, //da 19
        {23,21,25,24}, //da
        {22,25,26}, //da
        {23,27,26}, //da
        {27,24}, //da 23
        {31,28}, //da
        {29,28}, //da
        {30,29}, //da
        {31,30}, //da 27
        {29,31}, //net
        {30},
        {31},
        {}

    };

    long long spin[N];

    map<long long, map<long long, long long>> E_M_g;
    map<long long, long long> Energy;
    map<long long, long long> Mag;
    

    long long score = 1ULL << N;

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

    }

    ofstream out;
    out.open("fullresults.txt");
    if (out.is_open())
    {   
        long long G = 0;
        for(auto const& [en, M]: E_M_g)
        {
            for(auto const& [m, g]: M)
            {
                G += g;
                out << "Energy = " << en << ", M = " << m << ", g = " << g << "\n";
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
        long long G1 = 0;
        out << "Energy,g" << "\n";
        for(auto const& [en, g]: Energy)
        {
            out << en << "," << g << "\n";
        }

        out << "\n";
        out << "score = " << score << "\n";
        out << "max_g = " << G1 << "\n";
    }

    out.close();
    cout << "End\n";

    return 0;
}