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
        double h        = 0.0;
        int depth       = 0;

        int count_colors(char);

};

class PR_QUADTREE {

    public:

        NODE* _root                             = NULL;
        double _x;                              // minX = -54.9333 maxX = 82.4833
        double _y;                              // minY = -179.983 maxY = 180
        double _w;
        double _h;
        int _totalPoints                        =  0;
        unsigned long long _totalPopulation     =  0;

        PR_QUADTREE(double, double, double, double);
        ~PR_QUADTREE();
        bool insert(double, double, CITY*);
        int remove(double, double);
        unsigned long long total_population(double, double);
        NODE* search_node(double, double);
        NODE* search_city(double, double);
        int cities_in_region_driver(NODE*, double, double, double, double);
        int cities_in_region(double, double, double, double);
        unsigned long long population_in_region_driver(NODE*, double, double, double, double);
        unsigned long long population_in_region(double, double, double, double);
        int depths_in_region_driver(NODE*, double, double, double, double);
        int depths_in_region(double, double, double, double);
        bool collides(double, double, double, double, double, double, double, double);
        int get_max_depth(void);

};

PR_QUADTREE::PR_QUADTREE(double x = -90.01,
                         double y = -180.01,
                         double w =  180.02,
                         double h =  360.02){
    _x = x;
    _y = y;
    _w = w;
    _h = h;
    _root = new NODE;
    _root->x = x;
    _root->y = y;
    _root->w = w;
    _root->h = h;
}

PR_QUADTREE::~PR_QUADTREE(){}

/////////////////////////////////////////////////////////////////////////////////////////


