#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <iostream>
#include <fstream>

using namespace std;
unsigned char** GenerarArreglo (char* archiv_, int Tamagno){

    ifstream archivo(archiv_);  // Abrir en modo lectura
    if (!archivo) {
        cerr << "No se pudo abrir el archivo." << endl;
        return NULL;
    }
    char c;
    unsigned char** caracter = new unsigned char*[Tamagno];

    for (int l = 0; l < Tamagno; l++){
        caracter[l] = new unsigned char [3];
    }
    for (int i = 0; i < Tamagno; i++){
        for (int j = 0; j < 3; j++){
            archivo.get(c);
            caracter[i][j] = c;
        }
    }
    archivo.close();
    return caracter;
}

int ValoresArchivo(char* archiv_, int& TAM, unsigned char& clave){

    ifstream archivo(archiv_);  // Abrir en modo lectura
    if (!archivo) {
        cerr << "No se pudo abrir el archivo." << endl;
        return 1;
    }

    char letra; int cont = 0, c;
    while (archivo.get(letra)){
        if (cont < 2){
            if (cont == 0){
                clave = letra;
            } else {
                if (letra != clave){
                    if (((letra^clave) >= 16) ^ ((letra&clave) == clave)){
                        clave = 0;
                    }
                }
            }
        }
        cont++;
    }
    TAM = cont/3;
    archivo.close();
    return 0;
}

void DesencriptacionRot(unsigned char& Digito, int rotacion){
    int mascara = 255, resultado;
    resultado = Digito & mascara;
    resultado = resultado << (8-rotacion);
    Digito = (Digito >> rotacion)| resultado;
}

void AplicarDesencriptacion(unsigned char** ArregloEnc, int& Tamagno, int rotacion, unsigned char& clave){
    for (int i = 0; i < Tamagno; i++){
        for (int j = 0; j < 3; j++){
            ArregloEnc[i][j] = (ArregloEnc[i][j]^clave);
            DesencriptacionRot(ArregloEnc[i][j], rotacion);
            //cout << ArregloEnc[i][j];
        }
    }
}

void FounClave(unsigned char digito, unsigned char& clave, const int fila, int& cont, bool& Listo){
    unsigned char letra;
    Listo = false;
    //cout << Listo << digito << endl;
    while (!Listo){
        if (cont == 8 && fila == 0){
            clave++;
            cont = 0;
        }
        letra = digito ^ clave;
        //cout << (int)letra;
        DesencriptacionRot(letra, cont);
        if (!((letra < 65) || ((letra > 90) && (letra < 97)) || (letra > 122))){
            Listo = true;
            //cout << letra << cont;
            break;
        } else if (fila != 0){
            break;
        } else {
            cont++;
        }
    }
}

void VerificarFoundClave(unsigned char** ArregloEnc, unsigned char& clave, int& limitador, const int Tamagno){
    bool Good = false, Listo = false; int  i = 0, j = 2, c = 1;
    while (!Good){
        FounClave(ArregloEnc[i][j], clave, i, limitador, Listo);
        //cout << clave << limitador;
        if (!Listo){
            limitador = c;
            i = 0;
            c++;
        } else if (i == (Tamagno-1) && Listo){
            Good = true;
        } else {
            i++;
        }
        //cout << i << endl;
    }
}

#endif // FUNCIONES_H
