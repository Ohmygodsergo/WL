#include<iostream>
#include<map>
#include<random>
#include<fstream>
using namespace std;
#define N 3

void h_start(int spin_h[N][N], int spin_v[N][N], mt19937 &gen, uniform_int_distribution<int> &distr)
{
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++)
        {
            // spin_h[i][j] = distr(gen) * 2 - 1;
            // spin_v[i][j] = distr(gen) * 2 - 1;
            spin_h[i][j] = 1;
            spin_v[i][j] = 1;
        }
    }
}

int start_e(int spin_h[N][N], int spin_v[N][N])
{
    int h_r, h_l, v_d, v_u;
    int e_sum = 0;

    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++)
        {
            h_r = spin_h[i][j];
            h_l = spin_h[i][(j - 1 + N) % N];
            v_d = spin_v[i][j];
            v_u = spin_v[(i - 1 + N) % N][j];
            e_sum += (h_l * h_r) + (v_u * v_d) + (h_l * v_u) + (h_l * v_d) + (h_r * v_u) + (h_r * v_d);
        }
    }
    return e_sum;
}

int vertex();

void vivod(int spin_h[N][N], int spin_v[N][N])
{
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++)
        {
            cout << spin_h[i][j] << " ";
        }
    } 
    cout << "\n";
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++)
        {
            cout << spin_v[i][j] << " ";
        }
    } 
    cout << "\n";
}




class WangLandau
{
    private:
    random_device rd;
    mt19937 gen;
    uniform_int_distribution<int> distr;
    
    int spin_h[N][N];
    int spin_v[N][N];



};

int main()
{   
    random_device rd;
    mt19937 gen(1984);
    uniform_int_distribution<int> distr(0, 1);

    int spin_h[N][N];
    int spin_v[N][N];

    h_start(spin_h, spin_v, gen, distr);
    vivod(spin_h, spin_v);
    int E = start_e(spin_h, spin_v);
    cout << E << endl;
    return 0;
}

int vertex()
{

}