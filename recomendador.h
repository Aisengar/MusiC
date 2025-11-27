#ifndef RECOMENDADOR_H
#define RECOMENDADOR_H

#include <vector>
#include <string>
#include "lista.h"   // Para usar NodoCancion

class Recomendador {
public:

    // Carga todas las canciones desde la lista circular global (head)
    void cargarCancionesDesdeLista();

    // Genera un grafo de interacciones aleatorias
    void generarGrafoInteracciones(int interacciones);

    // Ejecuta PageRank y devuelve un vector de puntuaciones
    std::vector<double> ejecutarPageRank(double damping = 0.85, int iter = 1000);

    // Muestra el top N de canciones recomendadas
    void mostrarTop(int n, const std::vector<double>& pr);

private:

    std::vector<NodoCancion*> canciones;      // Lista lineal de canciones
    std::vector<std::vector<int>> grafo;      // Matriz de adyacencia
};

#endif