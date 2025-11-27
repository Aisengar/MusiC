#include "recomendador.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>

extern NodoCancion* head;

// Cargar canciones desde lista circular
void Recomendador::cargarCancionesDesdeLista() {
    canciones.clear();

    if (head == NULL) return;
	
	//Se hace un llamado al nodo cancion al igual que se hace al crear las otras estructuras de datos como
	//La lista (DLL) y el arvol AVL.
    NodoCancion* temp = head; 

    do {
        canciones.push_back(temp);
        temp = temp->next;
    } while (temp != head);

    // Crear matriz vacia NxN
    int N = canciones.size();
    grafo.assign(N, std::vector<int>(N, 0));

    std::cout << "\nCanciones cargadas al recomendador: " << N << "\n";
}

// Generar interacciones aleatorias
void Recomendador::generarGrafoInteracciones(int interacciones) {
    if (canciones.size() == 0) return;

    std::srand(std::time(NULL));
    int N = canciones.size();

    for (int i = 0; i < interacciones; i++) {
        int a = rand() % N;
        int b = rand() % N;
        if (a == b) continue;

        // Peso basado en la calificación del destino
        int peso = canciones[b]->calificacion;
        if (peso <= 0) peso = 1;

        grafo[a][b] += peso;
    }

    std::cout << "Interacciones generadas: " << interacciones << "\n";
}


// PageRank clasico
std::vector<double> Recomendador::ejecutarPageRank(double damping, int iter) {
    int N = canciones.size();
    std::vector<double> pr(N, 1.0 / N);
    std::vector<double> pr_new(N);

    for (int k = 0; k < iter; k++) {
        for (int i = 0; i < N; i++) {
            double suma = 0.0;

            for (int j = 0; j < N; j++) {
                int links = 0;
                for (int x = 0; x < N; x++) {
                    links += grafo[j][x];
                }

                if (links > 0 && grafo[j][i] > 0) {
                    suma += pr[j] * (grafo[j][i] / (double)links);
                }
            }

            pr_new[i] = (1 - damping) / N + damping * suma;
        }

        pr = pr_new;
    }

    std::cout << "PageRank ejecutado (" << iter << " iteraciones)\n";
    return pr;
}

// Mostrar top N canciones segun PR
void Recomendador::mostrarTop(int n, const std::vector<double>& pr) {
    int N = canciones.size();
    std::vector<std::pair<double, int>> ranking;

    for (int i = 0; i < N; i++) {
        ranking.push_back({pr[i], i});
    }

    std::sort(ranking.begin(), ranking.end(),
              [](auto& a, auto& b) { return a.first > b.first; });

    std::cout << "\n=== TOP " << n << "CANCIONES RECOMENDADAS ===\n";

    for (int i = 0; i < n && i < N; i++) {
        int idx = ranking[i].second;
        NodoCancion* c = canciones[idx];

        std::cout << i+1 << ". "
                  << c->titulo << "  | Genero: " << c->genero
                  << " | Calificacion: " << c->calificacion
                  << " | Score: " << ranking[i].first
                  << "\n";
    }
}