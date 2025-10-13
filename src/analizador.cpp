#include "../include/analizador.hpp"
#include <sstream>
using namespace std;

vector<string> separar_comando(const string &linea) {
    vector<string> tokens;
    string token;
    istringstream stream(linea);

    while (stream >> token) {
        tokens.push_back(token);
    }
    return tokens;
}
