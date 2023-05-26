#include <stdio.h>
#include <stdlib.h>
#define MAX 50
#define true 1
#define false 0

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
    int peso;
    struct no *prox;
} NO;

typedef struct vertice{
	int flag;
    int dist;
    bool aberto; //bool aberto
    int via; //int, registrar o nó toda vez q att no if(custo) e devolver numa lista de nó (como a lista caminho) ver linha 120
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
    int i = f->A[f->inicio].chave;
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

VERTICE * criaGrafo(int N, int A, int *ijpeso){
        VERTICE * gr = (VERTICE*)malloc(sizeof(VERTICE)*(N+1));
        for(int j=0; j<N+1; j++){
            gr[j].inicio = NULL;
        }
        for(int i=0; i<(3*A); i+=3){
            NO * primeiro = (NO *)malloc(sizeof(NO));
            primeiro->peso = ijpeso[i+2];            
            primeiro->adj = ijpeso[i+1];            
            primeiro->prox = gr[ijpeso[i]].inicio;
            gr[ijpeso[i]].inicio = primeiro;

            NO * segundo = (NO *)malloc(sizeof(NO));
            segundo->peso = ijpeso[i+2];
            segundo->adj = ijpeso[i];
            segundo->prox = gr[ijpeso[i+1]].inicio;
            gr[ijpeso[i+1]].inicio = segundo;
        }
    return (gr);
}

void imprime(VERTICE *gr, int N){
    for (int i=0; i<=N; i++){
        NO * p = gr[i].inicio;
        printf("[%d] : ",i);
        while(p){
            printf("%d(%d), -> ", p->adj, p->peso);
            p=p->prox;
        }
        printf("NULL");
        printf("\n");
    }
}

//ADD
//Preciso retornar um NO* para verificar o caminho completo
VERTICE * dijkstra(VERTICE * g, int inicio, int fim, int N, int ignorar, int chave, int* aberto){
    FILA * f = (FILA*) malloc(sizeof(FILA));
    inicializaFila(f);
    entrarFila(f, inicio);

    for(int x=1; x<=N; x++){
        g[x].dist = 10000; //INT_MAX
        g[x].via = -1;
    }

    g[inicio].dist = 0;
    g[inicio].via = -1;
    g[inicio].flag = 1;

    int i;
    while(tamanhoFila(f) > 0){
    	i = sairFila(f);
    	NO * p = g[i].inicio;
    	while(p != NULL){
            if(aberto[(p->adj)-1] == false && ignorar == 1) p = p->prox; //Pulando a sala trancada

    		if(g[p->adj].dist > g[i].dist + p->peso){
                g[p->adj].dist = g[i].dist + p->peso;
                g[p->adj].via = i;
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

    printf("\nCaminho ao contrário");
    printf("\n%i ", fim);
    printf("Via dele: %i", g[fim].via);
    printf("\n%i ", g[fim].via);
    printf("Via dele: %i", g[g[fim].via].via);
    int ultimo = g[fim].via;
    printf("\n%i ", g[ultimo].via);
    printf("Via dele: %i", g[g[ultimo].via].via);
    return g;
}

void caminho(int N, int A, int *ijpeso, int *aberto, int inicio, int fim, int chave){
    VERTICE * g = criaGrafo(N, A, ijpeso);

    g = dijkstra(g, inicio, fim, N, false, chave, aberto);

    int tem_chave = 0;
    int trancada = 0;

    //int
    int vertice = fim;

    while(vertice != -1){
        if(aberto[vertice-1] == false) trancada = 1;
        if(trancada == 1 && vertice == chave) tem_chave = 1;
        vertice = g[vertice].via;
    }

    printf("\nComeça caminho");

    int j = 1;
    int caminho[] = {};
    
    if(trancada == 0 || (trancada == 1 && tem_chave == 1)){
        printf("\nTrancada 0");
        caminho[0] = fim;
        
        printf("\nValor de caminho[0]: %i", caminho[0]);
        
        int prox = g[fim].via;
        printf("\nDefiniu prox em trancada");

        while(prox != -1){
            printf("\nEntrou no while com prox = %i", prox);
            caminho[j] = prox;
            printf("\nValor de caminho[%i]: %i", j, caminho[j]);

            printf("\nVia de g[prox]: %i", g[prox].via);    //Não passou aqui com prox = 1

            prox = g[prox].via;
            printf("\nprox alterado para: %i", prox);
            if(prox != -1) j++;
            printf("\nPassou do if com prox = %i", prox);
        }
        printf("\nTerminou while");
    }

    printf("\npassou da trancada = 0\n");

    int custo_trancada = 10000;
    int custo_ignorando = 10000;

    if(trancada == 1 && tem_chave == 0){
        printf("\ntrancada 1 chave 0\n");
        dijkstra(g, inicio, chave, N, true, chave, aberto);
        int custo_chave = g[chave].dist;
        dijkstra(g, chave, fim, N, false, chave, aberto);
        int custo_fim_chave = g[fim].dist;
        custo_trancada = custo_chave + custo_fim_chave;

        dijkstra(g, inicio, fim, N, 1, chave, aberto);
        custo_ignorando = g[fim].dist;
    }

    if(custo_ignorando > custo_trancada){      
        printf("\ncustos\n");                     //Arrumar ordem de caminho[]
        dijkstra(g, inicio, chave, N, true, chave, aberto);      //Caminho => chave->início,fim->chave+1
        int vertice = g[chave].via;

        caminho[0] = chave;

        while(vertice != -1){
            caminho[j] = vertice;

            vertice = g[vertice].via;
            j++;
        }
        dijkstra(g, chave, fim, N, false, chave, aberto);
        vertice = fim;
        while(vertice != chave){
            caminho[j] = vertice;

            vertice = g[vertice].via;
            j++;
        }
    } else if(custo_ignorando < custo_trancada){
        printf("\ncustos 2\n");
        dijkstra(g, inicio, fim, N, 1, chave, aberto);

        caminho[0] = fim;

        int vertice = g[fim].via;
        while(vertice != -1){
            caminho[j] = vertice;

            vertice = g[vertice].via;
            j++;
        }
    }

    printf("\nCaminho ao contrário");
    for(int b=0; b<3; b++){ //b<caminho total
        printf("\nCaminho: %i\n", caminho[b]);
        printf("Via dele: %i", g[caminho[b]].via);

    }
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
int main(){
    int N= 5; // grafo de 3 vértices numerados de 1..3
    int A = 9;
	int aberto[] = {true, true, true, true, true}; // todos abertos
	int inicio=1;
	int fim=5;
	int chave=2;
	int ijpeso[]={1,2,10, 2,3,20, 3,1,10, 1,4,50, 2,4,15, 3,4,10, 5,1,15, 5,2,2, 5,4,5};

    VERTICE* g = criaGrafo(N, A, ijpeso);
    imprime(g, N);
    caminho(N, A, ijpeso, aberto, inicio, fim, chave);
}