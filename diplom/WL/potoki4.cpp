#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <random>
#include <map>
using namespace std;
#define N 3

class WangLandau
{
private:
    int spin_h[N][N], spin_v[N][N];
    int e;
    int M = 0;
    
    int min_e = 10000;
    int best_spin_h[N][N], best_spin_v[N][N];

    map<int, long long> H;
    map<int, long long> H_total;
    map<int, double> ln_g;
    map<int, double> sum_abs_M;
    double f = 1.0;
    //double f = exp(1.0);

    mt19937 gen;
    uniform_real_distribution<double> dist_real;
    uniform_int_distribution<int> dist_coord;
    uniform_int_distribution<int> dist_coin;
    
    void hot_start(){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            spin_h[i][j] = dist_coin(gen) * 2 - 1;
            spin_v[i][j] = dist_coin(gen) * 2 - 1;
        }
    }
}

    int start_e()
{   
    M = 0;
    int e_sum = 0;
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            e_sum += get_vertex_e(i, j);
            M += spin_h[i][j];
            M += spin_v[i][j];
            // cout << "M: " << M << endl;
        }
    }

    return e_sum;
}

    int get_vertex_e(int i, int j)
{
    int h_r = spin_h[i][j];
    int h_l = spin_h[i][(j - 1 + N) % N];
    int v_d = spin_v[i][j];
    int v_u = spin_v[(i - 1 + N) % N][j];

    int e_sum = (h_l * h_r) + (v_u * v_d) + (h_l * v_u) + (h_l * v_d) + (h_r * v_u) + (h_r * v_d);
    //J = -1;
    return e_sum;

}


public:
    double getf()
    {
        return f;
    }

    void setf(double v)
    {
        f = v; 
    }

    WangLandau():
        gen(1337), 
        dist_real(0.0, 1.0), 
        dist_coord(0, N - 1), 
        dist_coin(0, 1) 
    {
        hot_start();
        e = start_e();
        cout << "start_M: " << M << endl;
    }

    void step()
{
    int i = dist_coord(gen);
    int j = dist_coord(gen);

    int dE = 0;

    if(dist_coin(gen) == 1)
    {
        int e_old = get_vertex_e(i, j) + get_vertex_e(i, (j + 1) % N);
        spin_h[i][j] *= -1;
        int e_new = get_vertex_e(i, j) + get_vertex_e(i, (j + 1) % N);
        dE = e_new - e_old;

        M += 2 * spin_h[i][j];

        int e_p = e + dE;
        double P = exp(ln_g[e] - ln_g[e_p]);
        if (dist_real(gen) < P) {   
            e = e_p;
        }else {
            M -= 2 * spin_h[i][j];
            spin_h[i][j] *= -1;
        }
    }
    else
    {
        int e_old = get_vertex_e(i, j) + get_vertex_e((i + 1) % N, j);
        spin_v[i][j] *= -1;
        int e_new = get_vertex_e(i, j) + get_vertex_e((i + 1) % N, j);
        dE = e_new - e_old;

        M += 2 * spin_v[i][j];

        int e_p = e + dE;
        double P = exp(ln_g[e] - ln_g[e_p]);
        if (dist_real(gen) < P) {   
            e = e_p;
        }else {
            M -= 2 * spin_v[i][j];
            spin_v[i][j] *= -1;
        }
    }

    if (e < min_e) {
    min_e = e;
    for(int ni=0; ni<N; ni++){
        for(int nj=0; nj<N; nj++){
            best_spin_h[ni][nj] = spin_h[ni][nj];
            best_spin_v[ni][nj] = spin_v[ni][nj];
            }
        }
    }
    
    ln_g[e] += f;
    //ln_g[e] += log(f);
    H[e] += 1;
    H_total[e] += 1;
    sum_abs_M[e] += abs(M);
}

    bool is_flat() 
{
    if (H.empty()) return false;

    double sum = 0;
    for (auto const& [energy, count] : H) sum += count;

    double avg = sum / H.size();
    if (avg < 100.0) return false; 

    for (auto const& [energy, count] : H) {
        if (count < avg * 0.85) return false;
    }
    
    return true; 
}

    void next_level()
{
    for(auto& [energy, count] : H) count = 0;
    // for(auto& [energy, sM] : sum_abs_M) sM = 0;
    f = f / 2.0;
    // f = sqrt(f)
}

    void save_to_file(string filename) 
{
    ofstream out(filename);
    if (out.is_open()) {
        out << "Energy,ln_g" << endl;
        for (auto const& [energy, weight] : ln_g) {
            out << energy << "," << weight << endl;
        }
        out.close();
    }
}

    void save_to_file1(string filename) 
{
    ofstream out(filename);
    if (out.is_open()) {
        out << "Energy,M,ln_g" << endl; // Уточнили заголовок
        
        for (auto const& [energy, weight] : ln_g) {
            
            double avg_m = 0.0;
            // Проверяем, посещалось ли это состояние, чтобы не делить на 0
            if (H_total.count(energy) && H_total.at(energy) > 0) {
                avg_m = (double)sum_abs_M.at(energy) / H_total.at(energy);
            }

            // Записываем Energy, ln_g и вычисленное среднее M
            out << energy << "," << avg_m << "," << weight << endl;
        }
        out.close();
    }
}

void Emin_print() {
    cout << "\n-------------------------------------------------" << endl;
    cout << "E_min = " << min_e << endl;
    
    for (int i = 0; i < N; i++) {
        // Каждая строка решетки (i) рендерится в 3 строки консоли

        // 1. Верхние спины (это вертикальные спины из предыдущего ряда v[(i-1+N)%N][j])
        for (int j = 0; j < N; j++) {
            int v_u = best_spin_v[(i - 1 + N) % N][j];
            cout << "      " << (v_u == 1 ? "+" : "-") << "      ";
        }
        cout << endl;

        // 2. Левый спин -> Узел (o) -> Правый спин
        for (int j = 0; j < N; j++) {
            int h_l = best_spin_h[i][(j - 1 + N) % N];
            int h_r = best_spin_h[i][j];
            
            cout << "  " << (h_l == 1 ? "+" : "-") 
                 << "   o   " << (h_r == 1 ? "+" : "-") << "  ";
        }
        cout << endl;

        // 3. Нижние спины (это вертикальные спины текущего ряда v[i][j])
        for (int j = 0; j < N; j++) {
            int v_d = best_spin_v[i][j];
            cout << "      " << (v_d == 1 ? "+" : "-") << "      ";
        }
        cout << endl << endl; // Отступ между рядами узлов
    }
    cout << "-------------------------------------------------" << endl;
}

};


int main()
{
    WangLandau sim;

    while (sim.getf() > 1e-8) 
    {
        for (int k = 0; k < 10000; k++) 
        {
            sim.step();
        }

        if (sim.is_flat()) 
        {
            sim.next_level();
            cout << "f = " << sim.getf() << endl;
        }
    }

    sim.Emin_print();
    sim.save_to_file1("results.out");
    
    return 0;
}