#ifndef FUNCIONES_PRINCIPALES_H
#define FUNCIONES_PRINCIPALES_H

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

void DesencriptacionRot(char& Digito){
    int mascara = 7, resultado;
    resultado = Digito & mascara;
        resultado = resultado << 5;
        Digito = (Digito >> 3)| resultado;
}

void DesencriptacionXor(char** ArregloEnc, int& Tamagno, char& clave){
    for (int i = 0; i < Tamagno; i++){
        for (int j = 0; j < 2; j++){
            ArregloEnc[i][j] = (ArregloEnc[i][j]^clave);
            DesencriptacionRot(ArregloEnc[i][j]);
        }
    }
}

#endif // FUNCIONES_PRINCIPALES_H