// devuelve un puntero a nodo con el cuadrante mas pequeño que incluye la posicion (x,y)
NODE* PR_QUADTREE::search_node(double x, double y){

    NODE* node = _root;

    while(1){

        // punto pertenece al primer cuadrante?
        if(node->x <= x && x < node->x + node->w/2 &&
           node->y + node->h/2 <= y && y < node->y + node->h){

                if(node->color != 'g')
                    break;
                else
                    node = node->first;

        // punto pertenece al segundo cuadrante?
        } else if(node->x + node->w/2 <= x && x < node->x + node->w &&
           node->y + node->h/2 <= y && y < node->y + node->h){

                if(node->color != 'g')
                    break;
                else
                    node = node->second;

        // punto pertenece al tercer cuadrante?
        } else if(node->x <= x && x < node->x + node->w/2 &&
           node->y <= y && y < node->y + node->h/2){

                if(node->color != 'g')
                    break;
                else
                    node = node->third;

        // punto pertenece al cuarto cuadrante?
        } else if(node->x + node->w/2 <= x && x < node->x + node->w &&
           node->y <= y && y < node->y + node->h/2){

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

bool PR_QUADTREE::insert(double x, double y, CITY* city){

    // se prueba la preexistencia de una ciudad en el mismo punto (en ese caso se ignora)
    // (search_node solo puede devolver nodos blancos o negros, nunca NULL ;D)
    NODE* node = search_node(x,y);

    // si existe una ciudad en la misma posicion se evita el insert
    if( node->color == 'b' && x == node->data->geoPointX && y == node->data->geoPointY){

        // si ya existe una ciudad con esta posicion se ignora
        // la nueva ciudad y se aprovecha de liberar su memoria
        // luego se retorna
        delete city;
        return(false);

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
                   temp->y + temp->h/2 <= y && y < temp->y + temp->h){

                        cuadrante1 = 1;

                // punto1 pertenece al segundo subcuadrante?
                } else if(temp->x + temp->w/2 <= x && x < temp->x + temp->w &&
                   temp->y + temp->h/2 <= y && y < temp->y + temp->h){

                        cuadrante1 = 2;

                // punto1 pertenece al tercer subcuadrante?
                } else if(temp->x <= x && x < temp->x + temp->w/2 &&
                   temp->y <= y && y < temp->y + temp->h/2){

                        cuadrante1 = 3;

                // punto1 pertenece al cuarto subcuadrante?
                } else if(temp->x + temp->w/2 <= x && x < temp->x + temp->w &&
                   temp->y <= y && y < temp->y + temp->h/2){

                        cuadrante1 = 4;

                // punto2 pertenece al primer subcuadrante?
                } if(temp->x <= oldX && oldX < temp->x + temp->w/2 &&
                   temp->y + temp->h/2 <= oldY && oldY < temp->y + temp->h){

                        cuadrante2 = 1;

                // punto2 pertenece al segundo subcuadrante?
                } else if(temp->x + temp->w/2 <= oldX && oldX < temp->x + temp->w &&
                   temp->y + temp->h/2 <= oldY && oldY < temp->y + temp->h){

                        cuadrante2 = 2;

                // punto2 pertenece al tercer subcuadrante?
                } else if(temp->x <= oldX && oldX < temp->x + temp->w/2 &&
                   temp->y <= oldY && oldY < temp->y + temp->h/2){

                        cuadrante2 = 3;

                // punto2 pertenece al cuarto subcuadrante?
                } else if(temp->x + temp->w/2 <= oldX && oldX < temp->x + temp->w &&
                   temp->y <= oldY && oldY < temp->y + temp->h/2){

                        cuadrante2 = 4;

                }

                // se crean los 4 nodos hijo
                temp->first = new NODE;
                temp->first->father = temp;
                temp->first->depth = temp->depth + 1;
                temp->first->color = 'w';
                temp->first->x = temp->x;
                temp->first->y = temp->y + temp->h/2;
                temp->first->w = temp->w/2;
                temp->first->h = temp->h/2;

                temp->second = new NODE;
                temp->second->father = temp;
                temp->second->depth = temp->depth + 1;
                temp->second->color = 'w';
                temp->second->x = temp->x + temp->w/2;
                temp->second->y = temp->y + temp->h/2;
                temp->second->w = temp->w/2;
                temp->second->h = temp->h/2;

                temp->third = new NODE;
                temp->third->father = temp;
                temp->third->depth = temp->depth + 1;
                temp->third->color = 'w';
                temp->third->x = temp->x;
                temp->third->y = temp->y;
                temp->third->w = temp->w/2;
                temp->third->h = temp->h/2;

                temp->fourth = new NODE;
                temp->fourth->father = temp;
                temp->fourth->depth = temp->depth + 1;
                temp->fourth->color = 'w';
                temp->fourth->x = temp->x + temp->w/2;
                temp->fourth->y = temp->y;
                temp->fourth->w = temp->w/2;
                temp->fourth->h = temp->h/2;

                // condicion de termino para creacion de subnodos
                // cuando ambos puntos no estan en el mismo cuadrante
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
    }

    // se aumenta el contador de puntos del quadtree
    _totalPoints++;

    return(true);

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
    NODE* father = NULL;

    // se comprueba la existencia de la ciudad
    if(node == NULL)
        return(0);

    // se borran los datos del nodo y se reinicia
    _totalPopulation -= node->data->population;
    _totalPoints -= 1;
    node->color = 'w';

    // si solo existia el nodo root se retorna
    if(node == _root){
        node->data = NULL;
        return(0);
    }

    father = node->father;

    // se COMPACTA el arbol de forma iterativa
    while(node != NULL && node->father != NULL) {

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

            node->data = NULL;
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
            node->data = NULL;
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

// devuelve true si existe colision entre dos rectangulos A y B con centro (x,y) con radios w y h (no diametros!)
bool PR_QUADTREE::collides(double Ax, double Ay, double Aw, double Ah, double Bx, double By, double Bw, double Bh){
    if( ( abs(Ax-Bx) <= (Aw+Bw) ) && ( abs(Ay-By) <= (Ah+Bh) ) )
        return(true);
    else
        return(false);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

// devuelve total de ciudades en region acotada por un rectangulo centrado en (x,y) con radios w y h (no diametros!)
int PR_QUADTREE::cities_in_region(double x, double y, double w, double h){
    return(cities_in_region_driver(_root, x, y, w, h));
}

int PR_QUADTREE::cities_in_region_driver(NODE* node, double rx, double ry, double rw, double rh){

    // si no existen puntos
    if(node == NULL || node->color == 'w')
        return(0);

    // si hay un solo punto se comprueba su pertenencia a la region
    if(node->color == 'b'){

        double nx = node->data->geoPointX;
        double ny = node->data->geoPointY;

        if(rx-rw <= nx && nx < rx + rw && ry-rh <= ny && ny < ry + rh)
           return(1);
        else
            return(0);

    // se comprueba cuales cuadrantes colisionan con la region y se acumulan
    } else if(node->color == 'g') {

        int ctr = 0;
        if(collides(rx, ry, rw, rh, node->first->x + node->first->w/2, node->first->y + node->first->h/2, node->first->w/2, node->first->h/2 ))
            ctr += cities_in_region_driver(node->first,  rx, ry, rw, rh);

        if(collides(rx, ry, rw, rh, node->second->x + node->second->w/2, node->second->y + node->second->h/2, node->second->w/2, node->second->h/2 ))
            ctr += cities_in_region_driver(node->second,  rx, ry, rw, rh);

        if(collides(rx, ry, rw, rh, node->third->x + node->third->w/2, node->third->y + node->third->h/2, node->third->w/2, node->third->h/2 ))
            ctr += cities_in_region_driver(node->third,  rx, ry, rw, rh);

        if(collides(rx, ry, rw, rh, node->fourth->x + node->fourth->w/2, node->fourth->y + node->fourth->h/2, node->fourth->w/2, node->fourth->h/2 ))
            ctr += cities_in_region_driver(node->fourth,  rx, ry, rw, rh);

        return(ctr);

    }
}

/////////////////////////////////////////////////////////////////////////////////////////

// devuelve total de poblacion en region acotada por un rectangulo centrado en (x,y) con radios w y h (no diametros!)
unsigned long long PR_QUADTREE::population_in_region(double x, double y, double w, double h){
    return(population_in_region_driver(_root, x, y, w, h));
}

unsigned long long PR_QUADTREE::population_in_region_driver(NODE* node, double rx, double ry, double rw, double rh){

    // si no existen puntos
    if(node == NULL || node->color == 'w')
        return(0);

    // si hay un solo punto se comprueba su pertenencia a la region
    if(node->color == 'b'){

        double nx = node->data->geoPointX;
        double ny = node->data->geoPointY;

        if(rx-rw <= nx && nx < rx + rw && ry-rh <= ny && ny < ry + rh)
           return(node->data->population);
        else
            return(0);

    // se comprueba cuales cuadrantes colisionan con la region y se acumulan
    } else if(node->color == 'g') {

        unsigned long long ctr = 0;
        if(collides(rx, ry, rw, rh, node->first->x + node->first->w/2, node->first->y + node->first->h/2, node->first->w/2, node->first->h/2 ))
            ctr += population_in_region_driver(node->first,  rx, ry, rw, rh);

        if(collides(rx, ry, rw, rh, node->second->x + node->second->w/2, node->second->y + node->second->h/2, node->second->w/2, node->second->h/2 ))
            ctr += population_in_region_driver(node->second,  rx, ry, rw, rh);

        if(collides(rx, ry, rw, rh, node->third->x + node->third->w/2, node->third->y + node->third->h/2, node->third->w/2, node->third->h/2 ))
            ctr += population_in_region_driver(node->third,  rx, ry, rw, rh);

        if(collides(rx, ry, rw, rh, node->fourth->x + node->fourth->w/2, node->fourth->y + node->fourth->h/2, node->fourth->w/2, node->fourth->h/2 ))
            ctr += population_in_region_driver(node->fourth,  rx, ry, rw, rh);

        return(ctr);

    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////

int PR_QUADTREE::get_max_depth(void){
    return(depths_in_region(_x, _y, _w*1000, _h*1000));
}

// devuelve las profundidades máximas en region acotada por un rectangulo centrado en (x,y) con radios w y h (no diametros!)
int PR_QUADTREE::depths_in_region(double x, double y, double w, double h){
    return(depths_in_region_driver(_root, x, y, w, h));
}

int PR_QUADTREE::depths_in_region_driver(NODE* node, double rx, double ry, double rw, double rh){

    // si no existen puntos
    if(node == NULL || node->color == 'w')
        return(0);

    // si hay un solo punto se comprueba su pertenencia a la region
    if(node->color == 'b'){

        double nx = node->data->geoPointX;
        double ny = node->data->geoPointY;

        if(rx-rw <= nx && nx < rx + rw && ry-rh <= ny && ny < ry + rh)
           return(node->depth);
        else
            return(0);

    // se comprueba cuales cuadrantes colisionan con la region y se acumulan
    } else if(node->color == 'g') {

        int maxDepth = 0;
        int temp = 0;

        if(collides(rx, ry, rw, rh, node->first->x + node->first->w/2, node->first->y + node->first->h/2, node->first->w/2, node->first->h/2 )){
            temp = depths_in_region_driver(node->first,  rx, ry, rw, rh);
            if(temp > maxDepth)
                maxDepth = temp;
        }

        if(collides(rx, ry, rw, rh, node->second->x + node->second->w/2, node->second->y + node->second->h/2, node->second->w/2, node->second->h/2 )){
            temp = depths_in_region_driver(node->second,  rx, ry, rw, rh);
            if(temp > maxDepth)
                maxDepth = temp;
        }

        if(collides(rx, ry, rw, rh, node->third->x + node->third->w/2, node->third->y + node->third->h/2, node->third->w/2, node->third->h/2 )){
            temp = depths_in_region_driver(node->third,  rx, ry, rw, rh);
            if(temp > maxDepth)
                maxDepth = temp;
        }

        if(collides(rx, ry, rw, rh, node->fourth->x + node->fourth->w/2, node->fourth->y + node->fourth->h/2, node->fourth->w/2, node->fourth->h/2 )){
            temp = depths_in_region_driver(node->fourth,  rx, ry, rw, rh);
            if(temp > maxDepth)
                maxDepth = temp;
        }

        return(maxDepth);

    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

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

    // datos de histograma para graficar
    double sub  = 1.0;
    double subX = 180*sub;
    double subY = 360*sub;
    unsigned long long temp2 = 0, temp3 = 0;
    file.open("citiesPerRegion_180x360.txt");

    for(int i=0; i<subX; i++){
        for(int j=0; j<subY; j++){
            temp2 = cities.cities_in_region( cities._x + i*cities._w/(subX),
                                             cities._y + j*cities._h/(subY),
                                             cities._w/(subX),
                                             cities._h/(subY) );

            file << i << "," << j << "," << temp2 << endl;
            temp3 += temp2;
            //cout << cities._x + i*cities._w/(sub) << " " << cities._y + j*cities._w/(sub) << " " << cities._w/(sub) << endl;
        }
        cout << i << " " << " " << temp2 << endl;
    }
    file.close();

    //while(1){}

    return(0);

}

/////////////////////////////////////////////////////////////////////////////////////////
