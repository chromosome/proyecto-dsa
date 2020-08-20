#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class CITY {
    public:
        unsigned long long population;
        double geoPointX;
        double geoPointY;
};

class NODE {
    public:
        CITY* data      = NULL;
        char color      = 'w';
        NODE* father    = NULL;
        NODE* first     = NULL;
        NODE* second    = NULL;
        NODE* third     = NULL;
        NODE* fourth    = NULL;
        double x        = 0.0;
        double y        = 0.0;
        double w        = 0.0;
        int depth       = 0;

        int count_colors(char);

};

class PR_QUADTREE {

    public:

        NODE* _root                     = NULL;
        double _x                       = -181.0; // minX = -54.9333 maxX = 82.4833
        double _y                       = -181.0; // minY = -179.983 maxY = 180
        double _w                       = 362.0;
        int _totalPoints                = 0;
        unsigned long long _totalPopulation   = 0;
        int _maxDepth                   = 0;

        PR_QUADTREE();
        ~PR_QUADTREE();
        void insert(double, double, CITY*);
        int remove(double, double);
        unsigned long long total_population(double, double);
        NODE* search_node(double, double);
        NODE* search_city(double, double);
        int cities_in_region_driver(NODE*, double, double, double);
        int cities_in_region(double, double, double);
        unsigned long long population_in_region_driver(NODE*, double, double, double);
        unsigned long long population_in_region(double, double, double);
        bool collides(double, double, double, double, double, double);

};

PR_QUADTREE::PR_QUADTREE(){
    _root = new NODE;
    _root->x = _x;
    _root->y = _y;
    _root->w = _w;
}

PR_QUADTREE::~PR_QUADTREE(){}

/////////////////////////////////////////////////////////////////////////////////////////


// devuelve un puntero a nodo con el cuadrante mas pequeño que incluye la posicion (x,y)
NODE* PR_QUADTREE::search_node(double x, double y){

    NODE* node = _root;

    while(1){

        // punto pertenece al primer cuadrante?
        if(node->x <= x && x < node->x + node->w/2 &&
           node->y + node->w/2 <= y && y < node->y + node->w){

                if(node->color != 'g')
                    break;
                else
                    node = node->first;

        // punto pertenece al segundo cuadrante?
        } else if(node->x + node->w/2 <= x && x < node->x + node->w &&
           node->y + node->w/2 <= y && y < node->y + node->w){

                if(node->color != 'g')
                    break;
                else
                    node = node->second;

        // punto pertenece al tercer cuadrante?
        } else if(node->x <= x && x < node->x + node->w/2 &&
           node->y <= y && y < node->y + node->w/2){

                if(node->color != 'g')
                    break;
                else
                    node = node->third;

        // punto pertenece al cuarto cuadrante?
        } else if(node->x + node->w/2 <= x && x < node->x + node->w &&
           node->y <= y && y < node->y + node->w/2){

                if(node->color != 'g')
                    break;
                else
                    node = node->fourth;

        }
    }

    // devuelve el nodo blanco o negro donde tendría que estar el punto
    // en teoria hasta este punto node solo puede ser blanco o negro, nunca NULL
    return(node);

}

/////////////////////////////////////////////////////////////////////////////////////////

// devuelve un puntero al nodo que contiene la ciudad o NULL si no existe
NODE* PR_QUADTREE::search_city(double x, double y){

    // se busca el nodo blanco o negro donde tendría que estar el punto
    NODE* node = search_node(x,y);

    // se devuelve el nodo que contiene la ciudad en x,y si existe o NULL si no existe
    if( node->color == 'b' && x == node->data->geoPointX && y == node->data->geoPointY)
        return(node);
    else
        return(NULL);

}

/////////////////////////////////////////////////////////////////////////////////////////

