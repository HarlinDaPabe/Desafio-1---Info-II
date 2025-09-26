#include <iostream>
#include "Funciones.h"
#include "Funciones-Principales.h"
#include <iomanip>

using namespace std;

int main() {
    unsigned char clave; unsigned char** Encriptado; int Tamagno, rotaciones; char l; int y;
    cout << "Ingresa la Cantidad de Archivos que posees para Decodificar: ";
    cin >> y;
    l = 49;
    while (l <= y+48){
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ TEXTO " << l << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        rotaciones = 0;
        char archivo[16] = {'E', 'n', 'c', 'r', 'i', 'p', 't', 'a', 'd', 'o', l, '.', 't', 'x', 't'};
        char pista[11] = {'p', 'i', 's', 't', 'a', l, '.', 't', 'x', 't'};
        rotaciones = ValoresArchivo(archivo, Tamagno, clave);
        if (rotaciones == 1){
            break;
        }
        Encriptado = GenerarArreglo(archivo, Tamagno);
        VerificarFoundClave(Encriptado, clave, rotaciones, Tamagno);
        AplicarDesencriptacion(Encriptado, Tamagno, rotaciones, clave);

        char Compresion[3][8] = {"RLE", "LZ78", "Nothing"}; char* x; int valor = 0, len_descomp, bytes; bool FindCompress = false;
        //while (!FindCompress && valor < 2){
            //if (valor == 0){
                //x = decompress_LZ78(Encriptado, Tamagno, len_descomp, bytes);
            //} else {
                //x = decompress_RLE(Encriptado, Tamagno, len_descomp, bytes);
            //}
            //valor++;
        //}
        if (valor != 2){
            cout << "Texto Original Descomprimido y Descencriptado:\n";
            for (int i = 0; i < Tamagno; i++){
                for (int j = 0; j < 3; j++){
                    if (Encriptado[i][j] < 97 || Encriptado[i][j] > 122){
                        //cout << (int)Encriptado[i][j];
                    } else {
                        //cout << Encriptado[i][j];
                    }
                }
            }
            cout << endl;
        } else {
            cout << "~El texto no se pudo Descomprimir con los Metodos Brindados~\n";
        }
        cout << "\nEstimado Usuario, El texto (" << archivo << ") fue codigicado con clave: " << clave << " --> 0x"<< hex << uppercase << setw(2) << setfill('0') << (int)clave << "\n";
        cout << "Se Implemento una rotacion de (" << rotaciones << ") Bits" << endl;
        //cout << "Metodo Descopresion: " << Compresion[valor] << endl << endl;
        delete[] Encriptado;
        l++;
    }
    return 0;
}
