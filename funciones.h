#include"estructuras.h"

using namespace std;

void mutacion(vector<cromosoma>& pob){
    
}


bool verificarVariabilidad(vector<cromosoma>& pob){
    long double prom=0;
	cout.precision(9);
	for (int i = 0; i < pob.size(); i++){
		prom=prom + pob.at(i).fitness;
	}
	prom = prom / pob.size();
	cout<<"fitness promedio:  "<<prom<<endl;
	long double sumatoria=0;
	long double varianza=0;
	for (int i = 0; i < pob.size(); i++){
		sumatoria=sumatoria + pow(pob.at(i).fitness+prom,2);
	}
	varianza = sumatoria/(pob.size()-1);
	cout<<"varianza:  "<<varianza<<endl;
	long double desviacion=sqrt(varianza);
	cout<<"Desviacion "<<desviacion<<endl;
    if(desviacion < 10) return true; //CAMBIAR AL EXPERIMENTAR
    else return false;
}


char random_gen(){ // "GEN" random para la cruza de padres
    vector<char> alf{'A', 'C', 'G', 'T'};
    random_device random;
    mt19937 engine{random()};
    uniform_int_distribution <int> alfa_pos(0,3); // rand pos 0 a 3 tam alf
    int pos_gen = alfa_pos(engine); // posicion random
    char gen = alf.at(pos_gen); // sacar el char de la posicion aleatoria de vector alfabeto
    return gen; // retorna char aleatorio
}

void setNewFitness(cromosoma& crom, vector<vector<char>>& mat ,float threshold){ 
    int fitness = 0;
    int filas = mat.size();
    int columnas = crom.gen.size();
    float lim = (float)columnas*threshold; 
    float aux = 0; //contador de posiciones diferentes

    for (int i = 0; i < filas; i++){
        for(int j = 0; j < columnas; j++){
            if(crom.gen.at(j) != mat.at(i).at(j)) aux++;
            if(aux >=  lim){
                fitness++;
                aux = 0;
                break;
            }
        }
    }

    crom.fitness = fitness;
    return;
}

vector<cromosoma> uniformCrossover(cromosoma& padre1,cromosoma& padre2 ){  
    vector<cromosoma> cruza;
    cromosoma hijo1;
    cromosoma hijo2;
    int cromSize = padre1.gen.size();
    for(int i = 0; i<cromSize ;i++){
        int aux = rand() % 2; //da 0 o 1
        if(aux){
            hijo1.gen.push_back(padre1.gen.at(i));//padre1 le hereda su gen al hijo1
            hijo2.gen.push_back(random_gen());//hijo 2 genera un GEN RANDOM
        }else{
            hijo2.gen.push_back(padre2.gen.at(i));//padre2 le hereda su gen al hijo2
            hijo1.gen.push_back(random_gen());//hijo 1 genera un GEN RANDOM
        } 
    }
    hijo1.fitness = 0;
    hijo2.fitness = 0;
    cruza.push_back(hijo1);
    cruza.push_back(hijo2);
    return cruza; //devolver los dos nuevos genes cruzados
}

cromosoma torneo(vector <cromosoma>& poblacion){
    int s=2; //nro de seleccion de participantes de torneo, comunmente se usa 2
    random_device random;//ej: uniform_real_distribution<> prob(0,0.1); uniform_int_distribution<int> dist(0,alf.size()-1); 
    mt19937 engine{random()};
    uniform_int_distribution<int> pos(0,poblacion.size()-1); //se escogen dos cromosomas random para que compitan
    int pos1 = pos(engine);
    int pos2 = pos(engine);

    cromosoma ganador;
    
    while (pos1 == pos2) pos2 = pos(engine); //para evitar torneo entre iguales.
    
    cromosoma padre1 = poblacion.at(pos1);
    cromosoma padre2 = poblacion.at(pos2);
   
    if(padre1.fitness > padre2.fitness) ganador = padre1;
    else ganador = padre2;

    return ganador;
}

void steady_state(vector<cromosoma>& poblacion_inicial, int limit_padres,int elitismo, vector<cromosoma>& nuevos, int padres_size){ //Elimina n viejos cromosomas y los reemplaza con n nuevos  **DECIDIR QUE ELIMINAR
    if (padres_size >= limit_padres){
        vector<cromosoma> new_pob; // antigua poblacion tomada 5
        vector<int> fitness_h; //posiciones tomadas
        
        cromosoma best;
        int best_fit;
        
        for (int i = 0; i < poblacion_inicial.size(); i++){
            for (int j = 0; j < poblacion_inicial.size() - i; j++){
                if(poblacion_inicial[j].fitness > poblacion_inicial[j + 1].fitness){
                    cromosoma aux = poblacion_inicial[j];
                    poblacion_inicial[j] = poblacion_inicial[j + 1];
                    poblacion_inicial[j + 1] = aux;
                }
            }
        }

        for (int i = 0; i < elitismo; i++){  //primeros 5 con mayor fitness
            new_pob.push_back(poblacion_inicial[i]); 
        }

        for (int i = 0; i < nuevos.size(); i++){   
            for (int j = 0; j < nuevos.size() - i; j++){
                if(nuevos[j].fitness > nuevos[j + 1].fitness){
                    cromosoma aux = nuevos[j];
                    nuevos[j] = nuevos[j + 1];
                    nuevos[j + 1] = aux;
                }
            }
        }
        for (int i = 0; i < elitismo; i++){
            new_pob.push_back(nuevos[i]);
        }
        
        poblacion_inicial.clear();
        poblacion_inicial = new_pob;

    }else
        return;
}



/*
vector<int>:: iterator it;
        sort(pos_chosen.begin(),pos_chosen.end());
        int old_size = pos_chosen.size();
        int new_size = old_size;

while(new_size != old_size){ //loops para sacar nros de posiciones que se repiten
            int new_size = old_size;//se hace un aseguramiento de que new size es igual a old size
            it = unique(pos_chosen.begin(),pos_chosen.end()); // se usa funcion unique para borrar copias
            pos_chosen.resize(distance(pos_chosen.begin(), it)); //se usa rezice por si se borraron elementos
            new_size = pos_chosen.size(); // luego se actualiza el valor de new size
        }
*/