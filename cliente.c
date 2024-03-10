#include <stdio.h>
#include "claves.h"
#include <mqueue.h>
#include "proxy.h"
#include <stdlib.h>
#include <string.h>

int main() {
    // Se inicializa el proxy
    if (init_proxy() == -1) {
        fprintf(stderr, "Error initializing proxy.\n");
        exit(EXIT_FAILURE);
    }

    // Ejemplo de uso de las funciones del proxy
    int key = 123;
    char value1[] = "example_value";
    int N_value2 = 3;
    double V_value2[] = {1.1, 2.2, 3.3};

    // Establecer un valor
    if (set_value_proxy(key, value1, N_value2, V_value2) == -1) {
        fprintf(stderr, "Error setting value.\n");
        exit(EXIT_FAILURE);
    }

    // Obtener un valor
    char value1_get[50];
    int N_value2_get = 3;
    double V_value2_get[N_value2_get];

    if (get_value_proxy(key, value1_get, &N_value2_get, V_value2_get) == -1) {
        fprintf(stderr, "Error getting value.\n");
        exit(EXIT_FAILURE);
    }

    // Modificar un valor
    double new_V_value2[] = {4.4, 5.5, 6.6};
    if (modify_value_proxy(key, value1, N_value2, new_V_value2) == -1) {
        fprintf(stderr, "Error modifying value.\n");
        exit(EXIT_FAILURE);
    }

    // Eliminar una clave
    if (delete_key_proxy(key) == -1) {
        fprintf(stderr, "Error deleting key.\n");
        exit(EXIT_FAILURE);
    }

    // Verificar la existencia de una clave
    if (exist_proxy(key) == -1) {
        fprintf(stderr, "Error checking existence.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
