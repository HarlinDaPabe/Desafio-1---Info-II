#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <iostream>
#include <fstream>

using namespace std;

unsigned char** GenerarArreglo (char* archiv_, int Tamagno, unsigned char& clave);
int ValoresArchivo(char* archiv_, int& TAM);
void AplicarDesencriptacion(unsigned char** ArregloEnc, int& Tamagno, int rotacion, unsigned char& clave);
void FounClave(unsigned char digito, unsigned char clave, int fila, int& cont, bool& Listo);
void VerificarFoundClave(unsigned char** ArregloEnc, unsigned char clave, int& limitador, int Tamagno);

unsigned char** GenerarArreglo (char* archiv_, int Tamagno, unsigned char& clave){
    ifstream archivo(archiv_, ios::binary);  // Abrir en modo lectura
    if (!archivo) {
        cerr << "No se pudo abrir el archivo." << endl;
        return NULL;
    }
    unsigned char c; int valor;
    unsigned char** caracter = new unsigned char*[Tamagno];

    for (int l = 0; l < Tamagno; l++){
        caracter[l] = new unsigned char [3];
    }
    for (int i = 0; i < Tamagno; i++){
        for (int j = 0; j < 3; j++){
            while ((valor = archivo.get())!= EOF) {
                //cout << (bool)((valor = archivo.get()) != EOF);
                c = static_cast<unsigned char>(valor);
                if (i == 0 && j == 0){
                    clave = c;
                }
                caracter[i][j] = c;
                break;
            }
        }
    }
    if (caracter[Tamagno-1][1] == caracter[Tamagno-1][2]){
        caracter[Tamagno-1][2] = '\0';
    }
    archivo.close();
    return caracter;
}

int ValoresArchivo(char* archiv_, int& TAM){

    ifstream archivo(archiv_, ios::binary);  // Abrir en modo lectura
    if (!archivo) {
        cerr << "No se pudo abrir el archivo." << endl;
        return 1;
    }

    int cont = 0; int c; unsigned char letra;
    while ((c = archivo.get()) != EOF){
        letra = static_cast<unsigned char>(c);
        cont++;
    }
    TAM = (cont+1)/3;
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
            if (ArregloEnc[i][j] != '\0'){
                ArregloEnc[i][j] = (ArregloEnc[i][j]^clave);
                DesencriptacionRot(ArregloEnc[i][j], rotacion);
                //cout << ArregloEnc[i][j];
            }
        }
    }
}

void FounClave(unsigned char digito, unsigned char clave, int fila, int& cont, bool& Listo){
    unsigned char letra;
    Listo = false;
    //cout << Listo << digito << endl;
    while (!Listo){
        letra = digito ^ clave;
        DesencriptacionRot(letra, cont);
        if (!((letra < 65) || ((letra > 90) && (letra < 97)) || (letra > 122))){
            Listo = true;
            //cout << letra << cont;
            break;
        } if (fila != 0){
            break;
        } else {
            cont++;
        }
    }
}

void VerificarFoundClave(unsigned char** ArregloEnc, unsigned char clave, int& limitador, int Tamagno){
    bool Good = false, Listo; int  i = 0, j = 2, c = 1;
    limitador = 0;
    while (!Good){
        if (ArregloEnc[i][j] != '\0'){
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
        } else {
            Good = true;
        }
    }
}

#endif // FUNCIONES_H
