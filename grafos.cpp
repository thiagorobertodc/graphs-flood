#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

typedef struct { //https://www.ime.usp.br/~pf/algoritmos_para_grafos/aulas/graphdatastructs.html
	int V;
	int A;
	int **adj;
	int passos;
} Grafo;

typedef struct {
	int cor;
	bool inundado;
	bool flag;
	bool colorido;
}Cores;

void verifica(Cores* C, int v);
int ColoreVertice(Grafo* G, int inicial, Cores* C, int pivo, vector<int>&);
int ReconhecimentoMonocromatico(Grafo* G, Cores* C, vector<int>& inundados, int pivo);
void FrequenciaCores(Grafo* G, Cores* C, vector<int>& inundados, vector<int>& adjacentes, int vet[]);
int InundaGrafo(Grafo* G, int pivo, Cores* C, vector<int>&, vector<int>&, int colorNumber);
void GRAFOInsereArco(Grafo* G, int v, int w);
Grafo GRAFOInit(int v);
static int ** MATRIZADJint(int linhas, const int colunas, int valor);

int main() {
	int v, a = 0, colorNumber = 0, pivo = 0;
	vector <int> inundados;
	vector<int> adjacentes;
	cin >> v;
	cin.clear();
	cin.ignore(); 
	Grafo G = GRAFOInit(v);

	cin >> a >> colorNumber >> pivo;
	cin.clear();
	cin.ignore();
	inundados.push_back(pivo);

	Cores* cores = new Cores[v];
	for (int i = 1; i <= v; i++) {
		cin >> cores[i].cor;
		cin.clear();
		cin.ignore();
		if (i == pivo) {
			cores[i].inundado = true;
			cores[i].flag = true;
			cores[i].colorido = true;
		}
		else {
			cores[i].inundado = false;
			cores[i].flag = false;
			cores[i].colorido = false;
		}
	}

	for (int i = 0; i < a; i++){
		int x, y;
		cin >> x;
		cin >> y;
		cin.clear();
		cin.ignore();
		GRAFOInsereArco(&G, x, y);
	}
	//verifica(cores, v);
	duration<double> time_span2;
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	InundaGrafo(&G, pivo, cores, inundados, adjacentes, colorNumber);
	G.passos--;
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	duration<double> time_span = duration_cast<duration<double> >(t2 - t1);
	delete [] G.adj[0];
	delete [] G.adj;
	cout << "\nNUMERO DE PASSOS: " << G.passos << " Tempo (em segundos): " << time_span.count() << endl; 
	//verifica(cores, v);
		return 0;
}

static int ** MATRIZADJint(int linhas, const int colunas, int valor) {
	int ** M = NULL; // linha 84 até 89: slide 111 da aula de introdução de POO; http://www.decom.ufop.br/guillermo/BCC221/slides/IntroC_____POO.pdf
	M = new int *[linhas];
	M[0] = new int[linhas * colunas];
	for (int i = 0; i < linhas; i++)
		M[i] = &M[0][i*colunas];

	for (int i = 0; i < linhas; i++)
		for (int j = 0; j < linhas; j++)
			M[i][j] = valor;
	return M;
}

Grafo GRAFOInit(int v) { // https://www.ime.usp.br/~pf/algoritmos_para_grafos/aulas/graphdatastructs.html
	Grafo G;
	G.V = v;
	G.A = 0;
	G.passos = 0;
	G.adj = MATRIZADJint(v, v, 0);
	return G;
}

void GRAFOInsereArco(Grafo* G, int v, int w) { // https://www.ime.usp.br/~pf/algoritmos_para_grafos/aulas/graphdatastructs.html
	if (G->adj[v - 1][w - 1] == 0) { 
		G->adj[v - 1][w - 1] = 1; 
		G->adj[w - 1][v - 1] = 1; 
		G->A++; 
	}
}

int InundaGrafo(Grafo* G, int pivo, Cores* C, vector<int> &inundados, vector <int> &adjacentes, int colorNumber) {
		int vet[colorNumber];
		int freq = 0, cormaisfreq = 0;
		for (int i = 1; i <= colorNumber; i++){
			vet[i] = 0;
		}
			

		ReconhecimentoMonocromatico(G, C, inundados, pivo);
		//cout << "oi" << endl;
		FrequenciaCores(G, C, inundados, adjacentes, vet);
		//cout << "oi" << endl;
	
		for (int i = 1; i <= colorNumber; i++) {
			if (vet[i] > freq) {
				freq = vet[i];
				cormaisfreq = i;
			}
		}

		for (int i = 0; i < adjacentes.size(); i++) {
			if (C[adjacentes[i]].cor == cormaisfreq) {
				ColoreVertice(G, adjacentes[i], C, pivo, inundados);
			}
		}

		G->passos++;

		for (int i = 1; i <= G->V; i++)
			C[i].flag = false; 
		adjacentes.clear(); 

		if (inundados.size() != G->V)
			InundaGrafo(G, pivo, C, inundados, adjacentes, colorNumber);

		return 0;
}



int ColoreVertice(Grafo* G, int inicial, Cores* C, int pivo, vector<int> &inundados) {
	for (int i = 0; i<G->V; i++) {
		if ((G->adj[inicial - 1][i] == 1) && (C[i + 1].cor == C[inicial].cor)) {
			if (!C[i + 1].colorido) {
				if (C[inicial].colorido)
					ColoreVertice(G, i + 1, C, pivo, inundados);
				else {
					C[inicial].colorido = true;
					ColoreVertice(G, i + 1, C, pivo, inundados);
				}
					
			}

		}
	}
	C[inicial].cor = C[pivo].cor;
	return 1;
}



void verifica(Cores* C, int v) {
	cout << endl;
	for (int i = 1; i <= v; i++) {
		cout << "Cor do vertice " << i << ": " << C[i].cor << endl;
	}
}

int ReconhecimentoMonocromatico(Grafo* G, Cores* C, vector<int>& inundados, int pivo) {
	for (int i = 0; i<G->V; i++) {  
		for (int j = 0; j < inundados.size(); j++) {
			if ((G->adj[i][inundados[j] - 1] == 1) && (C[i + 1].inundado == false)) {
				if (C[i + 1].cor == C[pivo].cor) {
					C[i + 1].inundado = true;
					inundados.push_back(i + 1);
					ReconhecimentoMonocromatico(G, C, inundados, pivo);
				}
			}
		}
	}
	return 1;
}
	

void FrequenciaCores(Grafo* G, Cores* C, vector<int>& inundados, vector<int>& adjacentes, int vet[]) {
	for (int i = 0; i < G->V; i++) {
		for (int j = 0; j < inundados.size(); j++) {
			if ((G->adj[i][inundados[j] - 1] == 1) && (C[i + 1].inundado == false)) {
				if (!C[i + 1].flag) {
					vet[C[i + 1].cor] += 1;
					adjacentes.push_back(i + 1);
					C[i + 1].flag = true;
				}
			}
		}
	}
}