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


class Compare
{
public:
    bool operator() (pair<int,pair<int,int> > a, pair<int,pair<int,int> > b)
    {
        return (a.ss.ss - a.ss.ff) >= (b.ss.ss - b.ss.ff);
    }
};

vector< pair< pair<int,int>,pair<int,int> > > lados;
vector< vector< pair<int,int> > > grafo;
vector< vector< pair<int,int> > > grafoR;
vector< vector< int > > floyd;
vector< vector< int > > grafoCc;
vector< int > cc;
priority_queue< pair< int,pair<int,int> >, vector< pair< int,pair<int,int> > >, Compare> posibles;
vector<int> camino;

int visitados[105];
int r_edges[105][105];
int p_edges[105][105];
int q_edges[105][105];
int cuenta[105][105];
int es_r[105];


void floyd_warshall(int edges){
	int aux,aux2,aux3;
	for(int k = 1; k <= edges; k++){
		for(int i = 1; i<= edges;i++){
			for(int j = 1; j <= edges; j++){
				aux = floyd[i][j];
				aux2 = floyd[i][k];
				aux3 = floyd[k][j];
				if(aux < aux2+aux3){
					floyd[i][j] = aux2+aux3; 
				}
			}
		}
	}

}

void llenar_componentes(){
	for(int i = 0; i < cc.size(); i++){
		if(cc[i] != -1){
			grafoCc[cc[i]].push_back(i);
		}
	}
}

// dfs con un booleano para eliminar componentes conexas falsas.
int dfs(vector< vector< pair<int,int> > > &grafo,int edge, int id){
	int falso = 0;
	cc[edge] = id;
	for(int i = 0; i < grafo[edge].size(); i++){
		if(grafo[edge][i].ff != 0){
			falso = 0;
			if(cc[i] == -1){
				dfs(grafo,i,id);
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
int comp_con(vector< vector< pair<int,int> > > &grafo,int edges){
	int id = 0;
	for(int i = 1; i <= edges; i++){
		if(cc[i] == -1){
			if(dfs(grafo,i,id)){
				id++;
			}
		}
	}
	return id;
}

int main(){
	int v1,v2,costo,beneficio,edges,edg1,edg2;

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
	cc.resize(edges+1);
	
	for(int i = 0; i <= edges; i++){
		grafo[i].resize(edges+1);
		grafoR[i].resize(edges+1);
		floyd[i].resize(edges+1);
		cc[i] = -1;
	}
	
	for(int i = 0; i <= edges; i++){
		for(int j = 0;j <= edges; j++){
			floyd[i][j] = -Inf;
			floyd[i][j] = grafo[i][j].ss - grafo[i][j].ff;
		}
	}

	while(edg1--){
		scanf("%d %d %d %d",&v1,&v2,&costo,&beneficio);
		aux = mp(v1,v2);
		aux1 = mp(v2,v1);
		aux2 = mp(costo,beneficio);
		grafo[v1][v2] = aux2;
		grafo[v2][v1] = aux2;
		lados.push_back(mp(aux,aux2));
		lados.push_back(mp(aux1,aux2));
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
		lados.push_back(mp(aux,aux2));
		lados.push_back(mp(aux1,aux2));
		p_edges[v1][v2] = 1;
		p_edges[v2][v1] = 1;
	}
	
	/*
	for(int i = 1; i <= edges; i++){
		for(int j = 1; j <= edges; j++){
			if(es_r[j] && !es_r[i]){
				grafoR[i][j] = grafo[i][j];
			}
		}
	}
	*/

	// Aca imprimo el grafoR
	printf("El Grafo R es: \n");
	for(int i = 0; i <= edges; i++){
		for(int j = i+1; j<= edges; j++){
			if(grafoR[i][j].ff!=0 || ((i == 1 || j == 1) && grafoR[i][j].ff!=0)){
				printf("%d %d %d %d\n",i,j,grafoR[i][j].ff,grafoR[i][j].ss);
			}
		}
	}

	//Probando las componentes conexas de R
	int ids = comp_con(grafoR,edges);

	grafoCc.resize(ids);

	printf("Las componentes son: \n");
	for(int i = 0; i <= edges; i++){
		for(int j = i+1; j <= edges; j++){
			if(grafoR[i][j].ff != 0){
				printf("%d %d %d %d\n",i,j,grafoR[i][j].ff,grafoR[i][j].ss);
				printf("Su id es: %d %d\n",cc[i],cc[j]);
			}
		}
	}	

	//printf("El camino es: \n");

	//for(int i = 0; i < camino.size(); i++){
	//	printf("%d ",camino[i]);
	//}
	printf("\n");

}
