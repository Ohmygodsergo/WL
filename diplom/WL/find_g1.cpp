#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

struct Record
{
    int energy;
    double m;      // Добавили поле для M
    double ln_g;
};

int main()
{
    ifstream file("results.out");
    string line;
    vector<Record> data;

    // Пропускаем заголовок "Energy,M,ln_g"
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string energy_str, m_str, lng_str;

        // Читаем три колонки
        getline(ss, energy_str, ',');
        getline(ss, m_str, ',');
        getline(ss, lng_str);

        Record temp; 
        temp.energy = stoi(energy_str);
        temp.m = stod(m_str);      // Считываем M
        temp.ln_g = stod(lng_str);

        data.push_back(temp);
    }
    file.close();

    // Находим минимум ln_g для нормализации (как было у вас)
    double min_lng = data[0].ln_g;
    for (const auto& record : data) {
        if (record.ln_g < min_lng) min_lng = record.ln_g;
    }

    // Создаем файл с результатом
    ofstream outfile("results[E,g,M,ln_g].out");
    
    // Пишем заголовок (теперь 4 колонки)
    outfile << "Energy,g,M,ln(g)" << endl;

    for (const auto& record : data) {
        // Вычисляем g
        double g = exp(record.ln_g - min_lng) * 2.0;

        // Записываем всё: энергия, M, ln_g и вычисленное g
        outfile << record.energy << "," << g << "," << record.m << "," << record.ln_g << endl;
    }

    outfile.close();
    cout << "Файл успешно создан: results[E,g,M,ln_g].out" << endl;

    return 0;
}