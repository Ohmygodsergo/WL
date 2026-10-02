#include <iostream>
#include <cstdint>

int getV(uint8_t state, int jy) 
{
    return (state >> jy) & 1; 
}

int getH(uint8_t state, int jy) 
{
    return (state >> (jy + 4)) & 1; 
}

int main() {
    uint8_t state = 45; // проверьте на этом числе
    for (int jy = 0; jy < 4; jy++) {
        std::cout << "V[" << jy << "] = " << getV(state, jy) << "\n";
    }
    for (int jy = 0; jy < 4; jy++) {
        std::cout << "H[" << jy << "] = " << getH(state, jy) << "\n";
    }
}