void PR_QUADTREE::insert(double x, double y, CITY* city){

    // se prueba la preexistencia de una ciudad en el mismo punto (en ese caso se ignora)
    NODE* node = search_node(x,y);

    // si existe una ciudad en la misma posicion se evita el insert
    if( node->color == 'b' && x == node->data->geoPointX && y == node->data->geoPointY){

        // si ya existe una ciudad con esta posicion se ignora
        // la nueva ciudad y se aprovecha de liberar su memoria
        delete city;

    } else {

        // se aumenta la poblacion total del quadtree
        _totalPopulation += city->population;

        // el cuadrante ya contiene un punto? es decir hay colision en el nodo?

        // NO hay colision
        if(node->color == 'w'){
            node->data = city;
            node->color = 'b';

        // SI hay colision
        } else if (node->color == 'b'){

            // se debe mover el punto antiguo a un nuevo nodo hijo
            CITY* oldCity = node->data;
            double oldX = oldCity->geoPointX;
            double oldY = oldCity->geoPointY;
            node->data = NULL;
            node->color = 'g';

            int cuadrante1 = 1;
            int cuadrante2 = 1;

            // se crean subramas hasta que el punto antiguo y el nuevo
            // se encuentren en cuadrantes diferentes

            NODE* temp = node;

            while(1){

                // se marca como gris
                temp->color = 'g';

                // se comprueba en que subcuadrante se encuentra cada punto

                // punto1 pertenece al primer subcuadrante?
                if(temp->x <= x && x < temp->x + temp->w/2 &&
                   temp->y + temp->w/2 <= y && y < temp->y + temp->w){

                        cuadrante1 = 1;

                // punto1 pertenece al segundo subcuadrante?
                } else if(temp->x + temp->w/2 <= x && x < temp->x + temp->w &&
                   temp->y + temp->w/2 <= y && y < temp->y + temp->w){

                        cuadrante1 = 2;

                // punto1 pertenece al tercer subcuadrante?
                } else if(temp->x <= x && x < temp->x + temp->w/2 &&
                   temp->y <= y && y < temp->y + temp->w/2){

                        cuadrante1 = 3;

                // punto1 pertenece al cuarto subcuadrante?
                } else if(temp->x + temp->w/2 <= x && x < temp->x + temp->w &&
                   temp->y <= y && y < temp->y + temp->w/2){

                        cuadrante1 = 4;

                // punto2 pertenece al primer subcuadrante?
                } if(temp->x <= oldX && oldX < temp->x + temp->w/2 &&
                   temp->y + temp->w/2 <= oldY && oldY < temp->y + temp->w){

                        cuadrante2 = 1;

                // punto2 pertenece al segundo subcuadrante?
                } else if(temp->x + temp->w/2 <= oldX && oldX < temp->x + temp->w &&
                   temp->y + temp->w/2 <= oldY && oldY < temp->y + temp->w){

                        cuadrante2 = 2;

                // punto2 pertenece al tercer subcuadrante?
                } else if(temp->x <= oldX && oldX < temp->x + temp->w/2 &&
                   temp->y <= oldY && oldY < temp->y + temp->w/2){

                        cuadrante2 = 3;

                // punto2 pertenece al cuarto subcuadrante?
                } else if(temp->x + temp->w/2 <= oldX && oldX < temp->x + temp->w &&
                   temp->y <= oldY && oldY < temp->y + temp->w/2){

                        cuadrante2 = 4;

                }

                // se crean los 4 nodos hijo
                temp->first = new NODE;
                temp->first->father = node;
                temp->first->depth = node->depth + 1;
                temp->first->color = 'w';
                temp->first->x = temp->x;
                temp->first->y = temp->y + temp->w/2;
                temp->first->w = temp->w/2;
                temp->first->w = temp->w/2;

                temp->second = new NODE;
                temp->second->father = node;
                temp->second->depth = node->depth + 1;
                temp->second->color = 'w';
                temp->second->x = temp->x + temp->w/2;
                temp->second->y = temp->y + temp->w/2;
                temp->second->w = temp->w/2;
                temp->second->w = temp->w/2;

                temp->third = new NODE;
                temp->third->father = node;
                temp->third->depth = node->depth + 1;
                temp->third->color = 'w';
                temp->third->x = temp->x;
                temp->third->y = temp->y;
                temp->third->w = temp->w/2;
                temp->third->w = temp->w/2;

                temp->fourth = new NODE;
                temp->fourth->father = node;
                temp->fourth->depth = node->depth + 1;
                temp->fourth->color = 'w';
                temp->fourth->x = temp->x + temp->w/2;
                temp->fourth->y = temp->y;
                temp->fourth->w = temp->w/2;
                temp->fourth->w = temp->w/2;

                // se actualiza el contador de maxima profundidad del quadtree :p
                if(_maxDepth < node->depth + 1)
                    _maxDepth = node->depth + 1;

                // condicion de termino para creacion de subnodos
                if(cuadrante1 != cuadrante2){
                    break;
                } else if(cuadrante1 == 1){
                    temp = temp->first;
                } else if(cuadrante1 == 2) {
                    temp = temp->second;
                } else if(cuadrante1 == 3) {
                    temp = temp->third;
                } else if(cuadrante1 == 4) {
                    temp = temp->fourth;
                }

            }

            // luego se agregan las ciudades a los cuadrantes correspondientes
            if(cuadrante1 == 1){
                temp->first->color = 'b';
                temp->first->data = city;
            } else if(cuadrante1 == 2) {
                temp->second->color = 'b';
                temp->second->data = city;
            } else if(cuadrante1 == 3) {
                temp->third->color = 'b';
                temp->third->data = city;
            } else if(cuadrante1 == 4) {
                temp->fourth->color = 'b';
                temp->fourth->data = city;
            }

            if(cuadrante2 == 1){
                temp->first->color = 'b';
                temp->first->data = oldCity;
            } else if(cuadrante2 == 2) {
                temp->second->color = 'b';
                temp->second->data = oldCity;
            } else if(cuadrante2 == 3) {
                temp->third->color = 'b';
                temp->third->data = oldCity;
            } else if(cuadrante2 == 4) {
                temp->fourth->color = 'b';
                temp->fourth->data = oldCity;
            }

        }

        // se aumenta el contador de puntos del quadtree
        _totalPoints++;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

// devuelve el total de nodos hijo del color especificado
int NODE::count_colors(char x){
    int ctr = 0;
    if(color == 'g'){
        if(first->color     == x) ctr++;
        if(second->color    == x) ctr++;
        if(third->color     == x) ctr++;
        if(fourth->color    == x) ctr++;
    }
    return(ctr);
}


int PR_QUADTREE::remove(double x, double y){

    NODE* node = search_city(x,y);
    NODE* father = node->father;

    // se comprueba la existencia de la ciudad
    if(node == NULL)
        return(-1);

    // se borran los datos del nodo y se reinicia
    _totalPopulation -= node->data->population;
    delete node->data;
    node->data = NULL;
    node->color = 'w';
    _totalPoints -= 1;

    // si solo existia el nodo root se retorna
    if(node == _root)
        return(0);

    // se COMPACTA el arbol de forma iterativa
    while(father != NULL) {

        // es necesaria una referencia directa al padre para evitar casos problematicos
        // en que el nodo elimine referencias asi mismo usando node->father->{first,second,...}
        father = node->father;

        // se cuentan los colores de los nodos hijos
        int g, w, b;
        g = father->count_colors('g');
        w = father->count_colors('w');
        b = 4 - g - w;

        // si solo quedan nodos blancos se eliminan y nodo padre se blanquea
        if(w == 4){

            delete father->first;
            delete father->second;
            delete father->third;
            delete father->fourth;
            father->first   = NULL;
            father->second  = NULL;
            father->third   = NULL;
            father->fourth  = NULL;
            father->color = 'w';
            node = father; // para siguiente iteracion

        // si solo queda un nodo negro entonces reemplaza al nodo padre y se eliminan los hijos
        } else if (b == 1 && w == 3) {

            father->color = 'b';
            father->data = node->data;
            delete father->first;
            delete father->second;
            delete father->third;
            delete father->fourth;
            father->first   = NULL;
            father->second  = NULL;
            father->third   = NULL;
            father->fourth  = NULL;
            node = father; // para siguiente iteracion

        // condicion de termino de compactacion (g >= 1 || b >= 2)
        } else {
            break;
        }
    }
    return(0);
}

/////////////////////////////////////////////////////////////////////////////////////////

// retorna la poblacion estimada de una ciudad (si ciudad no existe se considera poblacion estimada = 0)
unsigned long long PR_QUADTREE::total_population(double x, double y){

    NODE* node = search_city(x,y);
    if(node == NULL)
        return(0);
    else
        return(node->data->population);

}

/////////////////////////////////////////////////////////////////////////////////////////


bool PR_QUADTREE::collides(double Ax, double Ay, double Aw, double Bx, double By, double Bw){
    if( ( abs(Ax-Bx) <= 2*(Aw+Bw) ) || ( abs(Ay-By) <= 2*(Aw+Bw) ) )
        return(true);
    else
        return(false);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

int PR_QUADTREE::cities_in_region_driver(NODE* node, double rx, double ry, double rw){

    // si no existen puntos
    if(node == NULL || node->color == 'w')
        return(0);

    // si hay un solo punto se comprueba su pertenencia a la region
    if(node->color == 'b'){

        double nx = node->data->geoPointX;
        double ny = node->data->geoPointY;

        if(rx-rw/2 <= nx && nx < rx + rw/2 && ry-rw/2 <= ny && ny < ry + rw/2)
           return(1);
        else
            return(0);

    // si hay mas de un cuadrante se comprueba que el cuadrante y region colisionen
    } else if(node->color == 'g') {

        int ctr = 0;
        if(collides(rx, ry, rw/2, node->first->x + node->first->w/2, node->first->y + node->first->w/2, node->first->w/2 ))
            ctr += cities_in_region_driver(node->first,  rx, ry, rw);

        if(collides(rx, ry, rw/2, node->second->x + node->second->w/2, node->second->y + node->second->w/2, node->second->w/2 ))
            ctr += cities_in_region_driver(node->second, rx, ry, rw);

        if(collides(rx, ry, rw/2, node->third->x + node->third->w/2, node->third->y + node->third->w/2, node->third->w/2 ))
            ctr += cities_in_region_driver(node->third,  rx, ry, rw);

        if(collides(rx, ry, rw/2, node->fourth->x + node->fourth->w/2, node->fourth->y + node->fourth->w/2, node->fourth->w/2 ))
            ctr += cities_in_region_driver(node->fourth, rx, ry, rw);

        return(ctr);

    }
}

int PR_QUADTREE::cities_in_region(double x, double y, double w){
    return(cities_in_region_driver(_root, x, y, w));
}

/////////////////////////////////////////////////////////////////////////////////////////

unsigned long long PR_QUADTREE::population_in_region_driver(NODE* node, double rx, double ry, double rw){

    // si no existen puntos
    if(node == NULL || node->color == 'w')
        return(0);

    // si hay un solo punto se comprueba su pertenencia a la region
    if(node->color == 'b'){

        double nx = node->data->geoPointX;
        double ny = node->data->geoPointY;

        if(rx-rw/2 <= nx && nx < rx + rw/2 && ry-rw/2 <= ny && ny < ry + rw/2)
           return(node->data->population);
        else
            return(0);

    // si hay mas de un cuadrante se comprueba que el cuadrante y region colisionen
    } else if(node->color == 'g') {

        unsigned long long ctr = 0;
        if(collides(rx, ry, rw/2, node->first->x + node->first->w/2, node->first->y + node->first->w/2, node->first->w/2 ))
            ctr += population_in_region_driver(node->first,  rx, ry, rw);

        if(collides(rx, ry, rw/2, node->second->x + node->second->w/2, node->second->y + node->second->w/2, node->second->w/2 ))
            ctr += population_in_region_driver(node->second, rx, ry, rw);

        if(collides(rx, ry, rw/2, node->third->x + node->third->w/2, node->third->y + node->third->w/2, node->third->w/2 ))
            ctr += population_in_region_driver(node->third,  rx, ry, rw);

        if(collides(rx, ry, rw/2, node->fourth->x + node->fourth->w/2, node->fourth->y + node->fourth->w/2, node->fourth->w/2 ))
            ctr += population_in_region_driver(node->fourth, rx, ry, rw);

        return(ctr);

    }
}

unsigned long long PR_QUADTREE::population_in_region(double x, double y, double w){
    return(population_in_region_driver(_root, x, y, w));
}


/////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {

    // lectura de datos
    fstream file;
    file.open("worldcitiespop_fixed.csv");
    //file.open("mini.csv");
    string line;
    string word;
    string temp;
    int ctr = 0;

    PR_QUADTREE cities;

    getline(file,line);
    while(getline(file,line) && ctr < 3173647){

        stringstream ss(line);

        CITY* city = NULL;
        city = new CITY;

        /*  Country;City;AccentCity;Region;
        Population;Latitude;Longitude;geopoint */

        getline(ss,temp,';');
        //city->country = temp;

        getline(ss,temp,';');
        //city->city = temp;

        getline(ss,temp,';');
        //city->accentCity = temp;

        getline(ss,temp,';');
        //city->region = temp;

        getline(ss,word,';');
        city->population = stoul(word);

        getline(ss,word,';');
        //city->latitude = stold(word);

        getline(ss,word,';');
        //city->longitude = stold(word);

        getline(ss,word,',');
        city->geoPointX = stold(word);

        getline(ss,word,',');
        city->geoPointY = stold(word);

        // se insertan los datos al quadtree
        cities.insert(city->geoPointX, city->geoPointY, city);

        ctr++;

    }

    file.close();

    // pruebas insert
    NODE* x = cities.search_node((double)8.3766667,(double)-78.9591667);
    if(x != NULL)
        cout << "existe la ciudad <8.3766667,-78.9591667>" << endl;
    cout << "poblacion ciudad <8.3766667,-78.9591667>: " << x->data->population << endl;
    cout << "profundidad nodo de ciudad <8.3766667,-78.9591667>: " << x->depth << endl;
    cout << "total de ciudades en quadtree: " << cities._totalPoints << endl;
    cout << "total de habitantes en quadtree: " << cities._totalPopulation << endl;
    cout << "maxima profundidad de nodo en quadtree: " << cities._maxDepth << endl;

    // pruebas region
    double rx = 0.0;
    double ry = 0.0;
    double rw = 400.0;
    cout << "busqueda por region -> ciudades en: " << cities.cities_in_region(rx, ry, rw) << endl;
    cout << "busqueda por region -> habitantes en: " << cities.population_in_region(rx, ry, rw) << endl;

    // pruebas remove
    if(cities.remove((double)8.3766667,(double)-78.9591667) == 0)
        cout << "ciudad <8.3766667,-78.9591667> removida" << endl;
    x = cities.search_city((double)8.3766667,(double)-78.9591667);
    if(x == NULL)
        cout << "ya no existe la ciudad <8.3766667,-78.9591667> :C" << endl;

    // datos de histograma para graficar
    double sub = 25;
    unsigned long long temp2 = 0, temp3 = 0;
    file.open("histogram2D.txt");

    for(int i=0; i<sub; i++){
        for(int j=0; j<sub; j++){
            temp2 = cities.cities_in_region( cities._x + i*cities._w/(sub),
                                             cities._y + j*cities._w/(sub),
                                             cities._w/(sub) );
            cout << i << " " << j << " " << temp2 << endl;
            file << i << " " << j << " " << temp2 << endl;
            temp3 += temp2;
            //cout << cities._x + i*cities._w/(sub) << " " << cities._y + j*cities._w/(sub) << " " << cities._w/(sub) << endl;
        }
    }
    file.close();
    cout << endl << "total de poblacion por histograma " << sub << "x" << sub << " " << temp3 << endl << endl;

    //while(1){}

    return(0);

}

/////////////////////////////////////////////////////////////////////////////////////////
