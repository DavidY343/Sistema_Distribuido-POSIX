#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "claves.h"
#include <string.h>
#include "proxy.h"
#define NUM_THREADS 50

void *thread_function(void *arg) {
    int key = *((int *)arg);
    char value1[] = "example_value";
    int N_value2 = 3;
    double V_value2[] = {1.1, 2.2, 3.3};

    // Establecer un valor
    if (set_value_proxy(key, value1, N_value2, V_value2) == -1) {
        fprintf(stderr, "Error setting value for key %d.\n", key);
        pthread_exit(NULL);
    }

    // Obtener un valor
    char value1_get[50];
    int N_value2_get = 3;
    double V_value2_get[N_value2_get];

    if (get_value_proxy(key, value1_get, &N_value2_get, V_value2_get) == -1) {
        fprintf(stderr, "Error getting value for key %d.\n", key);
        pthread_exit(NULL);
    }

    // Modificar un valor
    double new_V_value2[] = {4.4, 5.5, 6.6};
    if (modify_value_proxy(key, value1, N_value2, new_V_value2) == -1) {
        fprintf(stderr, "Error modifying value for key %d.\n", key);
        pthread_exit(NULL);
    }

    // Verificar la existencia de una clave
    if (exist_proxy(key) == -1) {
        fprintf(stderr, "Error checking existence for key %d.\n", key);
        pthread_exit(NULL);
    }
    // Eliminar una clave
    if (delete_key_proxy(key) == -1) {
        fprintf(stderr, "Error deleting key %d.\n", key);
        pthread_exit(NULL);
    }

    // Verificar la existencia de una clave
    if (exist_proxy(key) == -1) {
        fprintf(stderr, "Error checking existence for key %d.\n", key);
        pthread_exit(NULL);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int keys[NUM_THREADS];

    // Crear hilos y asignar una clave única a cada hilo
    for (int i = 0; i < NUM_THREADS; i++) {
        keys[i] = i + 1;
        if (pthread_create(&threads[i], NULL, thread_function, (void *)&keys[i]) != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            exit(EXIT_FAILURE);
        }
        usleep(500000); // Espera 0.5 segundos entre la creación de cada hilo
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
