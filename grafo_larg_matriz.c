#include <stdio.h>
#include <stdlib.h>
#define MAX 50

typedef int bool;

typedef int TIPOCHAVE;

typedef struct{
    TIPOCHAVE chave;
} REGISTRO;

typedef struct{
    REGISTRO A[MAX];
    int inicio;
    int nroElem;
} FILA;

void inicializaFila(FILA * f){
    f->inicio = 0;
    f->nroElem = 0;
}

void entrarFila(FILA * f, int reg){
    if(f->nroElem >= MAX) return;
    int posicao = (f->inicio + f->nroElem) % MAX;
    f->A[posicao].chave = reg;
}

int sairFila(FILA * f){
    if(f->nroElem == 0) return 0;
    int i = f->inicio;
    f->inicio = (f->inicio + 1) % MAX;
    f->nroElem--;
    return i;
}

int tamanhoFila(FILA * f){
    return f->nroElem;
}

void zerarFlags(int flags[], int V){
    for(int i=0; i<V; i++) flags[i] = 0;
}

int V;

void larg(int m[V][V], int flags[], int i){
    FILA * f = (FILA *) malloc(sizeof(FILA));
    inicializaFila(f);
    entrarFila(f, i);

    while(tamanhoFila(f) > 0){
        i = sairFila(f);
        flags[i] = 1;

        for(int j=0; j<V; j++){
            if(m[i][j] == 1 && flags[j] == 0){
                entrarFila(f, j);
                //flags[j] = 1;
            }
        }
        flags[i] = 2;
    }
}

int main(){}

//OBS: Para qualquer propriedade do vértice do grafo, criar uma nova lista com o mesmo índice dos vértices (como a lista de flags)