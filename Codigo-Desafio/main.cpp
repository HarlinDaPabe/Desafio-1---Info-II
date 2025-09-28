#include <iostream>
#include <iomanip>
#include "Funciones.h"
#include "Funciones-Principales.h"

using namespace std;

int main() {
    unsigned char clave; static unsigned char** Encriptado; int Tamagno, rotaciones; char l; int y;
    cout << "Ingresa la Cantidad de Archivos que posees para Decodificar: ";
    cin >> y;
    l = 49;
    while (l <= y+48){
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ TEXTO " << l << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        rotaciones = 0;
        char archivo[16] = {'E', 'n', 'c', 'r', 'i', 'p', 't', 'a', 'd', 'o', l, '.', 't', 'x', 't'};
        rotaciones = ValoresArchivo(archivo, Tamagno);
        if (rotaciones == 1){
            break;
        }
        Encriptado = GenerarArreglo(archivo, Tamagno, clave);
        VerificarFoundClave(Encriptado, clave, rotaciones, Tamagno);
        AplicarDesencriptacion(Encriptado, Tamagno, rotaciones, clave);

        char Compresion[3][8] = {"RLE", "LZ78", "Nothing"}; char* x; int valor = 0, len_descomp, bytes; bool FindCompress = false;
        char pista[11] = {'p', 'i', 's', 't', 'a', l, '.', 't', 'x', 't'};

        while (!FindCompress && valor < 2){
            if (valor == 0){
                x = decompress_RLEl(Encriptado, Tamagno, len_descomp, bytes);
            } else {
                x = decompress_LZ78_bidimensional(Encriptado, Tamagno, len_descomp, bytes);
            }
            if (x != NULL){
                FindCompress = contiene_pista(x, len_descomp, pista);
            } else {
                delete[]x;
                x = NULL;
                valor++;
            }
        }

        for (int filas = 0; filas < Tamagno; filas++){
            delete[] Encriptado[filas];
        }
        delete[] Encriptado;
        Encriptado = NULL;

        if (valor != 2){
            cout << "Texto Original Descomprimido y Descencriptado:\n";
            for (int i = 0; i < len_descomp; i++){
                cout << x[i];
            }
            delete[]x;
            x = NULL;
            cout << endl;
        } else {
            cout << "~El texto no se pudo Descomprimir con los Metodos Brindados~\n";
        }

        cout << "\nEstimado Usuario, El texto (" << archivo << ") fue codigicado con clave: " << clave << " --> 0x"<< hex << uppercase << setw(2) << setfill('0') << (int)clave << "\n";
        cout << "Se Implemento una rotacion de (" << rotaciones << ") Bits" << endl;
        cout << "Metodo Descopresion: " << Compresion[valor] << endl;
        l++;
    }
    return 0;
}
