#include "../include/ejecutor.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <cstdlib> 

using namespace std;

// Verifica si el comando existe y es ejecutable
bool verificar_comando(const string &ruta) {
    if (access(ruta.c_str(), F_OK) == -1) {
        perror("Error al ejecutar el comando ");
        return false;
    }
    if (access(ruta.c_str(), X_OK) == -1) {
        perror("Error de permiso denegado para ejecutar el archivo");
        return false;
    }
    return true;
}

void ejecutar_comando(vector<string> &tokens) {
    if (tokens.empty()) return;

    // Caso especial: comando para salir de la mini-shell
    if (tokens[0] == "salir") {
        cout << "Saliendo de la miniShell..." << endl;
        exit(0);
    }

    string ruta_comando = tokens[0];
    if (ruta_comando[0] != '/') {
        ruta_comando = "/bin/" + ruta_comando;
    }

    if (!verificar_comando(ruta_comando)) {
        return;
    }

    // Busca si el comando incluye una redirección de salida (>)
    bool hay_redireccion = false;
    string archivo_salida;
    vector<char*> args;

    for (size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i] == ">") {
            hay_redireccion = true;
            if (i + 1 < tokens.size()) {
                archivo_salida = tokens[i + 1];
            } else {
                cerr << "Error: falta el nombre del archivo después de '>'" << endl;
                return;
            }
            break;
        } else {
            args.push_back(const_cast<char*>(tokens[i].c_str()));
        }
    }

    args.push_back(nullptr);

    // Crea un nuevo proceso para ejecutar el comando
    pid_t pid = fork();

    if (pid < 0) {
        perror("Error al crear el proceso hijo\n");
        return;
    }

    if (pid == 0) {
        if (hay_redireccion) {
            int desc_archivo = open(archivo_salida.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (desc_archivo < 0) {
                perror("Error al abrir el archivo para redirección\n");
                exit(1);
            }
            dup2(desc_archivo, STDOUT_FILENO);
            close(desc_archivo);
        }

        if (execvp(args[0], args.data()) == -1) {
            cerr << "Error al ejecutar el comando: " << args[0] << endl;
            exit(1);
        }

    } else {
        // Proceso padre: espera a que el hijo termine
        waitpid(pid, nullptr, 0);
    }
}