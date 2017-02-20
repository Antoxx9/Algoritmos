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

using namespace std;

vector< pair< pair<int,int>,pair<int,int> > > lados;
vector< vector< pair<int,int> > > grafo;
vector< vector< pair<int,int> > > grafoR;

int visitados[105];
int r_edges[105][105];
int p_edges[105][105];
int q_edges[105][105];
int cc[105];
int cuenta[105];
int es_r[105];

void dfs(int edge, int id){
	for(int i = 0; i < grafo[edge].size(); i++){
		if(grafo[edge][i].ff != 0){
			if(cc[i] == 0){
				dfs(i,id);
			}
		}
	}
}

void comp_con(int edges){
	int id = 1;
	for(int i = 1; i <= edges; i++){
		if(cc[i] == 0){
			dfs(i,id++);
		}
	}
}


int main(){
	int v1,v2,costo,beneficio,edges;
	pair<int,int> aux,aux1,aux2;
	scanf("%d",&edges);
	memset(visitados,0,sizeof(visitados));
	memset(r_edges,0,sizeof(r_edges));
	memset(p_edges,0,sizeof(p_edges));
	memset(q_edges,0,sizeof(q_edges));
	memset(cc,0,sizeof(cc));
	memset(cuenta,0,sizeof(cuenta));
	memset(es_r,0,sizeof(es_r));
	grafo.resize(edges+1);
	grafoR.resize(edges+1);
	for(int i = 0; i <= edges; i++){
		grafo[i].resize(edges+1);
		grafoR[i].resize(edges+1);
	}
	while(scanf("%d %d %d %d",&v1,&v2,&costo,&beneficio)!= EOF){
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
		else if(beneficio < costo){
			p_edges[v1][v2] = 1;
			p_edges[v2][v1] = 1;
		}
		else{
			q_edges[v1][v2] = 1;
			q_edges[v2][v1] = 1;
		}
	}

	for(int i = 1; i <= edges; i++){
		for(int j = 1; j <= edges; j++){
			if(es_r[j] && !es_r[i]){
				grafoR[i][j] = grafo[i][j];
			}
		}
	}
}
