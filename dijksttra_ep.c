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

typedef struct no{
    int adj;
    int origem;
    int peso;
    struct no *prox;
} NO;

typedef struct vertice{
	int flag;
    int custo;
    int fechada;
    int chave;
    NO* via;
	NO * inicio;
} VERTICE;

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

void zerarFlags(VERTICE * g, int V){
    for(int i = 0; i < V; i++) g[i].flag = 0;
}

NO* caminho(VERTICE * g, int inicio, int fim, int N){
    FILA * f = (FILA*) malloc(sizeof(FILA));
    inicializaFila(f);
    entrarFila(f, inicio);

    int menor_custo = 0;

    for(int x=0; x<N; x++){
        g[x].custo = 10000;
    }

    g[inicio].custo = 0;
    g[inicio].via = -1; //null; Verificar o que é melhor marcar como via (ínidice do vértice ou valor do vértice)
    g[inicio].flag = 1; //Marcando início como percorrido
    
    int i;
    while(tamanhoFila(f) > 0){
    	i = sairFila(f);
    	NO * p = g[i].inicio;
    	while(p){
            p->origem = i;

    		if(g[p->adj].custo > g[i].custo + p->peso){
                g[p->adj].custo = g[i].custo + p->peso;
                g[p->adj].via = p; //i

                if(g[p->adj].flag == 0) g[p->adj].flag = 1;
                    
                entrarFila(f, p->adj);
            }
            if(g[p->adj].flag == 0){
                g[p->adj].flag = 1;
                entrarFila(f, p->adj);
            }
            p = p->prox;
    	}
    	g[i].flag = 2;
    }
    //Fim do Dijkstra

    //Criando uma lista de nós para retorno
    NO* caminho[] = {};
    NO* aresta = g[fim].via;
    int j = 0;
    while(1){
        caminho[j] = aresta;
        if(g[aresta->origem].via < 0) break;
        aresta = g[aresta->origem].via;
        j++;
    }
    int a = 0;
    while(j>a){
        NO* auxiliar = caminho[j];
        caminho[j] = caminho[a];
        caminho[a] = auxiliar;
        j--;
        a++;
    }

    return caminho;
}

/*
Dúvida: retornar caminho está correto?
Dúvida: onde em ijpeso está indicado que um vértice está trancado/possui chave?
Problema: vértices tendo duas vias (em casos de ida e volta)
    A -> B -> C -> B -> D
    vias: A=null; B=A,C; C=B; D=B
*/
int main(){}

/*
  <2>  2  <2> 
1               4
  <1>  3  <5>
*/