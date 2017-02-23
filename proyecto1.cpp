#include <vector>
#include <cstdio>
#include <string.h>
#include <algorithm>
#include <functional>
#include <utility>
#include <queue>
#include <map>
#include <iostream>

#define ss second
#define ff first
#define mp make_pair
#define Inf 1000000

using namespace std;

vector< pair< int,pair<int,int> > > lados;
vector< vector< pair<int,int> > > grafo;
vector< vector< pair<int,int> > > grafoR;
vector< vector< int > > floyd;
vector< vector< int > > next;
vector< vector< int > > grafoCc;
vector< int > cc;
priority_queue< pair< int,pair<int,int> > > posibles;
vector<int> camino;
vector<int> uf;

int visitados[105];
int r_edges[105][105];
int p_edges[105][105];
int q_edges[105][105];
int cuenta[105][105];
int es_r[105];


// union find

void inituf(int n){
	
	for(int i = 0; i < n; i++){
		uf.push_back(i);
	}

}

int parent(int n){

	if(uf[n] == n){
		return n;
	}
	else{
		return uf[n] = parent(uf[n]);
	}

}

void setparent(int n, int i){

	uf[n] = i;

}

void joinfind(int n, int i){

	uf[parent(n)] = uf[parent(i)];

}


// Realiza floyd warshall para hallar los caminos de mayor beneficio entre todo el grafo.
void floyd_warshall(vector< vector< int > > &floyd, vector< vector< int > > &next, int edges){

	for(int k = 1; k <= edges; k++){
		for(int i = 1; i<= edges; i++){
			for(int j = 1; j <= edges; j++){
				if(floyd[i][j] < floyd[i][k]+floyd[k][j]){
					floyd[i][j] = floyd[i][k]+floyd[k][j]; 
					next[i][j] = next[i][k];
				}
			}
		}
	}

}

// crea un nuevo grafo donde cada nodo es una componente conexa.
void llenar_componentes(vector< vector< int > > &grafo, vector< int > &cc){

	for(int i = 0; i < cc.size(); i++){
		if(cc[i] != -1){
			grafo[cc[i]].push_back(i);
		}
	}

}

// Llena con el lado de menor costo entre cada componente conexa, diciendo de donde a donde va.
void llenar_lados(vector< vector< int > > &floyd, vector< vector< int > > &grafoCc, vector< pair< int,pair<int,int> > > &lados){
	
	int min;
	pair<int,int> lado;
	for(int k = 0; k <= grafoCc.size(); k++){
		for(int i = 0; i!= k && i <= grafoCc.size(); i++){
			min = 1000;
			for(int l = 0; l <= grafoCc[k].size(); l++){
				lado = mp(0,0);
				for(int j = 0; j <= grafoCc[i].size(); j++){
					if(floyd[grafoCc[k][l]][grafoCc[i][j]] < min){
						min = floyd[grafoCc[k][l]][grafoCc[i][j]];
						lado = mp(grafoCc[k][l],grafoCc[i][j]);
					}
				}
			}
		}
		lados.push_back(mp(min,lado));
	}

}

// dfs con un booleano para eliminar componentes conexas falsas.
int dfs(vector< vector< pair<int,int> > > &grafo, vector< int > &cc, int edge, int id){
	
	int falso = 0;
	cc[edge] = id;
	for(int i = 0; i < grafo[edge].size(); i++){
		if(grafo[edge][i].ff != -1){
			falso = 0;
			if(cc[i] == -1){
				dfs(grafo,cc,i,id);
			}
		}
	}
	if(falso){
		cc[edge] = -1;
		return 0;
	}
	return 1;

}

// Hace Dfs desde cada nodo de el grafo que aun no haya sido visitado.
int comp_con(vector< vector< pair<int,int> > > &grafo, vector< int > &cc, int edges){
	
	int id = 0;
	for(int i = 1; i <= edges; i++){
		if(cc[i] == -1){
			if(dfs(grafo,cc,i,id)){
				id++;
			}
		}
	}
	return id;

}

