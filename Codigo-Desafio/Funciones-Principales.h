#ifndef FUNCIONES_PRINCIPALES_H
#define FUNCIONES_PRINCIPALES_H

#include <iostream>
using namespace std;

char* decompress_LZ78(const char* comp, int comp_len, int& output_len, int& input_used) {
    // Inicializar variables
    output_len = 0;
    input_used = 0;

    // Capacidad inicial del diccionario
    int dict_capacity = 256;
    int dict_size = 1; // Empezamos con 1 porque la entrada 0 es vacía

    // Crear diccionario manualmente
    char** dictionary = new char*[dict_capacity];
    int* dict_lengths = new int[dict_capacity];

    // Inicializar entrada 0
    dictionary[0] = new char[1];
    dictionary[0][0] = '\0';
    dict_lengths[0] = 0;

    // Buffer para la salida
    int output_capacity = comp_len * 2;
    char* output = new char[output_capacity];

    int i = 0;

    while (i + 2 < comp_len) { // Necesitamos al menos 3 bytes
        // Leer el índice (2 bytes en big-endian)
        unsigned char high_byte = (unsigned char)comp[i];
        unsigned char low_byte = (unsigned char)comp[i + 1];
        int index = (high_byte << 8) | low_byte;

        // Leer el carácter
        char current_char = comp[i + 2];
        i += 3;

        // Verificar si el índice es válido
        if (index >= dict_size) {
            break; // Índice inválido
        }

        // Longitud de la cadena del diccionario
        int prefix_len = dict_lengths[index];
        int new_len = prefix_len + 1;

        // Crear la nueva cadena
        char* new_string = new char[new_len + 1];
        for (int j = 0; j < prefix_len; j++) {
            new_string[j] = dictionary[index][j];
        }
        new_string[prefix_len] = current_char;
        new_string[new_len] = '\0';

        // Agregar al diccionario si hay espacio
        if (dict_size >= dict_capacity) {
            // Redimensionar el diccionario
            int new_capacity = dict_capacity * 2;
            char** new_dict = new char*[new_capacity];
            int* new_lengths = new int[new_capacity];

            for (int j = 0; j < dict_size; j++) {
                new_dict[j] = dictionary[j];
                new_lengths[j] = dict_lengths[j];
            }

            delete[] dictionary;
            delete[] dict_lengths;

            dictionary = new_dict;
            dict_lengths = new_lengths;
            dict_capacity = new_capacity;
        }

        dictionary[dict_size] = new_string;
        dict_lengths[dict_size] = new_len;
        dict_size++;

        // Agregar la nueva cadena a la salida
        for (int j = 0; j < new_len; j++) {
            if (output_len >= output_capacity) {
                // Redimensionar el buffer de salida
                int new_output_capacity = output_capacity * 2;
                char* new_output = new char[new_output_capacity];

                for (int k = 0; k < output_len; k++) {
                    new_output[k] = output[k];
                }

                delete[] output;
                output = new_output;
                output_capacity = new_output_capacity;
            }

            output[output_len] = new_string[j];
            output_len++;
        }
    }

    input_used = i;

    // Liberar memoria del diccionario
    for (int j = 0; j < dict_size; j++) {
        delete[] dictionary[j];
    }
    delete[] dictionary;
    delete[] dict_lengths;

    // Crear una cadena final con terminador nulo
    char* final_output = new char[output_len + 1];
    for (int j = 0; j < output_len; j++) {
        final_output[j] = output[j];
    }
    final_output[output_len] = '\0';

    delete[] output;

    return final_output;
}

//void descompresor_rle(const char* comp, int comp_len, int& output_len, int& input_used){
    //Variables
    //output_len = 0;
    //input_used = 0;

    //int output_capacity = comp_len*10, count;
        //char* output = new char[output_capacity];
    //int i = 0;
    //while (i<comp_len){
        //count = 0;
        //while(i<comp_len && comp[i] >= "0" && comp[i] <="9"){
            //count= count *10 + (comp[i] - "0");
            //i++;
        //}
        //if(i>=comp_len){
            //break;
        //}
    //}
//}

bool contiene_pista(const char* texto, int longitud_texto, const char* pista, int longitud_pista) {
    // Verificar si la pista es más larga que el texto
    if (longitud_pista > longitud_texto) {
        return false;  // Si la pista es más larga, no puede estar contenida
    }
    //Calculamos hasta dónde podemos buscar en el texto
    int limite_busqueda = longitud_texto - longitud_pista;

    //Recorre el texto caracter por caracter
    for (int i = 0; i <= limite_busqueda; i++) {
        bool coincide = true;  // Línea 4: Asumimos que vamos a encontrar la pista

        //Compara cada caracter de la pista con el texto
        for (int j = 0; j < longitud_pista; j++) {
            //Si algún caracter no coincide, marcamos como falso
            if (texto[i + j] != pista[j]) {
                coincide = false;
                break;  //Sale del bucle interno para ahorrar tiempo
            }
        }

        //Se encuentra la pista completa, retornamos verdadero
        if (coincide) {
            return true;
        }
    }

    //Se recorre todo el texto y no se encuentra la pista, retornamos falso
    return false;
}

#endif // FUNCIONES_PRINCIPALES_H
