#include <vector>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <utility>
#include <queue>
#include <map>
#include <iostream>
#include <time.h>

#define ss second
#define ff first
#define mp make_pair
#define Inf 1000000

using namespace std;


vector<int> uf;

//Estructura de conjuntos disjuntos (union find) utilizada para la implementacion
//del algoritmo de kruskal para hallar el MST de un grafo.

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

// Funcion reconstruir la cual a partir de 2 nodos, reconstruye el camino de
// costo minimo entre ellos y los retorna en un vector. 
vector<int> reconstruir(vector< vector<int> > &next, int u, int v){
	vector<int> camino;
	camino.push_back(u);
	while(u != v){
		u = next[u][v];
		camino.push_back(u);	
	}
	return camino;
}

// Funcion kruskal la cual a partir de una lista de lados y componentes conexas
// halla el MST para conectar todas las componentes entre si
vector< pair< int, pair<int,int> > > kruskal(vector< pair< int, pair<int,int> > > &lados, 
											 vector<int> &cc, int ids){
	priority_queue<pair< int, pair<int,int> >, vector< pair< int, pair<int,int> > >, 
				   greater< pair< int, pair<int,int> > > > cola;
	vector< pair< int, pair<int,int> > > lados_s;
	pair<int, pair<int,int> > lado;
	for(int i = 0; i < lados.size(); i++){
		cola.push(lados[i]);
	}

	inituf(ids);
	
	while(!cola.empty()){
		lado = cola.top();
		cola.pop();
		if(parent(cc[lado.ss.ff]) != parent(cc[lado.ss.ss])){
			joinfind(cc[lado.ss.ff],cc[lado.ss.ss]);
			lados_s.push_back(lado);
		}
	}
	return lados_s;
}

// Funcion es_par, la cual dada un grafo en el cual se pueden
// representar lados duplicados, calcula la paridad del mismo.
bool es_par(vector<vector<vector<pair<int,int> > > > &grafo){
	bool es_par = true;
	int incidentes;
	for(int i = 0; i < grafo.size(); i++){
		incidentes = 0;
		for (int j = 0; j < grafo.size(); j++) {
			if (grafo[i][j].size() > 0) {
				incidentes+= grafo[i][j].size();
			} 		
		}
		if (incidentes % 2 != 0) {
			es_par = false;
			break;
		}
	}
	return es_par;
}

// Funcion gen_grafo_impar, a partir de un grafo que no es par, genera un
// grafo completo con todos los nodos de grado impar el cual esta compuesto
// por los caminos de costo minimo entre todos los nodos.
void gen_grafo_impar(vector< vector< vector< pair<int,int> >  > > &grafoR, vector< vector< int > > &grafoI, 
	                 vector< vector< int > > &floyd){
	vector<int> impares;
	int incidentes;
	for(int i = 0; i < grafoR.size(); i++){
		incidentes = 0;
		for (int j = 0; j < grafoR.size(); j++) {
			if (grafoR[i][j].size() > 0) {
				incidentes+= grafoR[i][j].size();
			} 		
		}
		if (incidentes % 2 != 0) {
			impares.push_back(i);
		}
	}

	for(int i = 0; i < impares.size(); i++){
		for(int j = i+1; j < impares.size(); j++){
			grafoI[impares[i]][impares[j]] = floyd[impares[i]][impares[j]];
			grafoI[impares[j]][impares[i]] = floyd[impares[j]][impares[i]];
		}
	}
}

