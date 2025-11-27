#include "arbolavl.h"

// Definición de la Raíz global
NodoAVL* raizAVL = NULL;

void inicializarAVL() {
    raizAVL = NULL;
}

// --- Funciones Auxiliares AVL ---

// Retorna la altura de un nodo (0 si es NULL)
static int altura(NodoAVL* n) {
    return n ? n->altura : 0;
}

// Retorna el máximo de dos enteros
static int max(int a, int b) {
    return (a > b) ? a : b;
}

// Crea un nuevo nodo AVL, enlazándolo a un NodoCancion existente
static NodoAVL* crearNodoAVL(NodoCancion* cancion) {
    NodoAVL* nuevo = (NodoAVL*)malloc(sizeof(NodoAVL));
    if (nuevo == NULL) {
        fprintf(stderr, "Error: Falla en la asignacion de memoria para NodoAVL.\n");
        return NULL;
    }
    nuevo->datosCancion = cancion; // Almacenamos el puntero a la canción
    nuevo->izq = nuevo->der = NULL;
    nuevo->altura = 1;
    return nuevo;
}

// Obtiene el factor de balance de un nodo
static int getBalance(NodoAVL* n) {
    return n ? altura(n->izq) - altura(n->der) : 0;
}

// Encuentra el nodo con el valor mínimo (el mas a la izquierda)
static NodoAVL* minValorNodo(NodoAVL* nodo) {
    NodoAVL* current = nodo;
    // Bucle para encontrar la hoja más a la izquierda
    while (current->izq != NULL)
        current = current->izq;
    return current;
}
// --- Rotaciones ---

// Rotacion simple a la derecha [Image of AVL tree Right Rotation]
static NodoAVL* rotacionDerecha(NodoAVL* y) {
    NodoAVL* x = y->izq;
    NodoAVL* T2 = x->der;

    // Realizar rotación
    x->der = y;
    y->izq = T2;

    // Actualizar alturas
    y->altura = max(altura(y->izq), altura(y->der)) + 1;
    x->altura = max(altura(x->izq), altura(x->der)) + 1;

    return x;
}

// Rotación simple a la izquierda 
static NodoAVL* rotacionIzquierda(NodoAVL* x) {
    NodoAVL* y = x->der;
    NodoAVL* T2 = y->izq;

    // Realizar rotación
    y->izq = x;
    x->der = T2;

    // Actualizar alturas
    x->altura = max(altura(x->izq), altura(x->der)) + 1;
    y->altura = max(altura(y->izq), altura(y->der)) + 1;

    return y;
}

// --- Insercion AVL ---

// Inserta recursivamente un NodoCancion en el AVL y mantiene el balance
NodoAVL* insertarAVL(NodoAVL* raiz, NodoCancion* nuevaCancion) {
    // Inserción normal de BST (alcanzar una hoja)
    if (!raiz) return crearNodoAVL(nuevaCancion);

    // Usamos el título para la comparacion
    int cmp = strcmp(nuevaCancion->titulo, raiz->datosCancion->titulo);
    
    if (cmp < 0) {
        raiz->izq = insertarAVL(raiz->izq, nuevaCancion);
    }
    else if (cmp > 0) {
        raiz->der = insertarAVL(raiz->der, nuevaCancion);
    }
    else {
        return raiz; // No insertar duplicados
    }

    //Actualizar altura del nodo actual
    raiz->altura = 1 + max(altura(raiz->izq), altura(raiz->der));

    //Obtener factor de balance
    int balance = getBalance(raiz);

    //Casos de Desbalance (Rotaciones)
    
    // Caso Izquierda-Izquierda (LL)
    if (balance > 1 && cmp < 0)
        return rotacionDerecha(raiz);

    // Caso Derecha-Derecha (RR)
    if (balance < -1 && cmp > 0)
        return rotacionIzquierda(raiz);

    // Caso Izquierda-Derecha (LR)
    if (balance > 1 && cmp > 0) {
        raiz->izq = rotacionIzquierda(raiz->izq);
        return rotacionDerecha(raiz);
    }

    // Caso Derecha-Izquierda (RL)
    if (balance < -1 && cmp < 0) {
        raiz->der = rotacionDerecha(raiz->der);
        return rotacionIzquierda(raiz);
    }

    return raiz;
}

