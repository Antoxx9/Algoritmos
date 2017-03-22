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
#define Inf 1000000000

using namespace std;

vector< vector< pair<int,int> > > grafo;   // Grafo original, construido durante la lectura.

class vertice {
	public:
		int id;
		int costo;
		bool visitado;
		vertice* predecesor;
		
		vertice(){}; 
		
		vertice(int ident, int cst, bool vis, vertice* pred): id(ident), costo(cst), visitado(vis), 
															  predecesor(pred){};
		
		bool operator<(vertice const& v) const{
			return this->costo > v.costo;
		}

};

pair <vector<int>,int> camino_costo_min(int n_edges, int i, int b) {
	priority_queue <vertice, vector<vertice> > cola;
	vertice vertices[n_edges + 1];
	vector<int> camino;
	int costo_cm;

	for(int j = 1; j <= n_edges; j++){
		vertices[j].id = j;
		vertices[j].costo = Inf;
		vertices[j].visitado = false;
		vertices[j].predecesor = NULL;
		if (j == i) {
			vertices[j].costo = 0;
		}
	}
	cola.push(vertices[i]);
	while(!cola.empty()){
		vertice v_aux = cola.top();
		//cout << "Pop: " << v_aux.id << endl;
		vertices[v_aux.id].visitado = true;
		for (int k = 1; k <= n_edges; k++) {
			if (grafo[v_aux.id][k].ss != -1 && k != v_aux.id) {
				if ((grafo[v_aux.id][k].ss - grafo[v_aux.id][k].ff) >= 0) {
					if (!vertices[k].visitado && vertices[k].costo > vertices[v_aux.id].costo) {
						vertices[k].costo = vertices[v_aux.id].costo;
						vertices[k].predecesor = &vertices[v_aux.id];
						//cout << "Push: " << vertices[k].id << endl;
						cola.push(vertices[k]);
					}
				}
				else {
					if (!vertices[k].visitado && vertices[k].costo > vertices[v_aux.id].costo + 
																	 grafo[v_aux.id][k].ff - 
																	 grafo[v_aux.id][k].ss) {
						vertices[k].costo = vertices[v_aux.id].costo + grafo[v_aux.id][k].ff 
											- grafo[v_aux.id][k].ss;
						vertices[k].predecesor = &vertices[v_aux.id];
						//cout << vertices[k].predecesor->id << endl;
						//cout << "Push: " << vertices[k].id << endl;
						cola.push(vertices[k]);
					}
				}
			}
		}
		cola.pop(); 
	}
	costo_cm = 0;
	while (b != i) {
		//cout << vertices[b].id << " ";
		camino.push_back(b);
		costo_cm += vertices[b].costo;
		b = vertices[b].predecesor->id; 
	}
	camino.push_back(b);
	//cout << vertices[b].id << " " << endl;
	return mp(camino, costo_cm);
}

//Aun no es probabilistico
pair <int,int> obtener_lado(vector< pair<int,int> > T, int b) {
	int max_benf = -1;
	pair <int,int> lado = mp(-1,-1);
	for (int i = 0; i < T.size(); i++) {
		if (T[i].ff == b || T[i].ss == b) {
			if ((grafo[T[i].ff][T[i].ff].ss - grafo[T[i].ff][T[i].ff].ff) > max_benf) {
					max_benf = grafo[T[i].ff][T[i].ff].ss - grafo[T[i].ff][T[i].ff].ff;
					lado = T[i];
			}
		}
	}
	if (lado.ss == b) {
		lado = mp(lado.ss,lado.ff);
	}
	return lado;
}

//Aun no es probabilistico
vector<int> obtener_camino(vector<pair <vector<int>,int> > caminos_cm) {
	vector<int> min_camino;
	int min_costo = Inf;
	//cout << "Cuantos caminos hay: " << caminos_cm.size() << endl;
	for (int i = 0; i < caminos_cm.size(); i++) {
		/*
		for (int g = 0; g < caminos_cm[i].ff.size(); g++) {
			cout << caminos_cm[i].ff[g] << " ";
		}
		cout << endl;	
		*/
		//cout << "REALLY BITCH" << endl;
		if (caminos_cm[i].ss < min_costo) {
			//cout << "REALLY BITCH 2" << endl;
			min_costo = caminos_cm[i].ss;
			min_camino = caminos_cm[i].ff;
		}
	}
	//cout << "Size del camino minimo: " << min_camino.size() << endl;
	return min_camino;
}