// Funcion perfect_matching, la cual se encarga de generar el apareamiento
// minimo perfecto de un grafo con la finalidad de volver par al mismo, devuelve
// la lista de lados que se añadiran al grafo.
void perfect_matching(vector< vector< int > > &grafoI, vector< pair< int, pair<int,int> > > &lados){
	priority_queue<pair< int, pair<int,int> >, vector< pair< int, pair<int,int> > >, 
	                    greater< pair< int, pair<int,int> > > > cola;
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

// Funcion floyd_warshall, la cual calcula los caminos de costo minimo entre cada
// par de nodos del grafo.
void floyd_warshall(vector< vector< int > > &floyd, vector< vector< int > > &next, int edges){
	for(int k = 1; k <= edges; k++){
		for(int i = 1; i <= edges; i++){
			for(int j = 1; j <= edges; j++){
				if(floyd[i][j] > floyd[i][k]+floyd[k][j]){
					floyd[i][j] = floyd[i][k]+floyd[k][j]; 
					next[i][j] = next[i][k];
				}
			}
		}
	}
}

// Funcion llenar_componente, la cual dado un grafo y los identificadores de
// las componentes conexas, crea el grafo asociado en el cual cada vertice 
// corresponde a una componente conexa del grafo original.
void llenar_componentes(vector< vector< int > > &grafo, vector< int > &cc){
	for(int i = 0; i < cc.size(); i++){
		if(cc[i] != -1){
			grafo[cc[i]].push_back(i);
		}
	}

}

// Funcion llenar_lados, esta se encarga de retornar una lista de lados compuesta
// por los caminos de costo minimo entre cada par de componentes conexas.
void llenar_lados(vector< vector< int > > &floyd, vector< vector< int > > &grafoCc, 
	              vector< pair< int,pair<int,int> > > &lados,vector< vector< pair<int,int> > > grafo){
	lados.clear();
	int min;
	pair<int,int> lado;
	for(int k = 0; k < grafoCc.size(); k++){
		for(int i = k+1; i < grafoCc.size(); i++){
			min = 100000000;
			for(int l = 0; l < grafoCc[k].size(); l++){
				for(int j = 0; j < grafoCc[i].size(); j++){
					if(floyd[grafoCc[k][l]][grafoCc[i][j]] < min){
						min = floyd[grafoCc[k][l]][grafoCc[i][j]];
						lado = mp(grafoCc[k][l],grafoCc[i][j]);
					}
					else if (floyd[grafoCc[k][l]][grafoCc[i][j]] == min) {
						if (grafo[grafoCc[k][l]][grafoCc[i][j]].ss > grafo[lado.ff][lado.ss].ss) {
							lado = mp(grafoCc[k][l],grafoCc[i][j]);
						}	
					}
				}
			}
		lados.push_back(mp(min,lado));
		}
	}
}

// Funcion dfs, la cual se encarga de recorrer el grafo a profundidad para
// obtener las componentes conexas del mismo, se apoya de un booleano para
// identificar nodos inexistentes o componentes conexas ficticias.
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

// Funcion comp_con, la cual se encarga de hacer dfs desde cada nodo del grafo 
// original que aun no haya sido visitado para asi calcular todas las componentes
// conexas del mismo, retorna la cantidad de componentes del grafo.
int comp_con(vector< vector< pair<int,int> > > &grafo, vector< int > &cc, int edges){
	
	bool puede = false;
	int id = 0;
	for(int i = 0; i <= edges; i++){
		puede = false;
		for(int j = 0; j < grafo[i].size(); j++){
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

// Funcion eulerian_path, la cual toma una estructura de grafo que permita
// lados duplicados, y se encarga de calcular un ciclo euleriano del mismo
// el cual es retornado en forma de vector. 
vector<int> eulerian_path(vector< vector< vector< pair<int,int> > > > &grafo){
	vector<int> res,st,visitados;
	visitados.resize(grafo.size()+1);
	bool vacio;
	st.push_back(1);
	while(!st.empty()){
		int v = st.back();
		vacio = true;
		for(int i = 1; i < grafo[v].size(); i++){
			if(v!=i && grafo[v][i].size() >= 1){
				vacio = false;
				break;
			}
		}

		if(vacio){
			for(int i = 0; i < grafo[v].size(); i++){
				grafo[v][i].clear();
				grafo[i][v].clear();
			}
			res.push_back(v);	
			st.pop_back();
		}
		else{
			int primero;
			for(int i = 0; i < grafo[v].size(); i++){
				if(v!=i && grafo[v][i].size() >= 1){
					grafo[v][i].clear();
					grafo[i][v].clear();
					primero = i;
					break;
				}
			}
			st.push_back(primero);
		}
	}
	reverse(res.begin(),res.end());
	if(res[res.size()-1] != 1){
		res.push_back(1);
	}
	return res;
}

// Funcion eulerian_path, la cual toma una estructura de grafo que no permita
// lados duplicados, y se encarga de calcular un ciclo euleriano del mismo
// el cual es retornado en forma de vector. 
vector<int> eulerian_path_no_dup(vector< vector< pair<int,int> > > &grafo){
	vector<int> res,st;
	vector<int> visitado;
	visitado.resize(grafo.size()+1);
	bool vacio;
	st.push_back(1);
	while(!st.empty()){
		int v = st.back();
		vacio = true;
		for(int i = 0; i < grafo[v].size(); i++){
			if(v!=i && grafo[v][i].ff != -1){
				vacio = false;
				break;
			}
		}

		if(grafo[v].empty() || vacio){
			for(int i = 0; i < grafo.size(); i++){
				grafo[i][v] = mp(-1,-1);
				grafo[v][i] = mp(-1,-1);
			}
			res.push_back(v);
			st.pop_back();
		}
		else{
			int primero;
			for(int i = 0; i < grafo[v].size(); i++){
				if(v!=i && grafo[v][i].ff != -1){
					grafo[v][i] = mp(-1,-1);
					grafo[i][v] = mp(-1,-1);
					primero = i;
					break;
				}
			}
			st.push_back(primero);
		}
	}
	reverse(res.begin(),res.end());
	if(res[res.size()-1] != 1){
		res.push_back(1);
	}
	return res;
}

// Funcion calcular_beneficio, dado un ciclo factible y el grafo original, se encarga
// de calcular el beneficio total obtenido por el ciclo. 
int calcular_beneficio(vector<int> ciclo,vector< vector< pair<int,int> > > grafo) {
	int beneficio = 0;
	for(int i = 0; i < ciclo.size()-1; i++) {
		beneficio += (grafo[ciclo[i]][ciclo[i+1]].ss - grafo[ciclo[i]][ciclo[i+1]].ff);
		grafo[ciclo[i]][ciclo[i+1]].ss = 0;
		grafo[ciclo[i+1]][ciclo[i]].ss = 0; 
	}
	return beneficio;
}

// Funcion elimi_euleriana, la cual implementa el algoritmo de mejoramiento expuesto
// en el enunciado con unas leves modificaciones para tratar de obtener una mejor solucion
// al ciclo original.
void elim_euleriana(vector<int> &ciclo, vector< vector< pair<int,int> > > grafo, vector< vector< int > > &next, 
				    vector< vector< int > > &floyd){
	// Declaracion de los auxiliares que usara la funcion
	int edges, beneficio_sol,beneficio_n,comp;
	vector< vector< pair<int,int> > > grafo_sol;
	vector< vector< pair<int,int> > > grafo_aux;
	vector< vector< pair<int,int> > > grafo_aux2;
	vector< vector< pair<int,int> > > grafo_aux3;
	vector< int > cc, ciclo_aux, ciclo_aux2, camino;
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

	// Eliminacion euleriana de lados duplicados.
	// En este punto, se procede a eliminar los lados duplicados del grafo
	// solucion desde el ultimo vertice al primero con la finalidad de desconectar
	// de ultimo al deposito.
	for (int i = edges-1; i >= 0; i--) {
		for (int j = i; j >= 0; j--) {
			if (grafo_sol[i][j].ff != -1 && grafo_sol[j][i].ff != -1) {
				grafo_aux = grafo_sol;
				grafo_aux[j][i].ff = (grafo_aux[i][j].ff = -1);
				grafo_aux[j][i].ss = (grafo_aux[i][j].ss = -1);
				grafo_aux[j][j].ff = (grafo_aux[j][j].ss = 0);
				grafo_aux[i][i].ff = (grafo_aux[i][i].ss = 0);
				for(int i = 0; i < edges; i++){
					cc[i] = -1;
				}
				comp = comp_con(grafo_aux,cc,edges-1);
				if (comp == 1) {
					grafo_aux3 = grafo_aux;
					ciclo_aux = eulerian_path_no_dup(grafo_aux3);
					// Se construye el ciclo factible asegurandonos de eliminar
					// las conexiones inexistentes en el grafo original.
					ciclo_aux2.clear(); 
					ciclo_aux2.push_back(ciclo_aux[0]);
					for(int i = 0; i < ciclo_aux.size()-1; i++){
						if (grafo[ciclo_aux[i]][ciclo_aux[i+1]].ff != floyd[ciclo_aux[i]][ciclo_aux[i+1]]) {
							camino.clear();
							camino = reconstruir(next, ciclo_aux[i], ciclo_aux[i+1]);
							for (int j = 1; j < camino.size(); j++) {
								ciclo_aux2.push_back(camino[j]);
							}
						}
						else {
							ciclo_aux2.push_back(ciclo_aux[i+1]);
						}
					}
					beneficio_n = calcular_beneficio(ciclo_aux2, grafo);
					if (beneficio_n > beneficio_sol) {
						grafo_sol = grafo_aux;
						ciclo = ciclo_aux2;
						beneficio_sol = beneficio_n;
					}
				}
			}
		}
	}

	// Eliminacion euleriana de componentes conexas.
	// Luego de la eliminacion euleriana de lados, se procede a realizar la 
	// eliminacion de componentes conexas para lo cual se eliminan los lados
	// duplicados restantes en el grafo solucion y a evaluar la cantidad de
	// componentes conexas restantes en el grafo. 
	for (int i = edges-1; i >= 0; i--) {
		for (int j = i; j >= 0; j--) {
			if (grafo_sol[i][j].ff != -1 && grafo_sol[j][i].ff != -1) {
				grafo_aux = grafo_sol;
				grafo_aux[j][i].ff = (grafo_aux[i][j].ff = -1);
				grafo_aux[j][i].ss = (grafo_aux[i][j].ss = -1);
				grafo_aux[j][j].ff = (grafo_aux[j][j].ss = 0);
				grafo_aux[i][i].ff = (grafo_aux[i][i].ss = 0);

				for(int i = 0; i < edges; i++){
					cc[i] = -1;
				}
				comp = comp_con(grafo_aux,cc,edges-1);
				if(comp != 1){
					grafoCc.clear();
					grafoCc.resize(edges);
					llenar_componentes(grafoCc,cc);
					for (int i = 0; i < grafoCc[0].size(); i++) {
						for(int j = 0; j < grafoCc[0].size(); j++){
							grafo_aux2[grafoCc[0][i]][grafoCc[0][j]] = grafo_aux[grafoCc[0][i]][grafoCc[0][j]];
						}
					}
					ciclo_aux = eulerian_path_no_dup(grafo_aux2);
					// Se construye el ciclo factible asegurandonos de eliminar
					// las conexiones inexistentes en el grafo original.
					ciclo_aux2.clear(); 
					ciclo_aux2.push_back(ciclo_aux[0]);
					for(int i = 0; i < ciclo_aux.size()-1; i++){
						if (grafo[ciclo_aux[i]][ciclo_aux[i+1]].ff != floyd[ciclo_aux[i]][ciclo_aux[i+1]]) {
							camino.clear();
							camino = reconstruir(next, ciclo_aux[i], ciclo_aux[i+1]);
							for (int j = 1; j < camino.size(); j++) {
								ciclo_aux2.push_back(camino[j]);
							}
						}
						else {
							ciclo_aux2.push_back(ciclo_aux[i+1]);
						}
					}
					beneficio_n = calcular_beneficio(ciclo_aux2, grafo);
					if (beneficio_n > beneficio_sol) {
						grafo_sol = grafo_aux;
						ciclo = ciclo_aux2;
						beneficio_sol = beneficio_n;
					}
				}
			}
		}
	}
}

// Funcion eliminacion_ciclo_negativo, la cual implementa otro algoritmo de mejora
// el cual se encarga de eliminar los ciclos que aporten beneficios negativos de 
// la solucion encontrada.
vector<int> eliminacion_ciclo_negativo(vector<int> ciclo, vector< vector< pair<int,int> > > grafo){
	pair<int,int> lado,lado1,lado2;
	vector<int> ciclo_aux;
	int beneficio;
	for(int i = 1; i < ciclo.size()-1; i++){
		ciclo_aux.clear();
		lado = mp(ciclo[i],i);
		ciclo_aux.push_back(lado.ff);
		for(int j = i+1; j < ciclo.size(); j++){
			lado2 = mp(ciclo[j],j);
			ciclo_aux.push_back(lado2.ff);
			if(lado.ff == lado2.ff){
				beneficio = calcular_beneficio(ciclo_aux,grafo);
				if(beneficio <= 0){
					ciclo.erase(ciclo.begin()+lado.ss,ciclo.begin()+lado2.ss);
				}
			}
		}
	}
	return ciclo;
}

// Programa principal
int main(int argc, const char **argv){
	clock_t tStart = clock();
	vector< pair< int,pair<int,int> > > lados; // Estructura para representar los lados de un grafo.
	vector< vector< pair<int,int> > > grafo;   // Grafo original, construido durante la lectura.
	vector< vector< pair<int,int> > > grafoR;  // Grafo que representa los lados R U Q.
	vector< vector< pair<int,int> > > grafo_aux; // Grafo auxiliar.
	vector< vector< vector< pair<int,int> > > > grafoR2; // Grafo que representa los lados R U Q y soporta lados duplicados.
	vector< vector< int > > grafoI; // Grafo para almacenar vectores impares.
	vector< vector< int > > floyd;  // Matriz para realizar floyd warshall.
	vector< vector< int > > next;	// Matriz para almacenar los caminos encontrados por floyd warshall.
	vector< vector< int > > grafoCc; // Grafo para construir el grafo compuesto por las componentes conexas.
 	vector< int > cc; // Arreglo que representa la componente conexa a la que pertenece el nodo i.
	vector<int> camino, recons; // Arreglo para representar un camino reconstruido.
	vector<int> ciclo; // Arreglo para representar el ciclo obtenido.
	vector<int> ciclo_aux; // Arreglo para representar un ciclo auxiliar.
	vector<int> ciclo_elim; // Arreglo para representar el ciclo obtenido luego de la mejora.
	int v1,v2,costo,beneficio,edges,edg1,edg2,ids;
	pair<int,int> aux,aux1,aux2;
	ofstream o_file;
	ifstream file;

	string auxiliar,nombre;
	file.open(argv[1]);
	nombre = argv[1];
	file >> auxiliar;
	file >> auxiliar;
	file >> auxiliar;
	file >> auxiliar;
	file >> edges;
	file >> auxiliar;
	file >> auxiliar;
	file >> auxiliar;
	file >> auxiliar;
	file >> edg1;
	// Inicializacion de las estructuras
	grafo.resize(edges+1);
	grafoR.resize(edges+1);
	grafoR2.resize(edges+1);
	grafo_aux.resize(edges+1);
	grafoI.resize(edges+1);
	floyd.resize(edges+1);
	next.resize(edges+1);
	cc.resize(edges+1);
	
	for(int i = 0; i <= edges; i++){
		grafo[i].resize(edges+1);
		grafoR[i].resize(edges+1);
		grafoR2[i].resize(edges+1);
		grafo_aux[i].resize(edges+1);
		grafoI[i].resize(edges+1);
		floyd[i].resize(edges+1);
		next[i].resize(edges+1);
		cc[i] = -1;
	}

	for(int i = 0; i <= edges; i++){
		for(int j = 0;j <= edges; j++){
			grafo[i][j] = mp(-1,-1);
			grafoR[i][j] = mp(-1,-1);
			grafo_aux[i][j] = mp(-1,-1);
			grafoI[i][j] = -1;
		}
	}

	// Comienzo de la lectura
	while(edg1--){
		file >> v1 >> v2 >> costo >> beneficio;
		aux = mp(v1,v2);
		aux1 = mp(v2,v1);
		aux2 = mp(costo,beneficio);
		grafo[v1][v2] = aux2;
		grafo[v2][v1] = aux2;
		if(beneficio >= 2*costo){
			grafoR[v1][v2] = aux2;
			grafoR[v2][v1] = aux2;
			grafoR2[v1][v2].push_back(aux2);
			grafoR2[v2][v1].push_back(aux2);
		}
		else{
			grafoR[v1][v2] = aux2;
			grafoR[v2][v1] = aux2;

			grafoR2[v1][v2].push_back(aux2);
			grafoR2[v2][v1].push_back(aux2);
		}
	}
	file >> auxiliar;
	file >> auxiliar;
	file >> auxiliar;
	file >> auxiliar;
	file >> auxiliar;
	file >> edg2;
	
	while(edg2--){
		file >> v1 >> v2 >> costo >> beneficio;
		aux = mp(v1,v2);
		aux1 = mp(v2,v1);
		aux2 = mp(costo,beneficio);
		grafo[v1][v2] = aux2;
		grafo[v2][v1] = aux2;
	}
	file.close();
	// Inicializacion delas matrices floyd y next las cuales
	// requieren del grafo ya construido.
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

	// Se calculan las componentes conexas del grafo y se
	// añade el deposito en caso de no estar.
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
	// Se realiza el floyd warshall asociado al grafo.
	floyd_warshall(floyd,next,edges);
	// En caso de tener un grafo desconectado, se procede a construir
	// el grafo en el cual cada nodo es una componente conexa, luego se
	// calcula el mst asociado a ese grafo y se añaden todos los lados
	// reconstruidos de los caminos de costo minimo en el grafo.
	if(ids != 1){
		grafoCc.resize(ids);
		llenar_componentes(grafoCc,cc);
		llenar_lados(floyd,grafoCc,lados,grafo);
		lados = kruskal(lados,cc,ids);
		for(int i = 0; i < lados.size(); i++){
			recons.clear();
			recons = reconstruir(next,lados[i].ss.ff,lados[i].ss.ss);
			for(int j = 0; j < recons.size()-1; j++){
				grafo_aux[recons[j]][recons[j+1]] = mp(floyd[recons[j]][recons[j+1]],-1);
				grafo_aux[recons[j+1]][recons[j]] = mp(floyd[recons[j+1]][recons[j]],-1);
			}
		}
		for(int i = 0; i < grafo_aux.size(); i++){
			for(int j = 0; j < grafo_aux[i].size(); j++){
				if(grafo_aux[i][j].ff !=-1){
					grafoR2[i][j].push_back(grafo_aux[i][j]);
				}
			}
		}
	}
	// Se verifica la paridad del grafo, en caso de no ser par, se calcula
	// el minimo apareamiento perfecto del grafo para poder volverlo par y se
	// añaden esos lados al grafo.
	if(!es_par(grafoR2)){
		gen_grafo_impar(grafoR2,grafoI,floyd);
		perfect_matching(grafoI,lados);
		for(int i = 0; i < lados.size(); i++){
			recons.clear();
			grafoR[lados[i].ss.ff][lados[i].ss.ss] = mp(lados[i].ff,-1);
			grafoR[lados[i].ss.ss][lados[i].ss.ff] = mp(lados[i].ff,-1);
			recons = reconstruir(next,lados[i].ss.ff,lados[i].ss.ss);
			for(int j = 0; j < recons.size()-1; j++){
				grafoR2[recons[j]][recons[j+1]].push_back(mp(floyd[recons[j]][recons[j+1]],-1));
				grafoR2[recons[j+1]][recons[j]].push_back(mp(floyd[recons[j+1]][recons[j]],-1));
			}
		}
	}
	// Se calcula un primer ciclo solucion al grafo y se reconstruye con
	// la finalidad de que para cada par de nodos exista un camino en el
	// grafo original.
	ciclo = eulerian_path(grafoR2);

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
	// Se procede a realizar la eliminacion euleriana para tratar de mejorar
	// la primera solucion obtenida.
	elim_euleriana(ciclo_aux, grafo, next, floyd);
	// Se eliminan los ciclos de costo negativo y se observa si se mejoro la 
	// solucion, en caso positivo se sustituye el resultado y el ciclo por
	// los obtenidos de la mejora y se imprimen. 
	ciclo_elim =eliminacion_ciclo_negativo(ciclo_aux,grafo);
	int resultado = calcular_beneficio(ciclo_aux, grafo);
	int resultado_elim = calcular_beneficio(ciclo_elim,grafo);
	if(resultado < resultado_elim){
		resultado = resultado_elim;
		ciclo_aux = ciclo_elim;
	}
	o_file.open(nombre.append("-salida.txt").c_str());
	o_file << resultado << endl;
	for(int i = 0; i < ciclo_aux.size(); i++){
		if(ciclo_aux[i] == 1){
			o_file << "d" << " ";
		}
		else{
			o_file << ciclo_aux[i]-1 << " ";
		}
	}
	o_file << endl;
	o_file.close();
	//printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
}
		
	