int main(){
	int v1,v2,costo,beneficio,edges,edg1,edg2,ids;
	pair<int,int> aux,aux1,aux2;

	scanf("number of vertices :  %d \n",&edges);
	scanf("number of required edges %d \n",&edg1);
	printf("%d\n", edges);
	printf("%d\n", edg1);

	memset(visitados,0,sizeof(visitados));
	memset(r_edges,0,sizeof(r_edges));
	memset(p_edges,0,sizeof(p_edges));
	memset(q_edges,0,sizeof(q_edges));

	grafo.resize(edges+1);
	grafoR.resize(edges+1);
	floyd.resize(edges+1);
	next.resize(edges+1);
	cc.resize(edges+1);
	
	// Inicializaciones.
	for(int i = 0; i <= edges; i++){
		grafo[i].resize(edges+1);
		grafoR[i].resize(edges+1);
		floyd[i].resize(edges+1);
		next[i].resize(edges+1);
		cc[i] = -1;
	}

	for(int i = 0; i <= edges; i++){
		for(int j = 0;j <= edges; j++){
			grafo[i][j] = mp(-1,-1);
			grafoR[i][j] = mp(-1,-1);
		}
	}


	// Comienzo de la lectura
	while(edg1--){
		scanf("%d %d %d %d",&v1,&v2,&costo,&beneficio);
		aux = mp(v1,v2);
		aux1 = mp(v2,v1);
		aux2 = mp(costo,beneficio);
		grafo[v1][v2] = aux2;
		grafo[v2][v1] = aux2;
		if(beneficio >= 2*costo){
			r_edges[v1][v2] = 1;
			r_edges[v2][v1] = 1;
			es_r[v1] = 1;
			es_r[v2] = 1;
			grafoR[v1][v2] = aux2;
			grafoR[v2][v1] = aux2;
		}
		else{
			grafoR[v1][v2] = aux2;
			grafoR[v2][v1] = aux2;
			q_edges[v1][v2] = 1;
			q_edges[v2][v1] = 1;
		}
	}

	scanf("\nnumber of non required edges  %d \n",&edg2);
	printf("%d\n",edg2);
	while(edg2--){
		scanf("%d %d %d %d",&v1,&v2,&costo,&beneficio);
		aux = mp(v1,v2);
		aux1 = mp(v2,v1);
		aux2 = mp(costo,beneficio);
		grafo[v1][v2] = aux2;
		grafo[v2][v1] = aux2;
		p_edges[v1][v2] = 1;
		p_edges[v2][v1] = 1;
	}
	
	// Inicializacion de floyd y next
	for(int i = 0; i <= edges; i++){
		for(int j = 0; j <= edges; j++){
			if(grafo[i][j].ff != -1){
				floyd[i][j] = -grafo[i][j].ff;
				next[i][j] = j;
				next[j][i] = j;
			}
			else{
				next[i][j] = -1;
				next[j][i] = -1;
				if(i == j){
					floyd[i][j] = 0;
				}
				else{
					floyd[i][j] = -Inf;
				}
			}
		}
	}

	// Aca imprimo el grafoR
	printf("El Grafo R es: \n");
	for(int i = 0; i <= edges; i++){
		for(int j = i+1; j<= edges; j++){
			if(grafoR[i][j].ff!=-1 || ((i == 1 || j == 1) && grafoR[i][j].ff!=-1)){
				printf("%d %d %d %d\n",i,j,grafoR[i][j].ff,grafoR[i][j].ss);
			}
		}
	}

	//Probando las componentes conexas de R
	ids = comp_con(grafoR,cc,edges);

	grafoCc.resize(ids);

	llenar_componentes(grafoCc,cc);

	floyd_warshall(floyd,next,edges);

	/*
	printf("Las componentes son: \n");
	for(int i = 0; i <= edges; i++){
		for(int j = i+1; j <= edges; j++){
			if(grafoR[i][j].ff != -1){
				printf("%d %d %d %d\n",i,j,grafoR[i][j].ff,grafoR[i][j].ss);
				printf("Su id es: %d %d\n",cc[i],cc[j]);
			}
		}
	}	
	

	for(int i = 1; i < floyd.size(); i++){
		for(int j = 1; j < floyd[i].size(); j++){
			printf("%d ",floyd[i][j]);
		}
		printf("\n");
	}
	*/

	for(int i = 0; i < grafoCc.size(); i++){
		printf("En la componente %d estan\n",i);
		for(int j = 0; j < grafoCc[i].size(); j++){
			printf("%d ",grafoCc[i][j]);
		}
		printf("\n");
	}


	//printf("El camino es: \n");

	//for(int i = 0; i < camino.size(); i++){
	//	printf("%d ",camino[i]);
	//}
	printf("\n");

}
