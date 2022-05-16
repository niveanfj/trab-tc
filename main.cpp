#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

class estado{

    estado *proximo;
    char letra;
    int estadoTransicao;

public:

    estado(char letra, int estadoTransicao, estado *proximo= nullptr) : proximo(proximo), letra(letra), estadoTransicao(estadoTransicao) {}

    char getLetra(){
        return letra;
    }
    int getTransicao(){
        return estadoTransicao;
    }
    estado *getProximo(){
        return proximo;
    }
    void setProximo(estado *proximo){
        this -> proximo = proximo;
    }

};


class transicao{
    estado *primeiro;
    int tamanho;
    bool estadoFinal;
    static vector<char> imp;

public:

    transicao(int estadoFinal): primeiro(nullptr), tamanho(0), estadoFinal(estadoFinal) {}

    void insere(char letra, int estadoTransicao){
        //criar um estado
        estado *novoestado = new estado(letra, estadoTransicao);
        // primeiro estado vazio
        if (primeiro == nullptr){
            primeiro = novoestado;
        }else{
            estado *atual = primeiro;
            // vai para o final da lista de transição
            while (atual -> getProximo() != nullptr){
                atual = atual -> getProximo();
            }
            atual -> setProximo(novoestado);
        }
        tamanho++;
    }

    int getTamanho(){return tamanho;}

    vector<int> vazio()
    {
    	vector<int> vazio;

    	estado *atual = primeiro;
    	while (atual != nullptr){
    		if(atual -> getLetra() == '&')
    			vazio.push_back(atual->getTransicao());

    		atual = atual -> getProximo();
        }

    	return vazio;

    }

    static void setImp(int i, char letra, int pos, bool ini)
    {
    	imp.push_back(i+'0');
    	imp.push_back(letra);
    	imp.push_back(pos+'0');

    	if (ini == true) //mudar pro inicio do vetor
    	{
    		std::rotate(imp.rbegin(), imp.rbegin() + 1, imp.rend());
    		std::rotate(imp.rbegin(), imp.rbegin() + 1, imp.rend());
    		std::rotate(imp.rbegin(), imp.rbegin() + 1, imp.rend());
    	}

    	return;

    }

    static void imprime_NFA()
    {
    	for(int i = 0; i < imp.size(); i = i+3)
    		cout << imp[i] << " -> " << imp[i+1] << " -> " << imp[i+2] << endl;

    	imp.clear();
    	return;
    }

    static void limpar_NFA(){imp.clear();}

    int buscar(char letra){
        if (primeiro == nullptr){
            return -1;
        }else{
            estado *atual = primeiro;
            while (atual != nullptr){
                if(atual -> getLetra() == letra){
                    return atual->getTransicao();
                }
                atual = atual -> getProximo();
            }
            return -1; //Caso não encontre
        }
    }

    bool getEstadoFinal(){
        return estadoFinal;
    }

    estado* getPrimeiro(){return primeiro;}

    void imprimir(){
       //cout << "tabela transicao:" << endl;
       //cout << "estado: "<< endl;
        estado *atual = primeiro;
        while(atual != nullptr){
            cout << atual -> getLetra() << " ";
            atual = atual ->getProximo();
        }
        cout << endl;
    }

    ~transicao(){
        estado *atual = primeiro;
        while (atual != nullptr){
            primeiro = primeiro -> getProximo();
            tamanho --;
            delete atual;
            atual = primeiro;
        }
    }

};

vector<char> transicao::imp;


int func_char(transicao* pos , char letra)		//Função para determinar se a letra existe na lista do estado
{
	int aux = pos->buscar(letra);

	if(aux != -1)
		return aux;
	else
	{
		if(pos->buscar('&')!= -1)			//Caso a letra não exista ele procura pela transição vazia
			return -1;
		else
			return -2;
	}
}

