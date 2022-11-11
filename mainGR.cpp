#include "funciones.h"
#include <fstream>
#include <time.h>
#include <vector>
#include <string>
#include <map>

using namespace std;

double TIMELIM;
double CURRENTTIME=0;
void fillMatrix(vector<vector<char>> &vec, fstream &file,int fila,int columna)
{
	vector<char> aux;
	int flag = 0;
	string trash;
	for (int i = 0; i < fila; ++i)
	{
		vec.push_back(aux);
		for (int j = 0; j < columna+2; ++j)
		{
			if (flag < columna)
			{
				vec.at(i).push_back(file.get());
				flag++;
			}
			else
			{
				trash = file.get();
				flag++;
				if (flag == columna+2)
					flag = 0;
			}
		}
	}
}
void printMatrix(vector<vector<char>> &vec)
{
	for (int i = 0; i < vec.size(); ++i)
	{
		for (int j = 0; j < vec.at(i).size(); ++j)
		{
			cout << vec.at(i).at(j);
		}
		cout << endl;
		// cout<<vec.at(i).size();
	}
}

void record_pos_dif(vector<char>& inst, int pos, vector<char>& solucion, vector<int>& char_dif_x_fila)
{
	// cout<<" "<<inst.size()<<" "<< solucion.size()<<" ";
	for (int i = 0; i < inst.size(); i++){
		if (inst.at(i) != solucion.at(pos)) char_dif_x_fila.at(i) += 1;
	}
}

int notificar(vector<int> &sol_posRecord, int limite,int columnas, int filas){ // sol_posrecord size es 100
	int total = 0; //cuantas filas cumplen con el threshold
	// cout<< "compare_th: "<<compare_th<<endl;
	for (int i = 0; i < sol_posRecord.size(); i++){
		//cout<<"FILA: "<<i<< " valor: "<<sol_posRecord.at(i)<<" ";
		if (sol_posRecord[i] >= limite ) total += 1;
	}
	cout<<total<<" de "<< filas <<"strings cumplen con el threshold"<<endl;
	return total;
}

void firstchoice(vector<int> &rep, vector<char> &sol, vector<int> &n_char_sol){//repeticiones por letra de alfabeto en una columna, letra escogida , cantidad de esa escogida
	int save = 0;
	for (int i = 0; i < rep.size() - 1; i++){
		if (rep.at(i) > rep.at(i + 1))
			save = i + 1;
	}
	n_char_sol.push_back(rep.at(save));
	switch (save)
	{
	case 0:
		sol.push_back('A');
		break;
	case 1:
		sol.push_back('C');
		break;
	case 2:
		sol.push_back('G');
		break;
	case 3:
		sol.push_back('T');
		break;
	}

}



//resuelve un archivo de texto. una instancia.
cromosoma greedy (float threshold,vector<vector<char>>& mat, vector<char>& vj,vector<int>& reps_j,vector<char>& alf,vector<char>& sol,vector <int>& n_char_sol,int filas,int columnas){
    
	random_device random; // genera un dado que se tira aleatoriamente
	mt19937 engine{random()}; // se planta una semilla https://cpppatterns.com/patterns/choose-random-element.html 
	uniform_real_distribution<> prob(0,0.1);// que genera una distribucion real desde 0 a 0.1
	
	float e = 0.1;//
	sol.clear(); //se limpia el vector solucion
    n_char_sol.clear(); 
	vector<int> solRep(filas, 0);// cuantos chars difieren de la solucion en una fila 
	float lim1 = static_cast<float>(mat.at(0).size())*threshold;
	int lim = static_cast<int>(lim1); //threshold traducido a cantidad de caracteres 
	map<char,int> repeticiones;// mapa para saber cuanto se repiten los caracteres del alfabeto, se actualiza x columna
    for (int j = 0; j < lim; j++){ // x columna
		vj.clear();
		reps_j.clear();
		repeticiones['A'] = 0;
		repeticiones['C'] = 0;
		repeticiones['G'] = 0;
		repeticiones['T'] = 0;
		for (int i = 0; i < mat.size(); i++){//
			vj.push_back(mat.at(i).at(j)); //lleno la columna actual 
		}
		for(int k = 0; k < vj.size();k++){
				repeticiones[vj.at(k)]++;
		}
		reps_j.push_back(repeticiones['A']);
		reps_j.push_back(repeticiones['C']);
		reps_j.push_back(repeticiones['G']);
		reps_j.push_back(repeticiones['T']);
		
		firstchoice(reps_j,sol,n_char_sol);	
		record_pos_dif(vj,j,sol,solRep);
	}
	

	for (int j = lim; j < mat.at(0).size(); j++){
		repeticiones['A'] = 0; //reciclamos el mapa repeticiones a un mapa de puntajes por 
		repeticiones['C'] = 0;// letra a medida de que se revise una columna
		repeticiones['G'] = 0;
		repeticiones['T'] = 0;
		for (int i = 0; i < filas; i++){
			for (map<char,int>::iterator it= repeticiones.begin();  it != repeticiones.end(); it++){//taamaño del alfabeto
				int aux = 0;
				if (it->first != mat.at(i).at(j)){
					it->second++;
					aux = solRep[i];
					aux++;
				}
				if(aux >= lim) it->second++;
			}//termina for de 4 pasadas	
		}//termino for de filas
		//------------------------------- se genera gen random ------------------------------------
		float p = prob(engine);
		uniform_int_distribution<int> dist(0,alf.size()-1); //se usa otra funcion con el engine para escoger una posicion aleatoria
		
		if(p<e){
			sol.push_back(alf[dist(engine)]); // se llama dist(engine) para agregar la posicion aleatoria escogida
		}else{
			auto pr = max_element(repeticiones.begin(),repeticiones.end(),[](const auto &x, const auto &y){
				return x.second < y.second; // funcion para comṕarar valores del mapa
			});
			//cout<<"letra escogida para iteracion :"<< j <<" "<<pr->first<<" puntaje: "<<pr->second<<endl;
			sol.push_back(pr->first);
		}
	}
	int fitness = notificar(solRep,lim,columnas,filas); //calcula fitness 
	cromosoma aux = {sol,fitness};
    //notificar(solRep,lim,columnas,filas);
	return aux;

}

