//--------------------------------------------------------------
// NOMES DOS RESPONSÁVEIS: Armando Augusto Marchini Vidal E Lucas
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#define MAX 50
#define true 1
#define false 0

int grupo() {
  return 0;
}

// ######### ESCREVA O NROUSP AQUI
int nroUSP1() {
    return 13673072;
}

int nroUSP2() {
    return 0;
}

typedef int TIPOCHAVE;

typedef struct{
    TIPOCHAVE chave;
} REGISTRO;

typedef struct{
    REGISTRO A[MAX];
    int inicio;
    int nroElem;
} FILA;

// elemento das listas de adjacência e de resposta - NAO ALTERE ESTA DEFINICAO
typedef struct estr 
{
    int adj; // elemento
	int peso; // custo (não precisa ser usado na resposta)
    struct estr *prox;
} NO;

// vertices do grafo (salas) - use este tipo ao criar o grafo  - NAO ALTERE ESTA DEFINICAO
typedef struct 
{
    int flag; // para uso na busca em largura e profundidade, se necessario
    bool aberto; // vale true se a sala em questao esta aberta
    int via; // use este campo se precisar computar algum caminho etc.
	int dist; // use este campo se precisar computar alguma distância etc.
    NO* inicio;
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
    for(int i = 0; i <= V; i++) g[i].flag = 0;
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

VERTICE * dijkstra(VERTICE * g, int inicio, int fim, int N, int ignorar, int chave, int* aberto){
    zerarFlags(g, N);
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
            if(aberto[(p->adj)] == false && ignorar == true){
                p = p->prox;    //Pulando a sala trancada
                if(p == NULL) break;
            }

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
    return g;
}

//------------------------------------------
// O EP consiste em implementar esta funcao
// e outras funcoes auxiliares que esta
// necessitar
//------------------------------------------
NO *caminho(int N, int A, int *ijpeso, int *aberto, int inicio, int fim, int chave)
{
	NO* resp = (NO*) malloc(sizeof(NO));
	//resp = NULL;

	VERTICE * g = criaGrafo(N, A, ijpeso);

    g = dijkstra(g, inicio, fim, N, false, chave, aberto);

    int tem_chave = 0;
    int trancada = 0;

    //int
    int vertice = fim;
    int tam = 1;

    while(vertice != -1){
        if(aberto[vertice] == false) trancada = 1;
        if(trancada == 1 && vertice == chave) tem_chave = 1;
        vertice = g[vertice].via;
        if(vertice != -1) tam++;
    }

    int j = 1;
    
    if(trancada == 0 || (trancada == 1 && tem_chave == 1)){
        int caminho[tam];
        caminho[0] = fim;
        
        int prox = g[fim].via;

        while(prox != -1){
            caminho[j] = prox;

            prox = g[prox].via;
            if(prox != -1) j++;
        }

        j = tam-1;

        int caminho_certo[tam];
        
        for(int a=0; a<tam; a++){
            caminho_certo[a] = caminho[j];
            j--;
        }

        resp->adj = caminho_certo[0];
        resp->peso =  0;
        NO* jorge = (NO*) malloc(sizeof(NO));
        resp->prox = jorge;
        for(int h=1; h<tam; h++){
            NO* aux = (NO*) malloc(sizeof(NO));
            jorge->adj = caminho_certo[h];
            jorge->peso = g[caminho_certo[h]].dist;
            jorge->prox = aux;
            jorge = jorge->prox;
        }
        jorge->adj = NULL;
        jorge->prox = NULL;
    }

    int custo_trancada = 100;
    int custo_ignorando = 100;

    if(trancada == 1 && tem_chave == 0){
        vertice = fim;
        g = dijkstra(g, inicio, chave, N, true, chave, aberto);
        int custo_chave = g[chave].dist;
        g = dijkstra(g, chave, fim, N, false, chave, aberto);
        int custo_fim_chave = g[fim].dist;
        custo_trancada = custo_chave + custo_fim_chave;

        g = dijkstra(g, inicio, fim, N, true, chave, aberto);
        custo_ignorando = g[fim].dist;
        
        if(custo_ignorando > custo_trancada){
            tam = 1;
                                                                        
            g = dijkstra(g, inicio, chave, N, true, chave, aberto);
            vertice = chave;
            while(vertice != -1){
                if(vertice != -1) tam++;
                vertice = g[vertice].via;
            }

            g = dijkstra(g, chave, fim, N, false, chave, aberto);
            vertice = fim;

            while(vertice != chave){
                tam++;
                vertice = g[vertice].via;
            }
            tam--;

            int caminho[tam];

            g = dijkstra(g, inicio, chave, N, true, chave, aberto);

            
            int custos[] = {};
            int iterador = 0;

            VERTICE distancia = g[chave];

            while(distancia.via != -1){
                custos[iterador] = distancia.dist;
                distancia = g[distancia.via];
                iterador++;
            }
            
            custos[iterador] = 0;

            int custo_somado = g[chave].dist;

            vertice = g[chave].via;

            caminho[0] = chave;

            while(vertice != -1){
                caminho[j] = vertice;

                vertice = g[vertice].via;
                j++;
            }

            g = dijkstra(g, chave, fim, N, false, chave, aberto);
            vertice = fim;            

            while(vertice != chave){
                caminho[j] = vertice;

                vertice = g[vertice].via;
                j++;
            }

            //Ajeitar caminho
            int z=0;
            while(1){
                if(caminho[z]==inicio) break;
                z++;
            }

            for(int w=0; w<z; w++){
                int aux = caminho[w];
                caminho[w] = caminho[z];
                caminho[z] = aux;
                z--;
            }

            z=0;
            while(1){
                if(caminho[z]==fim) break;
                z++;
            }

            for(int w=tam-1; w>z; w--){
                int aux = caminho[w];
                caminho[w] = caminho[z];
                caminho[z] = aux;
                z++;
            }
            resp->adj = caminho[0];
            resp->peso =  0;
            NO* jorge = (NO*) malloc(sizeof(NO));
            resp->prox = jorge;
            int iterador_dois = iterador-1;
            int passa = 0;
            for(int h=1; h<tam; h++){
                NO* aux = (NO*) malloc(sizeof(NO));
                jorge->adj = caminho[h];
                
                if(caminho[h] != chave && passa == 0) jorge->peso = custos[iterador_dois];
                else if(caminho[h] == chave){
                    passa = 1;
                    jorge->peso = g[caminho[h]].dist + custo_somado;
                } 
                else jorge->peso = g[caminho[h]].dist + custo_somado;

                jorge->prox = aux;
                jorge = jorge->prox;
                iterador--;
            }
            jorge->adj = NULL;
            jorge->prox = NULL;
        } else if(custo_ignorando <= custo_trancada){
            tam = 1;
            g = dijkstra(g, inicio, fim, N, true, chave, aberto);

            while(vertice != -1){
                tam++;
                vertice = g[vertice].via;
            }

            int caminho[tam - 1];
    
            caminho[0] = fim;
    
            int vertice = g[fim].via;
            while(vertice != -1){
                caminho[j] = vertice;
    
                vertice = g[vertice].via;
                j++;
            }

            j = tam-2;

            int caminho_certo[tam-1];

            for(int a=0; a<tam-1; a++){
                caminho_certo[a] = caminho[j];
                j--;
            }

            resp->adj = caminho_certo[0];
            resp->peso =  0;
            NO* jorge = (NO*) malloc(sizeof(NO));
            resp->prox = jorge;
            for(int h=1; h<tam-1; h++){
                NO* aux = (NO*) malloc(sizeof(NO));
                jorge->adj = caminho_certo[h];
                jorge->peso = g[caminho_certo[h]].dist;
                jorge->prox = aux;
                jorge = jorge->prox;
            }
            jorge->adj = NULL;
            jorge->prox = NULL;
        }
    }

	return resp;
}

//---------------------------------------------------------
// use main() para fazer chamadas de teste ao seu programa
//---------------------------------------------------------
int main() {


	// aqui vc pode incluir codigo de teste

	// exemplo de teste trivial

	int N=6; // grafo de 3 vértices numerados de 1..3
    int A = 7;
	int aberto[] = {0, 1,0,1,1,1}; // todos abertos
	int inicio=1;
	int fim=5;
	int chave=3;
	int ijpeso[]={1,2,2, 1,6,2, 6,3,3, 2,5,10, 3,2,12, 3,5,10, 3,4,10};

	// o EP sera testado com uma serie de chamadas como esta
	NO* teste = NULL;
	teste = caminho(N, A, ijpeso, aberto, inicio, fim, chave);

    while(teste->adj!=NULL){
        printf("\nresp: %i", teste->adj);
        printf("\ncusto: %i", teste->peso);
        teste = teste->prox;
    }
	return 0;

}

// por favor nao inclua nenhum código abaixo da função main()