// Programa principal
int main (int argc, const char **argv){
	clock_t tStart = clock();
	vector< pair< int,pair<int,int> > > lados; // Estructura para representar los lados de un grafo.
	vector< vector< int > > floyd;  // Matriz para realizar floyd warshall.
	vector< vector< int > > next;	// Matriz para almacenar los caminos encontrados por floyd warshall.
	vector<int> camino, recons; // Arreglo para representar un camino reconstruido.
	vector< pair<int,int> > ciclo;
	vector< pair<int,int> > T;
	pair<int,int> lado, lado_inv;
	int n_edges, n_edges_req, n_edges_nonreq, v1, v2, beneficio, costo, beneficio_total, b;
	bool is_deposit;
	ofstream o_file;
	ifstream file;
	string auxiliar,nombre;

	file.open(argv[1]);
	nombre = argv[1];
	file >> auxiliar >> auxiliar >> auxiliar >> auxiliar;
	file >> n_edges;
	file >> auxiliar >> auxiliar >> auxiliar >> auxiliar;
	file >> n_edges_req;

	// Inicializacion de las estructuras
	grafo.resize(n_edges + 1);
	floyd.resize(n_edges + 1);
	next.resize(n_edges + 1);
	
	for(int i = 0; i <= n_edges; i++){
		grafo[i].resize(n_edges + 1);
		floyd[i].resize(n_edges + 1);
		next[i].resize(n_edges + 1);
	}

	for(int i = 0; i <= n_edges; i++){
		for(int j = 0;j <= n_edges; j++){
			grafo[i][j] = mp(-1,-1);
		}
	}

	// Comienzo de la lectura
	is_deposit = false;
	while(n_edges_req--){
		file >> v1 >> v2 >> costo >> beneficio;
		T.push_back(mp(v1,v2));
		grafo[v2][v1] = (grafo[v1][v2] = mp(costo,beneficio));
		if (v1 == 1 || v2 == 1) {
			is_deposit = true;
		}
	}
	file >> auxiliar >> auxiliar >> auxiliar >> auxiliar >> auxiliar;
	file >> n_edges_nonreq;
	
	while(n_edges_nonreq--){
		file >> v1 >> v2 >> costo >> beneficio;
		grafo[v2][v1] = (grafo[v1][v2] = mp(costo,beneficio));
	}
	file.close();
	
	// Impresion de revision
	/*
	for(int i = 1; i <= n_edges; i++){
		for(int j = 1;j <= n_edges; j++){
			cout << "(" << grafo[i][j].ff << "," << grafo[i][j].ss << ") ";
		}
		cout << endl;
	}
	*/
	/*
	cout << "Lados R U Q: ";
	for(int i = 0; i < T.size(); i++) {
		  cout << "(" << T[i].ff << "," << T[i].ss << ") ";
	}
	cout << endl;
	*/
	/*
	cout << "Esta el deposito? " << is_deposit << endl;
	// Aqui acaba la impresion de revision.
	*/
	beneficio_total = 0;
	b = 1;
	if (!is_deposit) {
		int max_benf = -Inf;
		int max_i = 0;
		for (int i = 1; i <= n_edges; i++) {
			if (grafo[1][i].ss != -1) {
				if ((grafo[1][i].ss - grafo[1][i].ff) > max_benf) {
					max_benf = grafo[1][i].ss - grafo[1][i].ff;
					max_i = i;
				}
			} 
		}
		ciclo.push_back(mp(1,max_i));
		beneficio_total += grafo[1][max_i].ss - grafo[1][max_i].ff;
		grafo[max_i][1].ss = (grafo[1][max_i].ss = 0);
		b = max_i;
	}
	vector<pair <vector<int>,int> > caminos_cm;
	vector<int> min_camino;
	while (!T.empty()) {
		lado = obtener_lado(T,b);
		if (lado.ss != -1) {
			lado_inv = mp(lado.ss,lado.ff);
			for (int i = 0; i < T.size(); i++) {
				if (T[i] == lado || T[i] == lado_inv) {
					T.erase(T.begin() + i);
					break;
				}
			}
			ciclo.push_back(lado);
			beneficio_total += grafo[lado.ff][lado.ss].ss - grafo[lado.ff][lado.ss].ff;
			grafo[lado.ss][lado.ff].ss = (grafo[lado.ff][lado.ss].ss = 0);
			b = lado.ss;
			//cout << "Beneficio: " << beneficio_total << endl;
		}
		else {
			//cout << "Buscando caminos..." << endl;
			caminos_cm.clear();
			for (int i = 0; i < T.size(); i++) {
				caminos_cm.push_back(camino_costo_min(n_edges,T[i].ff,b));
				caminos_cm.push_back(camino_costo_min(n_edges,T[i].ss,b));	
			}
			//cout << "Caminos computados." << endl;
			min_camino = obtener_camino(caminos_cm);
			//cout << "Camino minimo obtenido." << endl;
			//cout << "Size del camino minimo: " << min_camino.size() << endl;
			for (int j = 0; j < min_camino.size() - 1; j++) {
				//cout << "Incluyendo camino en el ciclo: " << j << endl;
				lado = mp(min_camino[j], min_camino[j+1]);
				lado_inv = mp(lado.ss,lado.ff);
				ciclo.push_back(lado);
				for (int k = 0; k < T.size(); k++) {
					//cout << "Eliminando lados T: " << k << endl;
					if (T[k] == lado || T[k] == lado_inv) {
						T.erase(T.begin() + k);
						break;
					}
				}		
				beneficio_total += grafo[lado.ff][lado.ss].ss - grafo[lado.ff][lado.ss].ff;
				grafo[lado.ss][lado.ff].ss = (grafo[lado.ff][lado.ss].ss = 0);
				b = lado.ss;
				//cout << "Beneficio: " << beneficio_total << endl;
			}
			//cout << "Camino construido - ";
		}
		/*
		cout << "Ciclo: ";
		for(int i = 0; i < ciclo.size(); i++) {
			cout << "(" << ciclo[i].ff << "," << ciclo[i].ss << ") ";
		}
		cout << endl;
		*/
	}
	if (b != 1) {
		min_camino = (camino_costo_min(n_edges,1,b)).ff;
		for (int j = 0; j < min_camino.size() - 1; j++) {
			lado = mp(min_camino[j], min_camino[j+1]);
			ciclo.push_back(lado);		
			beneficio_total += grafo[lado.ff][lado.ss].ss - grafo[lado.ff][lado.ss].ff;
			grafo[lado.ss][lado.ff].ss = (grafo[lado.ff][lado.ss].ss = 0);
			b = lado.ss;
		}
	}
	cout << beneficio_total << endl;
	for(int i = 0; i < ciclo.size(); i++) {
		cout << "(" << ciclo[i].ff << "," << ciclo[i].ss << ") ";
	}
	cout << endl;
}