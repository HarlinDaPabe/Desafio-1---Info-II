#ifndef FUNCIONES_PRINCIPALES_H
#define FUNCIONES_PRINCIPALES_H

#include <iostream>
#include "Funciones.h"
#include <fstream>

using namespace std;

char* decompress_LZ78_bidimensional(unsigned char** comp, int filas, int& output_len, int& input_used);
char* decompress_RLEl(unsigned char** comp, int filas, int& output_len, int& input_used);
bool contiene_pista(const char* texto, int longitud_texto, const char* pista, int longitud_pista);

char* decompress_LZ78_bidimensional(unsigned char** comp, int filas, int& output_len, int& input_used){
    // Inicializar variables de salida
    output_len = 0;
    input_used = 0;
    if (comp[0][1] > 0 || comp[0][0] > 0){
        return NULL;
    }
    // Capacidad inicial del diccionario
    int dict_capacity = filas;
    int dict_size = 1; // Empezamos con 1 porque la entrada 0 es vacía

    // Crear diccionario: arreglo de punteros a char (cadenas)
    char** dictionary = new char*[dict_capacity];
    // Arreglo para guardar las longitudes de cada cadena en el diccionario
    int* dict_lengths = new int[dict_capacity];

    // Inicializar la entrada 0 del diccionario (cadena vacía)
    dictionary[0] = new char[1];
    dictionary[0][0] = '\0';
    dict_lengths[0] = 0;

    // Buffer para la salida (tamaño inicial estimado)
    int output_capacity = filas * 3; // Estimación: cada fila puede expandirse a varias letras
    char* output = new char[output_capacity];

    // Procesar cada fila del arreglo bidimensional
    for (int fila = 0; fila < filas; fila++) {
        // Verificar que la fila actual no sea nula
        if (comp[fila] == nullptr) {
            cout << "Fila " << fila << " es nula. Saltando." << endl;
            continue; // Saltar esta fila y continuar con la siguiente
        } else {

            // Cada fila tiene 3 columnas: [byte_alto, byte_bajo, carácter]
            // Leer los dos primeros bytes como el índice (big-endian)
            unsigned char high_byte = comp[fila][0];
            unsigned char low_byte = comp[fila][1];
            int index = (high_byte << 8) | low_byte;

            // Leer el tercer byte como el carácter a agregar
            char current_char = comp[fila][2];

            // Validar que el índice esté dentro del rango del diccionario actual
            if (index > dict_size){
                cout << "Índice inválido: " << index << " en fila " << fila << endl;
                break; // Salir del bucle si el índice no es válido
            }

            // Obtener la longitud de la cadena del diccionario en la posición 'index'
            int prefix_len = dict_lengths[index];
            // La nueva cadena tendrá la longitud del prefijo más uno
            int new_len = prefix_len + 1;

            // Crear la nueva cadena (prefijo + nuevo carácter)
            char* new_string = new char[new_len + 1]; // +1 para el carácter nulo

            // Copiar el prefijo desde el diccionario
            for (int j = 0; j < prefix_len; j++) {
                new_string[j] = dictionary[index][j];
            }

            // Añadir el nuevo carácter al final
            new_string[prefix_len] = current_char;
            new_string[new_len] = '\0'; // Añadir el terminador nulo

            // Verificar si necesitamos redimensionar el diccionario
            if (dict_size >= dict_capacity){
                // Duplicar la capacidad
                int new_capacity = dict_capacity * 2;
                char** new_dict = new char*[new_capacity];
                int* new_lengths = new int[new_capacity];

                // Copiar las entradas existentes
                for (int j = 0; j < dict_size; j++){
                    new_dict[j] = dictionary[j];
                    new_lengths[j] = dict_lengths[j];
                }

                // Liberar los arreglos antiguos
                delete[] dictionary;
                delete[] dict_lengths;

                // Actualizar los punteros y la capacidad
                dictionary = new_dict;
                dict_lengths = new_lengths;
                dict_capacity = new_capacity;
            }

            // Añadir la nueva cadena al diccionario
            dictionary[dict_size] = new_string;
            dict_lengths[dict_size] = new_len;
            dict_size++;

            // Añadir la nueva cadena al buffer de salida
            for (int j = 0; j < new_len; j++){
                // Verificar si necesitamos redimensionar el buffer de salida
                if (output_len >= output_capacity){
                    int new_output_capacity = output_capacity * 2;
                    char* new_output = new char[new_output_capacity];

                    // Copiar el contenido actual al nuevo buffer
                    for (int k = 0; k < output_len; k++) {
                        new_output[k] = output[k];
                    }

                    // Liberar el buffer antiguo y actualizar
                    delete[] output;
                    output = new_output;
                    output_capacity = new_output_capacity;
                }

                // Añadir el carácter actual al buffer de salida
                output[output_len] = new_string[j];
                output_len++;
            }
        }
    }

    // Actualizar la cantidad de bytes de entrada utilizados (3 bytes por fila)
    input_used += 3;

    // Crear un array de caracteres del tamaño exacto para el resultado final
    char* final_output = new char[output_len + 1];
    for (int j = 0; j < output_len; j++) {
        final_output[j] =  output[j];
    }
    final_output[output_len] = '\0';

    // Liberar la memoria del buffer de salida temporal
    delete[] output;

    // Liberar la memoria del diccionario
    for (int j = 0; j < dict_size; j++) {
        delete[] dictionary[j];
    }
    delete[] dictionary;
    delete[] dict_lengths;

    return final_output;
}

