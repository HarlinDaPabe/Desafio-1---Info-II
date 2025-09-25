#include <iostream>
#include "Funciones.h"
#include "Funciones-Principales.h"

using namespace std;

int main() {
    unsigned char clave; char archivo[20] = "Encriptado4.txt"; unsigned char** Encriptado; int Tamagno, rotaciones = 0; bool Findkey;
    ValoresArchivo(archivo, Tamagno, clave);
    Encriptado = GenerarArreglo(archivo, Tamagno);
    VerificarFoundClave(Encriptado, clave, rotaciones, Tamagno);
    //AplicarDesencriptacion(Encriptado, Tamagno, rotaciones, clave);
    for (int i = 0; i < Tamagno; i++){
        for (int j = 0; j < 3; j++){
            cout << Encriptado[i][j];
        }
    }
    delete[] Encriptado;
    return 0;
}