// --- Busqueda AVL ---

// Busca un nodo por el titulo de la cancion.Retorna el puntero a NodoCancion o NULL.
NodoCancion* buscarPorTituloAVL(NodoAVL* raiz, const char* titulo) {
    if (!raiz) return NULL;
    
    int cmp = strcmp(titulo, raiz->datosCancion->titulo);
    
    if (cmp == 0) {
        return raiz->datosCancion; //SI se encuentra el titulo devuelve los datos de la cancion
    } else if (cmp < 0) {
        return buscarPorTituloAVL(raiz->izq, titulo);
    } else {
        return buscarPorTituloAVL(raiz->der, titulo);
    }
}

// --- Liberar Memoria ---
void liberarAVL(NodoAVL* raiz) {
    if (raiz) {
        // NOTA: Solo liberamos el nodo AVL, los datos de la cancion se liberan con la lista enlazada (freeList)
        liberarAVL(raiz->izq);
        liberarAVL(raiz->der);
        free(raiz);
    }
}



// --- ELIMINACION POR TITULO ---
NodoAVL* EliminarNodoAvlTitulo(NodoAVL* raiz, const char* titulo) {
    // 1. REALIZAR EL BORRADO ESTANDAR DE BST
    if (raiz == NULL)
        return raiz;

    int cmp = strcmp(titulo, raiz->datosCancion->titulo);

    // Si el titulo es menor, buscar en la izquierda
    if (cmp < 0) {
        raiz->izq = EliminarNodoAvlTitulo(raiz->izq, titulo);
    }
    // Si el titulo es mayor, buscar en la derecha
    else if (cmp > 0) {
        raiz->der = EliminarNodoAvlTitulo(raiz->der, titulo);
    }
    // Si es igual, ESTE es el nodo a borrar
    else {
        // Caso 1: Nodo con solo un hijo o sin hijos
        if ((raiz->izq == NULL) || (raiz->der == NULL)) {
            NodoAVL* temp = raiz->izq ? raiz->izq : raiz->der;

            // Caso: Sin hijos
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else { 
                // Caso: Un hijo
                // Copiamos el contenido del hijo al nodo actual
                *raiz = *temp; 
            }
            free(temp); // Liberamos el nodo AVL (no la canción)
        } else {
            // Caso 2: Nodo con dos hijos
            // Obtener el sucesor inorder (el más pequeño del subárbol derecho)
            NodoAVL* temp = minValorNodo(raiz->der);

            // Copiar los datos del sucesor a este nodo
            raiz->datosCancion = temp->datosCancion;

            // Borrar el sucesor del subárbol derecho
            raiz->der = EliminarNodoAvlTitulo(raiz->der, temp->datosCancion->titulo);
        }
    }

    // Si el arbol tenía un solo nodo, retornar
    if (raiz == NULL)
        return raiz;

    // 2. ACTUALIZAR ALTURA DEL NODO ACTUAL
    raiz->altura = 1 + max(altura(raiz->izq), altura(raiz->der));

    // 3. OBTENER EL FACTOR DE BALANCE
    int balance = getBalance(raiz);

    // 4. BALANCEAR SI ES NECESARIO (4 Casos)

    // Caso Izquierda-Izquierda
    if (balance > 1 && getBalance(raiz->izq) >= 0)
        return rotacionDerecha(raiz);

    // Caso Izquierda-Derecha
    if (balance > 1 && getBalance(raiz->izq) < 0) {
        raiz->izq = rotacionIzquierda(raiz->izq);
        return rotacionDerecha(raiz);
    }

    // Caso Derecha-Derecha
    if (balance < -1 && getBalance(raiz->der) <= 0)
        return rotacionIzquierda(raiz);

    // Caso Derecha-Izquierda
    if (balance < -1 && getBalance(raiz->der) > 0) {
        raiz->der = rotacionDerecha(raiz->der);
        return rotacionIzquierda(raiz);
    }

    return raiz;
}