char* decompress_RLEl(unsigned char** comp, int filas, int& output_len, int& input_used) {
    // INICIALIZACIÓN
    output_len = 0;
    input_used = 0;

    if (comp[output_len][1] == 0){
        return NULL;
    }
    // Verificar que el arreglo sea válido
    if (comp == nullptr || filas <= 0) {
        return nullptr;
    }

    // ARREGLO BIDIMENSIONAL A LINEAL
    // Primero: calcular el tamaño total linealizando las filas
    int total_bytes = filas*3; // 3 columnas por fila

    // Crear un buffer temporal lineal
    unsigned char* comp_lineal = new unsigned char[total_bytes]; // +1 para el '\0'
    int index_lineal = 0;

    // Copiar todos los bytes del bidimensional al lineal
    for (int fila = 0; fila < filas; fila++) {
        if (comp[fila] == nullptr) {
            break; // Fila nula, terminar
        }

        for (int col = 0; col < 3; col++) {
            comp_lineal[index_lineal++] = comp[fila][col];
        }
    }

    //APLICAR RLE SOBRE EL ARREGLO LINEAL
    int capacidad_salida = total_bytes; // Estimación conservadora
    char* salida = new char[capacidad_salida];

    int i = 0; // Índice para recorrer el arreglo lineal

    //BUCLE PRINCIPAL RLE
    //cout << index_lineal;
    while (i < index_lineal) {
        // Leer todos los dígitos consecutivos para formar el número
        int count = 0;
        for (int k = 0; k < 2; k++) {
            count = (int)comp_lineal[i];
            i++;
        }

        // Verificar si llegamos al final
        if (i >= index_lineal) {
            break;
        }

        // Leer el carácter a repetir
        char caracter_actual = comp_lineal[i];
        i++;

        // Validar que el count sea válido
        if (count <= 0) {
            continue;
        }

        // Repetir el carácter count veces
        for (int j = 0; j < count; j++) {
            // Redimensionar si es necesario
            if (output_len >= capacidad_salida) {
                int nueva_capacidad = capacidad_salida + 150;
                char* nueva_salida = new char[nueva_capacidad];

                for (int k = 0; k < output_len; k++) {
                    nueva_salida[k] = salida[k];
                }

                delete[] salida;
                salida = nueva_salida;
                capacidad_salida = nueva_capacidad;
            }

            salida[output_len] = caracter_actual;
            //cout << caracter_actual;
            output_len++;
        }
    }

    input_used = i; // Bytes procesados del arreglo lineal

    //  CREAR RESULTADO FINAL
    char* resultado_final = new char[output_len + 1];
    for (int j = 0; j < output_len; j++) {
        resultado_final[j] = salida[j];
    }
    resultado_final[output_len] = '\0';

    delete[] comp_lineal;
    delete[] salida;

    return resultado_final;
}

bool contiene_pista(char* texto, int& longitud_texto, char* pista){
    int longitud_pista, auxiliar;

    ValoresArchivo(pista, auxiliar);
    longitud_pista = auxiliar*3;
    ifstream archivo(pista, ios :: binary);
    char letra; unsigned char pista_text[longitud_pista]; int c;
    for (int i = 0; i < longitud_texto; i++){
        while ((c = archivo.get())!= EOF) {
            letra = static_cast<unsigned char>(c);
            pista_text[i] = letra;
            break;
        }
    }
    archivo.close();

    // Verificar si la pista es más larga que el texto
    if (longitud_pista > longitud_texto) {
        return false;  // Si la pista es más larga, no puede estar contenida
    }

    //Recorre el texto caracter por caracter
    int cont = 0;
    for (int i = 0; i < longitud_texto; i++) {
        if (texto[i] == pista_text[cont]){
            cont++;
        } else if (cont != 0 && (texto[i] != pista_text[0])){
            cont--;
        }
        if (cont == longitud_pista){
            return true;
        }
    }
    return false;
}

#endif // FUNCIONES_PRINCIPALES_H
