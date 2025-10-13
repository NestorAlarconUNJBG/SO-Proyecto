#include "../include/analizador.hpp"
#include "../include/ejecutor.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    string linea;
    system("clear");
    cout << "-----------------------------\n";
    cout << "MiniShell en C++ sobre Linux \n";
    cout << "-----------------------------\n";

    while (true) {
        cout << "MiniShell$ ";
        getline(cin, linea);

        if (linea.empty()) continue;

        vector<string> tokens = separar_comando(linea);
        ejecutar_comando(tokens);
    }

    return 0;
}
