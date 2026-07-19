#include<iostream>
#include <cmath>
#include<fstream>
#include<string>   
#include<sstream>
#include <vector>

using namespace std;

struct Record
{
    int energy;
    double ln_g;
};

int main()
{
    ifstream file("results.out"); // открываем файл
    string line;
    vector<Record> data;

    // Пропускаем заголовок (первую строку "Energy,ln_g")
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line); // Превращаем строку в поток
        string energy_str, lng_str;

        // Читаем до первой запятой — это энергия
        getline(ss, energy_str, ',');
        // Читаем то, что осталось после запятой — это ln_g
        getline(ss, lng_str);

        // 2. Превращаем текст в числа и заполняем структуру
        Record temp; 
        temp.energy = stoi(energy_str); // из строки в int
        temp.ln_g = stod(lng_str);      // из строки в double

        // 3. Добавляем структуру в вектор
        data.push_back(temp);

    }

    file.close();

    // 4. Давай проверим, что в векторе что-то есть
    cout << "Прочитано строк: " << data.size() << endl;
    if (!data.empty()) {
        cout << "Первая энергия: " << data[0].energy << endl;
    }

    double min_lng = data[0].ln_g; // Берем первое значение за эталон
    for (const auto& record : data) {
        if (record.ln_g < min_lng) {
            min_lng = record.ln_g;
        }
    }

    // 2. Создаем и открываем новый файл
    ofstream outfile("results[E,ln_g,g].out");

    // 3. Пишем заголовок (теперь три колонки)
    outfile << "Energy,ln_g,g" << endl;

    // 4. Записываем данные
    for (const auto& record : data) {
        // Вычисляем g относительно минимума
        // Вычитаем min_lng, чтобы не было переполнения (overflow)
        double g = exp(record.ln_g - min_lng) * 2.0;

        // Пишем в файл: энергия, оригинальный логарифм и наше g
        outfile << record.energy << "," << record.ln_g << "," << g << endl;
    }

    outfile.close();

    return 0;
}