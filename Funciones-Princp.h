#ifndef FUNCIONES_PRINCP_H
#define FUNCIONES_PRINCP_H

#include <iostream>

using namespace std;

int main()
{
    char* decompresor_LZ78(const char* comp, int comp_len, int& output_len, int& input_used) {
        // Inicializar variables
        output_len = 0;
        input_used = 0;

        // Capacidad inicial para el diccionario
        int dict_capacity = 256;
        int dict_size = 1; // Empezamos en 1 porque la entrada 0 es vacía

        // Crear diccionario
        char** dictionary = new char*[dict_capacity];
        int* dict_lengths = new int[dict_capacity];

        // Entrada 0: cadena vacía
        dictionary[0] = new char[1];
        dictionary[0][0] = '\0';
        dict_lengths[0] = 0;

        // Buffer para la salida (tamaño inicial)
        int output_capacity = comp_len * 2; // Estimación inicial
        char* output = new char[output_capacity];

        int i = 0; // Posición en la entrada comprimida
           }
    }
#endif // FUNCIONES_PRINCP_H