bool busca_no_NFA_char(vector<transicao*> lista, string w, int i, int j, int k)
{
	vector<int> div_estados;

		while(j < w.size())	//Percorrer toda a palavra
		{

			int aux = func_char(lista[i], w[j]);
			if(aux >= 0)		//Caso a letra exista naquele estado
			{
				transicao::setImp(i, w[j], aux, false);
				i = aux;		//Vai para o proximo estado
				j++;			//E para a proxima letra da palavra
			}
			else if(aux == -1)			//Caso tenha a transição vazia mas não a letra
			{
				div_estados = lista[i]->vazio();

				while(k < div_estados.size())
				{
					//cout << div_estados[k] << endl;
					if((busca_no_NFA_char(lista, w, div_estados[k], j, k)) == true)
					{
						transicao::setImp(i, '&', div_estados[k], true);
						return true;
					}
					k++;
				}
				return false;
			}
			else if(aux == -2)						//Caso não exista nem letra nem transição vazia
				return false;
		}

		int verifica = func_char(lista[i], '&');
		if((j >= w.size()) && (verifica > 0))
		{
			transicao::setImp(i, '&', verifica, false);
			i = verifica;
		}

		if (lista[i]->getEstadoFinal() == true)		//Verifica se o estado que parou é final ou não
			return true;
		else
		{
			//transicao::limpar_NFA();
			return false;
		}
}

int main(){

    string alfabeto, letraTrasicao, w;
    vector <transicao*> lista;
    vector <int> estadosfinais;
    vector <string> W;
    int estados, A, F, X, Y;
    bool saida = false;

    //cout << "Tamanho do alfabeto: " << endl;
    cin >> A;

    //cout << "Alfabeto: " << endl;
    cin >> alfabeto;

    //cout << "Numero de estados: " << endl;
    cin >> estados;

    //cout << "Numeros de estados finais: " << endl;
    cin >> F;

    //Insere os estados finais em um vetor
    //cout << "Estados finais: " << endl;
    for (int i = 0; i < F; i++){
        int aux;
        cin >> aux;
        estadosfinais.push_back(aux);
    }

    //Inserir elemento na lista
    for (int i = 0; i < estados; i++)
    {
    	bool aux = false;

    	for(int j = 0; j < estadosfinais.size(); j++)	//Percorrer o vetor de estados finais
    		if(estadosfinais[j] == i)					// Pra ver se o estado atual(i) corresponde a um final
    			aux = true;

    	if (aux == true)								//Marca se for final ou não
    		lista.push_back(new transicao(true));
    	else
    		lista.push_back(new transicao(false));
    }

    //Entrada da função de transição
    //cout << "Função de transicao formato (X sring Y): " << endl;
    do{
        cin >> X >> letraTrasicao >> Y;
        if ((X == -1) && (letraTrasicao == "*") && (Y == -1)){
            saida = true;
        }else{
            for(int i = 0; i < letraTrasicao.length(); i++){
                lista[X]->insere(letraTrasicao[i], Y);
            }
        }

    }while(saida != true);


    //Imprime os estados e relações existentes
    printf("\n--Tabela de transicao --\n");
    for(int i= 0; i < estados; i++){
    	cout << "Estado: " << i << endl;
        lista[i]->imprimir();
    }/*

    //Mostrar quais são os estados finais.
    printf("\nEstados Finais:\n");
    for (int i = 0; i < estados; i++){
     	if(lista[i]->getEstadoFinal() == true)
       		cout << i << ": true" << endl;
       	else
       		cout << i << ": false" << endl;
    }*/

    //Inserir em w as palavras para busca
    //cout << "Informe as palavras que deseja buscar no NFA:" << endl;
    do{
        saida = false;
        cin >> w;

        if (w == "*") {
            saida = true;
        }else{
            W.push_back(w);
        }
    }while(saida != true);

    //Tenta percorrer o NFA com as palavras de W
    for(int i = 0; i < W.size(); i++){
    	if(busca_no_NFA_char(lista, W[i], 0, 0, 0) == true)
    	{
    		cout << W[i] << ": sim" << endl;
    		transicao::imprime_NFA();
    	}
    	else
    	{
    		cout << W[i] << ": nao" << endl;
    		transicao::imprime_NFA();
    		transicao::limpar_NFA();
    	}

    	printf("\n");
    }

    return 0;
}
