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
    int origem; //NÃO
    int peso;
    struct no *prox;
} NO;

typedef struct vertice{
	int flag;
    int custo; //dist
    int fechada; //aberto
    int chave; //NÃO
    NO* via; //int, registrar o nó toda vez q att no if(custo) e devolver numa lista de nó (como a lista caminho) ver linha 120
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
    f->nroElem++;
}

int sairFila(FILA * f){
    if(f->nroElem == 0) return 0;
    int i = f->A[0].chave;
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

VERTICE* criaGrafo(int N, int A, int *ijpeso, int *aberto, int inicio, int fim, int chave){
    //Início->novo ==> novo->início
    //Usar i/3 como base pra saber se é igual (se ijpeso[i] < i/3, vértice já existe)? Precisa verificar
    VERTICE* g = (VERTICE*) malloc(sizeof(VERTICE)*N);
    int j = 0;
    for(int i=0; i<(3*A); i+3){
        NO* p;
        NO* p2;

        p->peso = ijpeso[i+2];
        p->origem = ijpeso[i];
        p->adj = ijpeso[i+1];

        p2->peso = ijpeso[i+2];
        p2->origem = ijpeso[i+1];
        p2->adj = ijpeso[i];

        if(ijpeso[i] != ijpeso[i-3]) g[j].inicio = p;           //g = [1, 2, x, 4] 
        g[ijpeso[i+1]].inicio = p2;                             //1 (10)->(20) 2 (10) 4(20)

        if(ijpeso[i] == ijpeso[i-3] || ijpeso[i-2] == ijpeso[i]){
            NO* aresta = g[i-3].inicio;
            aresta->prox = p;
            aresta = aresta->prox;
        }
        if(ijpeso[i] != ijpeso[i+3]) j++;
    }
    //[1,2,10, 1,3,15, 1,4,20, 2,3,45, 2,4,30]

    return g;
}

//ADD
//Preciso retornar um NO* para verificar o caminho completo
int dijkstra(VERTICE * g, FILA * f, int inicio, int fim, int N, int ignorar){ //Att com lista de chaves
    int tem_chave = -1;

    int i;
    while(tamanhoFila(f) > 0){
    	i = sairFila(f);
    	NO * p = g[i].inicio;
    	while(p){
            p->origem = i; //Não presta

            if(g[p->adj].chave == 1){ 
                tem_chave = p->adj;
            }
            if(g[p->adj].fechada == 1 && ignorar == 1) p = p->prox; //Pulando a saala trancada

    		if(g[p->adj].custo > g[i].custo + p->peso){
                g[p->adj].custo = g[i].custo + p->peso;
                g[p->adj].via = p; //transformar em i quando alterar a struct
                //nos_caminho[i] = p

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

    return tem_chave;
}

NO* caminho(VERTICE * g, int inicio, int fim, int N){
    FILA * f = (FILA*) malloc(sizeof(FILA));
    inicializaFila(f);
    entrarFila(f, inicio);

    for(int x=0; x<N; x++){
        g[x].custo = 1000; //INT_MAX
    }

    g[inicio].custo = 0;
    g[inicio].via = -1; //null; Verificar o que é melhor marcar como via (ínidice do vértice ou valor do vértice)
    g[inicio].flag = 1; //Marcando início como percorrido

    int sala_chave = dijkstra(g, f, inicio, fim, N, 0);


    int tem_chave = 0;
    int trancada;

    NO* aresta = g[fim].via;

    while(aresta){
        if(g[aresta->origem].fechada == 1 || g[fim].fechada == 1) trancada = 1;
        if(trancada == 1 && g[aresta->origem].chave == 1) tem_chave = 1;
        aresta = g[aresta->origem].via;
    }

    int custo_trancada = 10000;
    int custo_ignorando = 10000;

    if(trancada == 1 && tem_chave == 0){
        dijkstra(g, f, inicio, sala_chave, N, 1);
        int custo_chave = g[sala_chave].custo;
        dijkstra(g, f, sala_chave, fim, N, 0);
        int custo_fim_chave = g[fim].custo;
        custo_trancada = custo_chave + custo_fim_chave;

        dijkstra(g, f, inicio, fim, N, 1);
        custo_ignorando = g[fim].custo;
    }

    int j = 0;
    NO* caminho[] = {};

    if(custo_ignorando > custo_trancada){ 
        dijkstra(g, f, inicio, sala_chave, N, 1);
        NO* aresta = g[sala_chave].via;
        while(aresta){
            caminho[j] = aresta;

            aresta = g[aresta->origem].via;
            j++;
        }
        dijkstra(g, f, sala_chave, fim, N, 0);
        NO* aresta = g[fim].via;
        while(aresta){
            caminho[j] = aresta;

            aresta = g[aresta->origem].via;
            j++;
        }
    } else{
        dijkstra(g, f, inicio, fim, N, 1);
        NO* aresta = g[fim].via;
        while(aresta){
            caminho[j] = aresta;

            aresta = g[aresta->origem].via;
            j++;
        }
    }

    //Invertendo lista

    int a=0;
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
Dúvida: padrão da lista ijpeso para criar no* prox;
Dúvida: como fazer o campo prox nos vértices?
    {1,2,10, 2,3,20, 3,1,10} -> É assim
    {1,2,10, 1,3,10, 2,3,20} -> Poderia ser assim
Problema: vértices tendo duas vias (em casos de ida e volta)
    A -> B -> C -> B -> D
    vias: A=null; B=A,C; C=B; D=B
*/
int main(){}