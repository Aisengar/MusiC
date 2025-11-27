#include "lista.h"
// En estas lineas de codigo declaramos la funcion para crear nuestro nodo. con la estrustura declarada en el archivo lusta.h
// Tambien se hace la creacion de la funcion donde se incertaran al final de nuetsra DLL(lista doble enlasada)
// La funcion que utilizamos para imprimir todas las canciones de nuestra PLay list en orden.
// yY muy importante la funcion para Limpiar la memoria 

// Definición real de las variables globales
NodoCancion* head = NULL;
NodoCancion* tail = NULL;

//Crear nodo
NodoCancion* crearNodo(const char* id, const char* titulo, const char* ruta, const char* genero, int calificacion) {
    NodoCancion* newNode = (NodoCancion*)malloc(sizeof(NodoCancion));
    if (newNode == NULL) { 
        fprintf(stderr, "Memory allocation failed!\n"); 
        return NULL; 
    }
    
    strncpy(newNode->id, id, sizeof(newNode->id) - 1);
    strncpy(newNode->titulo, titulo, sizeof(newNode->titulo) - 1);
    strncpy(newNode->ruta, ruta, sizeof(newNode->ruta) - 1);
    strncpy(newNode->genero, genero, sizeof(newNode->genero) - 1);
    newNode->calificacion = calificacion;
    
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

// insertar al final (tail)
void insertEnd(const char* id, const char* titulo, const char* ruta, const char* genero, int calificacion) {
    NodoCancion* newNode = crearNodo(id, titulo, ruta, genero, calificacion);
    
    if (newNode == NULL) return; 

    if (head == NULL) { 
        head = tail = newNode;
        
        //Apuntar a si mismo para cerrar el círculo inicial
        head->next = head;
        head->prev = head;
    } else {
        // Enlace estandar doble
        tail->next = newNode;
        newNode->prev = tail;
        
        //Actualizamos la cola
        tail = newNode;        
        
        //Cerrar el círculo  de nuestra lista circular doble mente enlazada
        tail->next = head; // El siguiente del ultimo es el primero
        head->prev = tail; // El previo del primero es el ultimo
    }
}

void displayForward() {
    if (head == NULL) {
        printf("La lista esta vacia.\n");
        return;
    }

    NodoCancion* curr = head;
    printf("\n--- Lista de Reproduccion (Circular) ---\n");
    
    // Usamos do-while para asegurar que entre al menos una vez
    do {
        printf("ID: %s | Titulo: %s | Genero: %s\n", curr->id, curr->titulo, curr->genero);
        curr = curr->next;
    } while (curr != head); // Se detiene cuando volvemos al inicio
    
    printf("--- Fin de la vuelta (regresando a Head) ---\n");
}

// Creamos la funcion para liberar memoria y que nuestro pc no explot :V
void freeList() {
    if (head == NULL) return;

    NodoCancion* curr = head;
    NodoCancion* next_node;

    // Recorremos hasta volver al inicio
    do {
        next_node = curr->next;
        free(curr);
        curr = next_node;
    } while (curr != head);

    head = tail = NULL; 
    printf("Memoria liberada correctamente.\n");
}

// ---ELIMINAR POR TÍTULO ---
void eliminarCancionPorTitulo(const char* tituloBorrar) {
    // 1. Verificar si la lista esta vacía
    if (head == NULL) {
        printf("La lista esta vacia, no se puede eliminar nada.\n");
        return;
    }

    NodoCancion* curr = head;
    int encontrado = 0;

    //Buscar la cancion recorriendo el circulo 
    do {
        // strcmp devuelve 0 si las cadenas son identicas
        if (strcmp(curr->titulo, tituloBorrar) == 0) {
            encontrado = 1;
            break; // Detenemos 'curr' en el nodo a borrar
        }
        curr = curr->next;
    } while (curr != head);

    // Si dimos toda la vuelta y no lo encontramos
    if (!encontrado) {
        printf("No se encontro la cancion: '%s'\n", tituloBorrar);
        return;
    }

    // Proceder a desconectar el nodo 

    // CASO A: Es el UNICO nodo en la lista
    if (curr == head && curr == tail) {
        head = NULL;
        tail = NULL;
    }
    // CASO B: Es la CABEZA (Head)
    else if (curr == head) {
        head = head->next;       // El head avanza uno
        head->prev = tail;       // El nuevo head mira atras al tail anterior
        tail->next = head;       // El tail se actualiza para mirar al nuevo head
    }
    // CASO C: Es la COLA (Tail)
    else if (curr == tail) {
        tail = tail->prev;       // El tail retrocede uno
        tail->next = head;       // El nuevo tail mira adelante al head original
        head->prev = tail;       // El head mira atrás al nuevo tail
    }
    // CASO D: Esta en MEDIO de otros dos nodos
    else {
        // El nodo ANTERIOR salta al SIGUIENTE
        curr->prev->next = curr->next;
        // El nodo SIGUIENTE salta al ANTERIOR
        curr->next->prev = curr->prev;
    }

    // 4. Liberar la memoria y confirmar
    printf("Cancion eliminada correctamente: %s\n", curr->titulo);
    free(curr);
}