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
    int via;
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

//FUNCAO QUE CRIA UM GRAFO A PARTIR DAS INFO FORNECIDAS
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

//FUNÇÃO QUE IMPRIME O GRAFO PRA VER SE TA CERTO
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

void caminho(VERTICE * g, int inicio, int fim, int N){
    FILA * f = (FILA*) malloc(sizeof(FILA));
    inicializaFila(f);
    entrarFila(f, inicio);

    for(int x=0; x<=N; x++){
        g[x].custo = 10000;
        g[x].via = -1;
    }

    g[inicio].custo = 0;
    g[inicio].via = -1; //null; Verificar o que é melhor marcar como via (ínidice do vértice ou valor do vértice)
    g[inicio].flag = 1; //Marcando início como percorrido
    
    int i;
    while(tamanhoFila(f) > 0){
    	i = sairFila(f);
    	NO * p = g[i].inicio;
    	while(p != NULL){

    		if(g[p->adj].custo > g[i].custo + p->peso){
                g[p->adj].custo = g[i].custo + p->peso;
                g[p->adj].via = i; //i

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
    int caminho[] = {};
    caminho[0] = fim;

    int prox = g[fim].via;
    int j = 1;
    while(prox != -1){
        caminho[j] = prox;
        prox = g[prox].via;
        if(prox != -1) j++;
    }

    printf("\nCaminho ao contrário");
    for(int b=0; b<3; b++){ //a<caminho total
        printf("\n%i\n", caminho[b]);
    }
}

/*
Dúvida: retornar caminho está correto?
Dúvida: onde em ijpeso está indicado que um vértice está trancado/possui chave?
Problema: vértices tendo duas vias (em casos de ida e volta)
    A -> B -> C -> B -> D
    vias: A=null; B=A,C; C=B; D=B
*/
int main(){
    int N= 5; // grafo de 3 vértices numerados de 1..3
    int A = 9;
	int aberto[] = {1,1,1}; // todos abertos
	int inicio=1;
	int fim=5;
	int chave=2;
	int ijpeso[]={1,2,10, 2,3,20, 3,1,10, 1,4,50, 2,4,15, 3,4,10, 5,1,15, 5,2,2, 5,4,5};

    VERTICE* g = criaGrafo(N, A, ijpeso);
    imprime(g, N);
    caminho(g, inicio, fim, N);
}

/*
  <2>  2  <2> 
1               4
  <1>  3  <5>
*/