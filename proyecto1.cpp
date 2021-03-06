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
#define Inf 10000000

using namespace std;

vector< pair< int,pair<int,int> > > lados;
vector< pair< int,pair<int,int> > > ladosaux;
vector< vector< pair<int,int> > > grafo;
vector< vector< pair<int,int> > > grafoR;
vector< vector< int > > grafoI;
vector< vector< int > > floyd;
vector< vector< int > > next;
vector< vector< int > > grafoCc;
vector< int > cc;
priority_queue< pair< int,pair<int,int> > > posibles;
vector<int> camino;
vector<int> ciclo;
vector<int> uf;

int visitados[105];
int r_edges[105][105];
int p_edges[105][105];
int q_edges[105][105];
int cuenta[105][105];
int es_r[105];


// union find

void inituf(int n){
	uf.clear();
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

vector<int> reconstruir(vector< vector<int> > &next, int u, int v){
	vector<int> camino;
	camino.push_back(u);
	while(u != v){
		u = next[u][v];
		camino.push_back(u);	
	}
	return camino;
}

vector< pair< int, pair<int,int> > > kruskal(vector< pair< int, pair<int,int> > > &lados, vector<int> &cc, int ids){
	priority_queue<pair< int, pair<int,int> >, vector< pair< int, pair<int,int> > >, greater< pair< int, pair<int,int> > > > cola;

	for(int i = 0; i < lados.size(); i++){
		cola.push(lados[i]);
	}
	inituf(ids);
	vector< pair< int, pair<int,int> > > lados_s;
	pair<int, pair<int,int> > lado;
	while(!cola.empty()){
		lado = cola.top();
		cola.pop();
		if(parent(cc[lado.ss.ff]) != parent(cc[lado.ss.ss])){
			joinfind(cc[lado.ss.ff],cc[lado.ss.ss]);
			//printf("%d %d\n",lado.ss.ff,lado.ss.ss);
			lados_s.push_back(lado);
		}
	}
	return lados_s;
}

bool es_par(vector<vector<pair<int,int> > > &grafo){
	bool es_par = true;
	int incidentes;
	for(int i = 0; i < grafo.size(); i++){
		incidentes = 0;
		for (int j = 0; j < grafo.size(); j++) {
			if (grafo[i][j].ff != -1) {
				incidentes++;
			} 		
		}
		if (incidentes % 2 != 0) {
			es_par = false;
			break;
		}
	}
	return es_par;
}

void gen_grafo_impar(vector< vector< pair<int,int> > > &grafoR, vector< vector< int > > &grafoI, vector< vector< int > > &floyd){
	vector<int> impares;
	int incidentes;
	for(int i = 0; i < grafoR.size(); i++){
		incidentes = 0;
		for (int j = 0; j < grafoR.size(); j++) {
			if (grafoR[i][j].ff != -1) {
				incidentes++;
			} 		
		}
		if (incidentes % 2 != 0) {
			impares.push_back(i);
			//printf("%d\n",i);
		}
	}

	for(int i = 0; i < impares.size(); i++){
		//printf("%d ",impares[i]);
		for(int j = i+1; j < impares.size(); j++){
			grafoI[impares[i]][impares[j]] = floyd[impares[i]][impares[j]];
			grafoI[impares[j]][impares[i]] = floyd[impares[j]][impares[i]];
		}
	}
	//printf("\n");

	for(int i = 0; i < grafoI.size(); i++){
		for(int j = i+1; j < grafoI[i].size(); j++){
			if(grafoI[i][j] != -1){
				//printf("%d %d %d\n",i,j,grafoI[i][j]);
			}
		}
	}
}

void perfect_matching(vector< vector< int > > &grafoI, vector< pair< int, pair<int,int> > > &lados){
	priority_queue<pair< int, pair<int,int> >, vector< pair< int, pair<int,int> > >, greater< pair< int, pair<int,int> > > > cola;
	pair< int, pair<int,int> > lado;

	for(int i = 0; i < grafoI.size(); i++){
		for(int j = i+1; j < grafoI[i].size(); j++){
			if(grafoI[i][j] != -1){
				cola.push(mp(grafoI[i][j],mp(i,j)));
			}
		}
	}
	lados.clear();
	while(!cola.empty()){
		lado = cola.top();
		cola.pop();
		if(grafoI[lado.ss.ff][lado.ss.ss] != -1){
			lados.push_back(lado);
			for(int i = 0; i < grafoI.size(); i++){
				for(int j = 0; j < grafoI[i].size(); j++){
					if(i == lado.ss.ff || i == lado.ss.ss || j == lado.ss.ff || j == lado.ss.ss){
						grafoI[i][j] = -1;
					}
				}
			}
		}
	}


}

// Realiza floyd warshall para hallar los caminos de mayor beneficio entre todo el grafo.
void floyd_warshall(vector< vector< int > > &floyd, vector< vector< int > > &next, int edges){

	for(int k = 1; k <= edges; k++){
		for(int i = 1; i <= edges; i++){
			for(int j = 1; j <= edges; j++){
				if(floyd[i][j] > floyd[i][k]+floyd[k][j]){
					//printf("%d %d %d %d %d %d\n", i,j,k,floyd[i][j], floyd[i][k], floyd[k][j]);
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
	lados.clear();
	int min;
	pair<int,int> lado;
	for(int k = 0; k < grafoCc.size(); k++){
		for(int i = k+1; i < grafoCc.size(); i++){
			min = 1000000;
			for(int l = 0; l < grafoCc[k].size(); l++){
				for(int j = 0; j < grafoCc[i].size(); j++){
					if(floyd[grafoCc[k][l]][grafoCc[i][j]] < min){
						min = floyd[grafoCc[k][l]][grafoCc[i][j]];
						//printf("%d\n",min);
						//printf("%d %d %d \n",k,l,grafoCc[k][l]);
						//printf("%d %d %d \n",i,j,grafoCc[i][j]);
						lado = mp(grafoCc[k][l],grafoCc[i][j]);
					}
				}
			}
		//printf("%d-%d-%d\n",min, lado.ff,lado.ss);
		lados.push_back(mp(min,lado));
		}
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
	bool puede = false;
	int id = 0;
	for(int i = 1; i <= edges; i++){
		puede = false;
		for(int j = 1; j < grafo[i].size(); j++){
			if(grafo[i][j].ff != -1){
				puede = true;
				break;
			}
		}
		if(cc[i] == -1 && puede){
			if(dfs(grafo,cc,i,id)){
				id++;
			}
		}
	}
	return id;

}

vector<int> eulerian_path(vector< vector< pair<int,int> > > &grafo){
	vector<int> res,st;
	vector<int> visitado;
	visitado.resize(grafo.size()+1);
	bool vacio;
	st.push_back(1);
	while(!st.empty()){
		int v = st.back();
		//printf("El nodo revisado es: %d \n", v);
		vacio = true;
		for(int i = 0; i < grafo[v].size(); i++){
			if(v!=i && grafo[v][i].ff != -1){
				//printf("El nodo %d tiene de fcking vecino 1ero a: %d \n",v,i);
				vacio = false;
				break;
			}
		}

		if(grafo[v].empty() || vacio){
			//printf("El nodo %d esta vacio\n",v);
			for(int i = 0; i < grafo.size(); i++){
				grafo[i][v] = mp(-1,-1);
				//grafo[v][i] = mp(-1,-1);
			}
			visitado[1] = 0;
			//if(!visitado[v]){
				res.push_back(v);
			//	visitado[v] = 1;
			//}
			st.pop_back();
		}
		else{
			//printf("El nodo no esta vacio\n");
			int primero;
			for(int i = 0; i < grafo[v].size(); i++){
				if(v!=i && grafo[v][i].ff != -1){
					//printf("El nodo %d tiene de fcking vecino a: %d \n",v,i);
					grafo[v][i] = mp(-1,-1);
					grafo[i][v] = mp(-1,-1);
					primero = i;
					break;
				}
			}
			st.push_back(primero);
			//grafo[v].pop_back();
		}
	}
	reverse(res.begin(),res.end());
	return res;
}

int calcular_beneficio(vector<int> ciclo, vector< vector< pair<int,int> > > grafo) {
	int beneficio = 0;
	for(int i = 0; i < ciclo.size()-1; i++) {
		beneficio += (grafo[ciclo[i]][ciclo[i+1]].ss - grafo[ciclo[i]][ciclo[i+1]].ff);
		grafo[ciclo[i]][ciclo[i+1]].ss = 0;
		grafo[ciclo[i+1]][ciclo[i]].ss = 0; 
	}
	return beneficio;
}

void elim_euleriana(vector<int> &ciclo, vector< vector< pair<int,int> > > grafo){
	int edges, beneficio_sol;
	vector< vector< pair<int,int> > > grafo_sol;
	vector< vector< pair<int,int> > > grafo_aux;
	vector< vector< pair<int,int> > > grafo_aux2;
	vector< int > cc;
	vector< vector< int > > grafoCc;
	beneficio_sol = calcular_beneficio(ciclo, grafo);
	// Creacion del grafo con los lados del ciclo optimo.
	edges = grafo.size();
	cc.resize(edges);
	grafo_sol.resize(edges);
	grafo_aux2.resize(edges);
	grafoCc.resize(edges);
	for(int i = 0; i < edges; i++){
		cc[i] = -1;
		grafo_sol[i].resize(edges);
		grafo_aux2[i].resize(edges);
	}
	for(int i = 0; i < edges; i++){
		for(int j = 0;j < edges; j++){
			grafo_sol[i][j] = mp(-1,-1);
			grafo_aux2[i][j] = mp(-1,-1);
		}
	}
	for (int i = 0; i < ciclo.size()-1; i++) {
		grafo_sol[ciclo[i]][ciclo[i+1]] = grafo[ciclo[i]][ciclo[i+1]];
	}

	//printf("%d\n", edges);
	//printf("El Grafo Solucion es: \n");
	//for(int i = 0; i < edges; i++){
	//	for(int j = 0; j< edges; j++){
	//		printf("(%d,%d) ", grafo_sol[i][j].ff, grafo_sol[i][j].ss);
	//	}
	//	printf("\n");
	//}
	//printf("\n");

	// Eliminacion euleriana de lados duplicados.
	for (int i = 0; i < edges; i++) {
		for (int j = i; j < edges; j++) {
			if (grafo_sol[i][j].ff != -1 && grafo_sol[j][i].ff != -1) {
				//printf("[%d][%d]\n", i,j);
				grafo_aux = grafo_sol;
				grafo_aux[j][i].ff = (grafo_aux[i][j].ff = -1);
				grafo_aux[j][i].ss = (grafo_aux[i][j].ss = -1);
				grafo_aux[j][j].ff = (grafo_aux[j][j].ss = 0);
				grafo_aux[i][i].ff = (grafo_aux[i][i].ss = 0);

				//printf("El Grafo Auxiliar es: \n");
				//for(int i = 0; i < edges; i++){
				//	for(int j = 0; j< edges; j++){
				//		printf("(%d,%d) ", grafo_aux[i][j].ff, grafo_aux[i][j].ss);
				//	}
				//	printf("\n");
				//}
				//printf("\n");
				if (comp_con(grafo_aux,cc,edges-1) == 1) {
					vector <int> ciclo_aux = eulerian_path(grafo_aux);
					//for(int k = 0; k < ciclo_aux.size(); k++){
					//	printf("%d ", ciclo_aux[k]);
					//}
					//printf("\n");
					int beneficio_n = calcular_beneficio(ciclo_aux, grafo);
					//printf("Los beneficios1 son: %d %d\n",beneficio_n, beneficio_sol);
					if (beneficio_n > beneficio_sol) {
						grafo_sol = grafo_aux;
						ciclo = ciclo_aux;
					}
				}
			}
		}
	}

	// Eliminacion euleriana de componentes conexas.
	llenar_componentes(grafoCc,cc);
	for(int i = 0; i < grafoCc[0].size(); i++){
		printf("%d ",grafoCc[0][i]);
	}
	printf("\n");
	for (int i = 0; i < grafoCc[0].size(); i++) {
		for(int j = 0; j < grafoCc[0].size(); j++){
			grafo_aux2[grafoCc[0][i]][grafoCc[0][j]] = grafo_sol[grafoCc[0][i]][grafoCc[0][j]];
			//grafo_aux2[grafoCc[0][j]][grafoCc[0][i]] = grafo_sol[grafoCc[0][j]][grafoCc[0][i]];
			//if(grafo_sol[grafoCc[0][i]][grafoCc[0][j]].ff != -1)
			//printf("%d %d %d %d\n",grafoCc[0][i],grafoCc[0][j],grafo_sol[grafoCc[0][i]][grafoCc[0][j]].ff,grafo_sol[grafoCc[0][i]][grafoCc[0][j]].ss);
		}


	}
	//printf("El Grafo Auxiliar2 es: \n");
	//for(int i = 0; i < edges; i++){
	//	for(int j = 0; j< edges; j++){
	//		printf("(%d,%d) ", grafo_aux2[i][j].ff, grafo_aux2[i][j].ss);
	//	}
	//	printf("\n");
	//}
	//printf("El nuevo ciclo es: \n");
	vector <int> ciclo_aux = eulerian_path(grafo_aux2);
	for(int k = 0; k < ciclo_aux.size(); k++){
		//printf("%d ", ciclo_aux[k]);
	}
	printf("\n");
	int beneficio_n = calcular_beneficio(ciclo_aux, grafo);
	//printf("Los beneficios2 son: %d %d\n",beneficio_n, beneficio_sol);
	if (beneficio_n > beneficio_sol) {
		grafo_sol = grafo_aux;
		ciclo = ciclo_aux;
	}

}


int main(){
	int v1,v2,costo,beneficio,edges,edg1,edg2,ids;
	pair<int,int> aux,aux1,aux2;

	scanf("number of vertices :  %d \n",&edges);
	scanf("number of required edges %d \n",&edg1);
	//printf("%d\n", edges);
	//printf("%d\n", edg1);

	memset(visitados,0,sizeof(visitados));
	memset(r_edges,0,sizeof(r_edges));
	memset(p_edges,0,sizeof(p_edges));
	memset(q_edges,0,sizeof(q_edges));

	lados.clear();
	grafo.resize(edges+1);
	grafoR.resize(edges+1);
	grafoI.resize(edges+1);
	floyd.resize(edges+1);
	next.resize(edges+1);
	cc.resize(edges+1);
	
	// Inicializaciones.
	for(int i = 0; i <= edges; i++){
		grafo[i].resize(edges+1);
		grafoR[i].resize(edges+1);
		grafoI[i].resize(edges+1);
		floyd[i].resize(edges+1);
		next[i].resize(edges+1);
		cc[i] = -1;
	}

	for(int i = 0; i <= edges; i++){
		for(int j = 0;j <= edges; j++){
			grafo[i][j] = mp(-1,-1);
			grafoR[i][j] = mp(-1,-1);
			grafoI[i][j] = -1;
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
	//printf("%d\n",edg2);
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
				floyd[i][j] = grafo[i][j].ff;
				next[i][j] = j;
				next[j][i] = i;
			}
			else {
				next[i][j] = j;
				next[j][i] = i;
				if(i == j){
					floyd[i][j] = Inf;
				}
				else{
					floyd[i][j] = Inf;
				}
			}
		}
	}

	/*
	// Aca imprimo el grafoR
	printf("El Grafo R es: \n");
	for(int i = 0; i <= edges; i++){
		for(int j = i+1; j<= edges; j++){
			if(grafoR[i][j].ff!=-1 || ((i == 1 || j == 1) && grafoR[i][j].ff!=-1)){
				printf("%d %d %d %d\n",i,j,grafoR[i][j].ff,grafoR[i][j].ss);
			}
		}
	}
	*/



	//Probando las componentes conexas de R
	ids = comp_con(grafoR,cc,edges);
	if(cc[1] == -1){
		cc[1]++;
		for(int i = 2; i < cc.size(); i++){
			if(cc[i] != -1){
				cc[i]++;
			}
		}
		ids++;
	}
	floyd_warshall(floyd,next,edges);
	vector<int> recnst;
	if(ids != 1){
		grafoCc.resize(ids);
		llenar_componentes(grafoCc,cc);	
		llenar_lados(floyd,grafoCc,lados);
		lados = kruskal(lados,cc,ids);
		for(int i = 0; i < lados.size(); i++){
			recnst.clear();
			recnst = reconstruir(next,lados[i].ss.ff,lados[i].ss.ss);
			for(int j = 0; j < recnst.size()-1; j++){
				if(grafo[recnst[j]][recnst[j+1]].ff != -1){
					grafoR[recnst[j]][recnst[j+1]] = grafo[recnst[j]][recnst[j+1]];
					grafoR[recnst[j+1]][recnst[j]] = grafo[recnst[j+1]][recnst[j]];
				}
				else{
					grafoR[recnst[j]][recnst[j+1]] = mp(floyd[recnst[j]][recnst[j+1]],-1);
					grafoR[recnst[j+1]][recnst[j]] = mp(floyd[recnst[j+1]][recnst[j]],-1);
				}
			}
		}
	}
	if(!es_par(grafoR)){
		gen_grafo_impar(grafoR,grafoI,floyd);
		perfect_matching(grafoI,lados);
		for(int i = 0; i < lados.size(); i++){
			//if(grafoR[lados[i].ss.ff][lados[i].ss.ss].ff == -1){
				// Son estas 2 para quitar el perfect
			//	grafoR[lados[i].ss.ff][lados[i].ss.ss] = mp(lados[i].ff,-1);
			//	grafoR[lados[i].ss.ss][lados[i].ss.ff] = mp(lados[i].ff,-1);
			//}
		}
	}
	ciclo = eulerian_path(grafoR);
	vector<int> reconst;
	vector<int> ciclo_aux;
	/*
	printf("El ciclo antes del Artificio: \n");
	for(int i = 0; i < ciclo.size(); i++){
		printf("%d ",ciclo[i]);
	}
	printf("\n");

	printf("El ciclo luego del Artificio: \n");
	for(int i = 0; i < ciclo.size(); i++){
		printf("%d ",ciclo[i]);
	}
	printf("\n");
	*/
	//printf("%d\n", ciclo[ciclo.size()-1]);
	//printf("%d\n", ciclo[ciclo.size()]);
	if(ciclo[ciclo.size()-1] != 1){
		reconst = reconstruir(next, ciclo[ciclo.size()-1], 1);
		for(int i = 1; i < reconst.size(); i++){
			ciclo.push_back(reconst[i]);
		}
	}
	ciclo_aux.push_back(ciclo[0]);
	for(int i = 0; i < ciclo.size()-1; i++){
		if (grafo[ciclo[i]][ciclo[i+1]].ff != floyd[ciclo[i]][ciclo[i+1]]) {
			camino = reconstruir(next, ciclo[i], ciclo[i+1]);
			for (int j = 1; j < camino.size(); j++) {
				ciclo_aux.push_back(camino[j]);
			}
		}
		else {
			ciclo_aux.push_back(ciclo[i+1]);
		}
	}
	

	//elim_euleriana(ciclo_aux, grafo);
	
	// Aun tiene detalles, la eliminacion devuelve un ciclo no factible.
	if(ciclo_aux[ciclo_aux.size()-1] != 1){
		reconst = reconstruir(next, ciclo_aux[ciclo_aux.size()-1], 1);
		for(int i = 1; i < reconst.size(); i++){
			ciclo_aux.push_back(reconst[i]);
		}
	}
	int resultado = calcular_beneficio(ciclo_aux, grafo);
	printf("%d\n", resultado);
	for(int i = 0; i < ciclo_aux.size(); i++){
		printf("%d ",ciclo_aux[i]);
	}
	printf("\n");
	int fact = 1;

	for(int i = 0; i < ciclo_aux.size()-1; i++){
		if(grafo[ciclo_aux[i]][ciclo_aux[i+1]].ff == -1){
			printf("%d %d\n",ciclo_aux[i],ciclo_aux[i+1]);
			fact = 0;
			break;
		}
	}
	//printf("Factible: %d\n",fact);
	//printf("Son %d\n",ids);


	/*
	for(int i = 0; i < grafoCc.size(); i++){
		printf("En la componente %d estan\n",i);
		for(int j = 0; j < grafoCc[i].size(); j++){
			printf("%d ",grafoCc[i][j]);
		}
		printf("\n");
	}
	*/


	
	/*
	printf("Lados de llenar lados de el vs de todos: \n");
	for(int i = 0; i < lados.size(); i++){
		printf("%d %d %d\n",lados[i].ff, lados[i].ss.ff, lados[i].ss.ss);
	}
	printf("\n");
	*/

	
	/*
	printf("Aca los del kruskal: \n");
	for(int i = 0; i < lados.size(); i++){
		printf("%d %d %d\n",lados[i].ff, lados[i].ss.ff, lados[i].ss.ss);
	}
	printf("\n");
	*/


	/*
	printf("El Grafo R es: \n");
	for(int i = 0; i <= edges; i++){
		for(int j = i+1; j<= edges; j++){
			if(grafoR[i][j].ff!=-1 || ((i == 1 || j == 1) && grafoR[i][j].ff!=-1)){
				printf("%d %d %d %d\n",i,j,grafoR[i][j].ff,grafoR[i][j].ss);
			}
		}
	}
	*/
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


	
	for(int i = 1; i < next.size(); i++){
		for(int j = 1; j < next[i].size(); j++){
			printf("%d ",next[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	for(int i = 1; i < floyd.size(); i++){
		for(int j = 1; j < floyd[i].size(); j++){
			printf("%d ",floyd[i][j]);
		}
		printf("\n");
	}
``	*/
	/*
	for(int i = 0; i < grafoCc.size(); i++){
		printf("En la componente %d estan\n",i);
		for(int j = 0; j < grafoCc[i].size(); j++){
			printf("%d ",grafoCc[i][j]);
		}
		printf("\n");
	}
	*/

	//printf("El camino es: \n");

	//for(int i = 0; i < camino.size(); i++){
	//	printf("%d ",camino[i]);
	//}
}
		
	

