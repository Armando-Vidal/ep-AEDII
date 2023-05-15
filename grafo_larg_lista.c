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
    struct no *prox;
} NO;

typedef struct vertice{
	int flag;
    int dist;
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

int v;
int i;

void larg(VERTICE * g, int ini){
    FILA * f = (FILA*) malloc(sizeof(FILA));
    inicializaFila(f);
    entrarFila(f, ini);
    g[ini].flag = 1; //Marcando início como percorrido
    
    while(tamanhoFila(f) > 0){
    	i = sairFila(f);

    	//if(g[i].qtd = 0){ //Encontrando o ponto desejado
    	//	while(tamanhoFila(f) > 0){ //Laço para liberar a memória
    	//		sairFila(&f);
    	//	}
    	//	return i; //Retornando ponto desejado
    	//}
    	NO * p = g[i].inicio;
    	while(p){
    		if(g[p->adj].flag == 0){
    			g[p->adj].flag = 1;
    			//g[p->adj].dist = g[i].dist + 1;//ou g[i].dist + g[p->adj].dist  //Distância do início a partir do elemento anterior
    			entrarFila(f, p->adj);                                            //mesmo sistema p/ exercício
    		}
            p = p->prox;
    	}
    	g[i].flag = 2;
    }
}

int main(){}