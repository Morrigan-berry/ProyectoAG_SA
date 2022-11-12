#include"estructuras.h"

using namespace std;

void printPobFit(vector<cromosoma>& pob){
    for (int i = 0; i < pob.size(); ++i){
        cout<<"fitness "<<i<<" : "<<pob.at(i).fitness<<endl;
    }
}
void printPobStats(vector<cromosoma>& pob){
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
        sumatoria=sumatoria + pow(pob.at(i).fitness-prom,2);
    }
    varianza = sumatoria/(pob.size());
    cout<<"varianza:  "<<varianza<<endl;
    long double desviacion=sqrt(varianza);
    cout<<"Desviacion "<<desviacion<<endl;
    return;
}


void mutacion(vector<cromosoma>& hijos){
    vector<char> alf{'A', 'C', 'G', 'T'};
    random_device random;
    mt19937 engine{random()};
    uniform_int_distribution <int> alfa_pos(0,3);
    for (int h = 0; h < hijos.size(); h++){

        for (int i = 0; i < hijos[h].gen.size(); i++){
            if( (rand()%1) == 1 ){
                int pos_aux = alfa_pos(engine);
                while(hijos[h].gen[i] == alf[pos_aux]){ //asegurarse que no sea el mismo char que el gen del hijo
                    int pos_aux = alfa_pos(engine);
                }
                hijos[h].gen[i] = alf[pos_aux];
            }
        }
        
    }
    
    
}

bool verificar(vector<cromosoma>& pob,int filas,float threshold){ //verificar termino o estancamiento.
    float lim = (float)filas*threshold;
    long double prom=0;
	cout.precision(9);
	for (int i = 0; i < pob.size(); i++){
		prom = prom + pob.at(i).fitness;
	}
	prom = prom / pob.size();
	long double sumatoria = 0;
	long double varianza = 0;
	for (int i = 0; i < pob.size(); i++){
		sumatoria=sumatoria + pow(pob.at(i).fitness-prom,2);
	}
	varianza = sumatoria/(pob.size());
	long double desviacion=sqrt(varianza);
    if(prom > lim) return true;
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
        aux = 0;
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
   
    if(padre1.fitness >= padre2.fitness) ganador = padre1;
    else ganador = padre2;

    return ganador;
}

void steady_state(vector<cromosoma>& poblacion_inicial,int elitismo, vector<cromosoma>& nuevos,vector<cromosoma>& padres){ //Elimina n viejos cromosomas y los reemplaza con n nuevos  **DECIDIR QUE ELIMINAR
        vector<cromosoma> new_pob; // antigua poblacion tomada 5
        vector<int> fitness_h; //posiciones tomadas
        
        cromosoma best;
        int best_fit;
        
        for (int i = 0; i < poblacion_inicial.size()-1; i++){
            for (int j = 0; j < poblacion_inicial.size() - i-1; j++){
                if(poblacion_inicial[j].fitness < poblacion_inicial[j + 1].fitness){
                    cromosoma aux = poblacion_inicial[j];
                    poblacion_inicial[j] = poblacion_inicial[j + 1];
                    poblacion_inicial[j + 1] = aux;
                }
            }
        }

        for (int i = 0; i < elitismo; i++){  //primeros 5 con mayor fitness
            new_pob.push_back(poblacion_inicial[i]); 
        }

        for (int i = 0; i < nuevos.size()-1; i++){   
            for (int j = 0; j < nuevos.size() - i-1; j++){
                if(nuevos[j].fitness < nuevos[j + 1].fitness){
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
        padres.clear();
        poblacion_inicial = new_pob;
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