int main(int argc, char *argv[])
{
	// para ejecutar ->  ./a.out -i 100-300 -t 80 -th 0.75 -pi 10 (poblacion inicial greedy)
    srand(time(NULL));
    string s1=argv[1];
    string s2=argv[2];
    string s3=argv[3];
    string s4=argv[4];
	string s5=argv[5];
    string s6=argv[6];
	string s7=argv[7];
	string s8=argv[8];
    char str[20];
    strcpy(str,s4.c_str());
    double timeLimit = atof(str);
	TIMELIM = timeLimit;
    if(s1 != "-i") return 0;
    if(s2 !="100-300" && s2!="100-600"&&s2!="100-800"&&s2!="200-300"&&s2!="200-600"&&s2!="200-800") return 0;
    if(s3 != "-t") return 0;
    if(timeLimit < 0 && timeLimit > 100) return 0;
    if(s5 != "-th") return 0;
    if(s6 !="0.75" && s6 != "0.80" && s6 != "0.8" && s6!="0.85") return 0;
	if(s7 != "-pi") return 0;
	int pi = stoi(s8);
	if(pi > 15 ) return 0;


    string aux = s2.substr(0,3);
    int filas = stoi(aux);
    aux = s2.substr(4,3);
    int columnas = stoi(aux);
    float threshold = stof(s6);
	
	vector<string> texts;
	for (int i = 0; i < 1; i++){
		texts.push_back(s2);
	}
	texts.at(0) += "-001.txt"; //AHORA SOLO ESTAMOS USANDO LA INSTANCIA 1 DSP USAREMOS LAS OTRAS 

	vector<long double> promedios;
	vector<char> alfabeto{'A', 'C', 'G', 'T'};
	vector<vector<char>> vec1; 


	fstream file1(texts.at(0), fstream::in | fstream::out);
	
	fillMatrix(vec1, file1,filas,columnas);

	vector<char> v_j;	  // vector columna en el que se esta trabajando
	vector<char> s_g;	  // solucion greedy
	vector<vector<char>> sols_vec(pi);

	vector<int> scount_g; // cantidad de la letra elegida en una iteracion de columna VJ.
	 // vector de 100 pos cada una representa la fila i de cada instancia y muestra en
	// cuantas casillas difiere de la solucion
	vector<cromosoma> pob; //poblacion incial
	cromosoma crom_aux;
	vector<int> fitness;
	vector<int> rep_j; // vector con repeticiones tamaño 4
	//---------------------------------------------------------PARTE AG -----------------------------------------------------------------------
	
	
	//inicializacion variables de tiempo
	auto start = chrono::high_resolution_clock::now();
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> diff = end - start;
	
	//Se inicia el algoritmo genetico con la población inicial ya creada y evaluada
	for ( int i = 0; i < pi ; i++){ // se genera la población inicial y se evalua el fitness respectivo.
		crom_aux = greedy(threshold,vec1,v_j,rep_j,alfabeto,s_g,scount_g,filas,columnas);	
		pob.push_back(crom_aux);
	}


	vector<cromosoma> padres;
	vector<cromosoma> nuevos_hijos;
	vector<cromosoma> cruza;
	int limit_padres = 10;
	int i = 1;
	
	while(diff.count() < timeLimit){ //se detiene hasta que se acabe el tiempo limite de ejecución 
		
		//torneo se escogen los dos mejores padres de un conjunto 
		cout<<"generacion: "<<i<<" "<<endl;
		cromosoma padre1;
		cromosoma padre2;
		padre1=(torneo(pob)); 
		padre2=(torneo(pob));
		//Se recombina con los padres escogidos mediante una tecnica de cruzamiento (uniform crossover)
		cruza = uniformCrossover(padre1,padre2); 		 
		setNewFitness(cruza.at(0),vec1,threshold);
		setNewFitness(cruza.at(1),vec1,threshold);
		padres.push_back(padre1);
		padres.push_back(padre2);
		nuevos_hijos.push_back(cruza.at(0));
		nuevos_hijos.push_back(cruza.at(1));
		cruza.clear();
		//se genera una nueva población y se evalua ()
		//10 padres x 2 hijos por par de padres =  10 hijos nuevos  + 15 poblacion inicial(incluye padres) = 25 total
		// -> steady state() pob nueva 15 -> escojo 5 hijos de los mejores (nueva gen) y 5 padres de los mejores (gen vieja)
		
		steady_state(pob,limit_padres,5,nuevos_hijos,padres.size());
		//ahora nuestra nueva población es nuestra población inicial de la siguiente iteración
		
		//en caso de terminar antes break; 
		bool termino = verificarVariabilidad(pob); 
		if(termino) break;
		i++; //numero de iteración
		//se actualiza el tiempo
		end = chrono::high_resolution_clock::now();
		diff = end - start;

	}
	return 0;
}
