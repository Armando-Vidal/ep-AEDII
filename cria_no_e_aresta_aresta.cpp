NO *criaNo (int v, int peso){
  NO *temp = (NO *)malloc(sizeof(NO));
  temp->adj = v;
  temp->peso = peso;
  temp->prox = NULL;
  return (temp);
}

bool criaAresta (GRAFO *gr, int vi, int vf, int p){
  
  NO *novo = criaNo(vf,p);
  novo->prox = gr->adj[vi].inicio;
  gr->adj[vi].inicio = novo;
  gr->arestas++;
  return (true);
}
