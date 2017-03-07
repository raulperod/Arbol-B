using namespace std;

// enumeraciones
enum EsMenor{SI,NO};
enum Donde{VACIO,PRINCIPIO,ENMEDIO,FINAL};

struct Casilla2;
#include "Pagina.h"

// representa la casilla que va en la lista del arbol b
struct Casilla2{
	Casilla2 *siguiente,*anterior; // el siguiente y el anterior en la lista 
	//del arbol b
	Casilla *direccionArbol; // direccion a la casilla del arbol	
};

class ArbolB{
	int orden; // es el orden del arbol b	
	// es el numero minimo de casillas que puede tener una pagina (orden-1 / 2)
	int minimo; 
	Pagina *raiz; // la pagina raiz del arbol b
	Casilla *lugarEncontrado; // el lugar encontrado
	Casilla2 *principio,*final; // principio y final de la lista del arbol b
	int cuantos; // el numero de casillas en el arbol b
	
public:

	ArbolB(int); // contructor del arbol b
	ArbolB();//otro constructor
	~ArbolB(); // destructor del arbol b
	//pone el orden y el minimo que puede tener cada pagina
	void ponerOrden(int orden);
	void destructorCopia();//copia dle destructor
	bool buscar(int); // busca un numero en el arbol b
	void agregar(int); // agregar un numero en el arbol b
	void agregarEnLista(Casilla *);//agrega una casilla en la lista del arbol b
	void pintarArbol();//Para probar el arbol, ponerlo mas bonito despues
	void pintar();///pinta la lista de numeros ordenados
	// funcion que divide una pagina en dos 
	//cuando este supere el numero maximo permitido
	void dividir(Casilla *,Pagina*); 
	bool borrar(int); // funcion que borra un numero en el arbol
	// funcion que desliga una casilla de la lista del arbol b
	void desligar(Casilla2 *); 
	//funcion que intercambia el valor de dos casillas 
	void intercambiar(Casilla *, Casilla *); 
	// funcion que traspasa las casillas de una pagina a otra 
	void traspasarCasillas(Pagina *, Pagina *); 
	bool agregarArchivo();//agregar numeros al arbol b por archivo
	bool guardarArchivo();//guardar los numeros ordenados en un archivo
	bool borrarArchivo();//borrar los numeros ordenados en un archivo
};

/* contructor del arbol b, tiene como paramtro el 'orden' del arbol*/
ArbolB::ArbolB(int orden){
	this->orden = orden;
	this->minimo = (orden-1) / 2;
	raiz = NULL;
	lugarEncontrado = NULL;
	principio =NULL;
	final = NULL;
	cuantos = 0;
}
//otro constructor
ArbolB::ArbolB(){
	this->orden = 0;
	this->minimo = 0;
	raiz = NULL;
	lugarEncontrado = NULL;
	principio =NULL;
	final = NULL;
	cuantos = 0;
}
//funcion que pone el orden del arbol y el minimo
void ArbolB::ponerOrden(int orden){
	this->orden = orden;
	this->minimo = (orden-1) / 2;
}
/* destructor del arbol b*/
ArbolB::~ArbolB(){
	Casilla2 *p;
	p=principio;
	
	while(p){
		if(p->direccionArbol) p->direccionArbol->actual->terminar();
		p = p->siguiente;
	}
	
	orden = 0;
	raiz = NULL;
	lugarEncontrado = NULL;
	principio =NULL;
	final = NULL;
	cuantos = 0;
		
}
//Funcion copia del destructor para porde reiniciar el arbol
void ArbolB::destructorCopia(){
	Casilla2 *p;
	p=principio;
	
	while(p){
		if(p->direccionArbol) p->direccionArbol->actual->terminar();
		p = p->siguiente;
	}
	
	orden = 0;
	raiz = NULL;
	lugarEncontrado = NULL;
	principio =NULL;
	final = NULL;
	cuantos = 0;
		
}
/* funcion que buscar un numero 'a' en el arbol b, tiene como parametro el numero 'a' a buscar
   regresa un booleano dependiendo de si lo encontro o no */
bool ArbolB::buscar(int a){
	
	Casilla *lugar; // lugar en la casilla encontrado
	Pagina *p,*actual; // actual es la pagina donde seguira buscando
	p = raiz;

	while(p){
		if( actual = p->buscar2(a) ){
			
			lugar = p->obtenerLugarEncontrado();
			if( !(lugar->actual == actual) ){
			
				p = actual;
			}else{
				// encontro el numero y guarda la casilla en lugarEncontrado
				lugarEncontrado = lugar;
				return true;
			}						
	
		}else if( lugar = p->obtenerLugarEncontrado() ){
			// no se encontro el numero, guarda la casilla encontrada
			// la cual se encuentra en una pagina hoja
			lugarEncontrado = lugar;
			return false;
		}	

	}
	
	lugarEncontrado = NULL;
	return false;
}
/*  funcion que agrega un numero al arbol b, tiene como parametro el numero 'a' 
	a agregar */
void ArbolB::agregar(int a){
	
	if( buscar(a) )	return; // busca el numero 'a'

	if( lugarEncontrado ){ // si el lugar encontrado existe
		//se agrega en la pagina hoja encontrada
		lugarEncontrado->actual->agregar(a);
		//se obtiene la casilla que se acaba de agregar
		lugarEncontrado=lugarEncontrado->actual->obtenerLugarEncontrado();
		//se agrega en la lista
		agregarEnLista(lugarEncontrado);
		//s i al agregar la casilla la pagina supera lo permitido entonces
		//la pagina tiene qu edividirse
		if(lugarEncontrado->actual->obtenerCuantos() == orden){
			
			Pagina *padre = lugarEncontrado->actual->obtenerPadre();
			
			Casilla *mitad;
			dividir(mitad,padre);
			while(padre && padre->obtenerCuantos() == orden){
				padre = padre->obtenerPadre();
				dividir(mitad,padre);
			}
		}
		
	}else{ // si no existe lugar encontrado , significa que no hay raiz
		Pagina *pagina = new Pagina(orden-1);
		pagina->agregar(a);
		raiz = pagina;
		agregarEnLista( pagina->obtenerLugarEncontrado() );	
	}
	cuantos++;
}
/* funcion que divide una pagina en dos y sube la mitad hacia el padre si es 
	que lo tiene, tiene como parametros la 'mitad' de la pagina y el 'padre' 
	de la pagina */
//la mitad no es necesaria enviarsela, puede recibir un solo parametro
void ArbolB::dividir(Casilla *mitad,Pagina *padre){
	// obtengo la mitad de la pagina
	mitad= lugarEncontrado->actual->obtenerMitad(); 
	Casilla *aux1,*aux2; 
	Pagina *pagina1,*pagina2;
	// creo dos paginas nuevas
	pagina1 = new Pagina(orden-1);
	pagina2 = new Pagina(orden-1);
	// guardo en la pagina1 las casilla anteriores a la mitad	
	for( aux1 = mitad->anterior ; aux1 ; aux1 = aux2 ){
		aux2 = aux1->anterior;
		pagina1->agregar(aux1);
	}
	// guardo en la pagina2 las casillas posteriores a la mitad
	for( aux1 = mitad->siguiente ; aux1 ; aux1 = aux2 ){
		aux2 = aux1->siguiente;
		pagina2->agregar(aux1);
	}
	//------------------------------------------------------------------------------------------
	if(padre){
		padre->agregar(mitad);// agrega la mitad en el padre
		// obtengo la casilla una vez agregada en el padre
		lugarEncontrado = padre->obtenerLugarEncontrado(); 
		// pongo los hijos a la casilla
		lugarEncontrado->hizq = pagina1; 
		lugarEncontrado->hder = pagina2;
		// pongo el padre de las paginas nuevas
		pagina1->ponerPadre( padre );
		pagina2->ponerPadre( padre );
		//acomodo los hijos de los vecinos de la casilla y pongo el padre 
		//casilla de las paginas nuevas
		if(lugarEncontrado->anterior){
			lugarEncontrado->anterior->hder = lugarEncontrado->hizq;
			pagina1->ponerPadreCasilla( lugarEncontrado->anterior );
		}else{
			pagina1->ponerPadreCasilla(lugarEncontrado);
		}
		if(lugarEncontrado->siguiente) lugarEncontrado->siguiente->hizq = lugarEncontrado->hder;
		pagina2->ponerPadreCasilla(lugarEncontrado);
		// acomodo los hijos de las paginas nuevas
		pagina1->acomodarHijos();
		pagina2->acomodarHijos();
	}else{
		// como no tiene padre, crea una pagina nueva
		Pagina *padreNuevo = new Pagina(orden-1);
		padreNuevo->agregar(mitad); // agrego la mitad a la nueva pagina
		lugarEncontrado = padreNuevo->obtenerLugarEncontrado(); // obtengo la casilla
		// pongo los hijos a la casilla
		lugarEncontrado->hizq = pagina1;
		lugarEncontrado->hder = pagina2;
		// pongo el padre a las nuevas paginas
		pagina1->ponerPadre( padreNuevo );
		pagina2->ponerPadre( padreNuevo );
		pagina1->ponerPadreCasilla(lugarEncontrado);
		pagina2->ponerPadreCasilla(lugarEncontrado);
		// acomodo los hijos de las paginas nuevas
		pagina1->acomodarHijos();
		pagina2->acomodarHijos();
		raiz = padreNuevo; // pongo como raiz a la nueva pagina padre
	}
}
/* funcion que agrega una casilla a la lista del arbol b, tiene como parametro 
	una casilla 'direccion'*/
void ArbolB::agregarEnLista(Casilla *direccion){
	
	if(principio){
		
		// crea una nueva casilla2
		Casilla2 *p = new Casilla2;
		// coloca las direcciones
		p->direccionArbol = direccion;
		direccion->direccionLista = p;
		
		if( (direccion->anterior) && (direccion->siguiente) ){
			
			p->anterior = direccion->anterior->direccionLista;
			p->siguiente = direccion->siguiente->direccionLista;
			p->anterior->siguiente = p;
			p->siguiente->anterior = p;
				
		}else if( (direccion->anterior) && !(direccion->siguiente) ){
			
			p->anterior = direccion->anterior->direccionLista;
			
			if( direccion->anterior->direccionLista->siguiente ){
				p->siguiente = direccion->anterior->direccionLista->siguiente;
				p->anterior->siguiente = p;
				p->siguiente->anterior = p;
			}else{
				p->siguiente = NULL;
				p->anterior->siguiente = p;
				final = p;
			}
		}else if( !(direccion->anterior) && (direccion->siguiente) ){
			
			p->siguiente = direccion->siguiente->direccionLista;
			if( direccion->siguiente->direccionLista->anterior ) {
				p->anterior = direccion->siguiente->direccionLista->anterior;
				p->siguiente->anterior = p;
				p->anterior->siguiente = p;
			}else{
				p->anterior = NULL;
				p->siguiente->anterior=p;
				principio = p;	
			}	
		} 
	}else{ // si no hay principio 
		
		// crea una nueva casilla2 y la pone como principio
		Casilla2 *p = new Casilla2;
		p->direccionArbol = direccion;
		direccion->direccionLista = p;
		p->siguiente = NULL;
		p->anterior = NULL;
		principio = p;
		final = p;
	}
}
//-----------------------------------------------------------------------------------------------------------
bool ArbolB::borrar(int a){
	if ( buscar( a ) ){ // si encuentra el numero
 		Casilla *p,*q,*r;
		p = lugarEncontrado;
		// si tiene hijos	
		if( p->hizq ){	
			// intercambio el valor
			 // q es el anterior de p en la lista
			q = p->direccionLista->anterior->direccionArbol;
			intercambiar(p,q); // intercambia el valor
			p = q; // una vez intercambiado el valor borramos al anterior como pagina hoja
		} 
		// desligo a p de ambas listas	
		desligar(p->direccionLista);
		p->actual->desligar(p);
		// si p se encuentra en la raiz o la pagina de p se quedo con el minimo o mas
		if( p->actual == raiz || p->actual->obtenerCuantos() >= this->minimo ){
			// si la raiz que quedo con 0 casillas entonces ponemos nulo a la raiz
			if( p->actual == raiz && p->actual->obtenerCuantos() == 0) raiz = NULL; 
			cuantos--;
			// borramos ambas casillas
			delete p->direccionLista;
			delete p;
			return true;
		} 
		
		Pagina *vec; // aqui ira la pagina vecina // hizq o hder
		q = p->actual->obtenerPadreCasilla(); // padre de p
//--------------------------NueVA CAPTurA---------------------------------------------------------------------------------	
		//acomoda las variables
		if( q->hder == p->actual  ){
			if( q->siguiente && q->hizq->obtenerCuantos() <= minimo ){
				if(q->siguiente->hder->obtenerCuantos() > minimo ){
					q = q->siguiente; 
					r = q->hder->obtenerPrincipio();				
					vec = q->hder; // r->actual
				}else{
					 // r es la casilla final de la pagina izquierda de q
					r = q->hizq->obtenerFinal();
					vec = q->hizq; // r->actual
				}
				
			}else{
				// r es la casilla final de la pagina izquierda de q
				r = q->hizq->obtenerFinal(); 
				vec = q->hizq; // r->actual
			} 
					
		}else{
			r = q->hder->obtenerPrincipio();				
			vec = q->hder; // r->actual
		}
//-----------------------------------------------------------------------------------------------------------			
		// caso 1 y 2
		if(vec->obtenerCuantos() > minimo){
			// agrego a r ... lo subo para que tome el lugar de q
			// desligo a q y r
			r->actual->desligar(r);
			q->actual->desligar(q);
			q->actual->agregar(r); 
			r->hder = q->hder; 
			r->hder->ponerPadreCasilla(r);
			r->hizq = q->hizq;
			if(!(r->anterior)) r->hizq->ponerPadreCasilla(r);
			// bajo a q en la pagina de p
			p->actual->agregar(q);
			q->hder = NULL;
			q->hizq = NULL;
		}else{ // caso 3
			// borro a q de su pagina 
			q->actual->desligar(q);
			// traspaso las casillas de 'p->actual' a 'vec'
			traspasarCasillas(vec,p->actual);
			//acomodar hijos
			if(q->siguiente && !(q->anterior) && vec == q->hizq){
				q->siguiente->hizq=q->hizq;
				q->hizq->ponerPadreCasilla(q->siguiente);
			}else if(q->siguiente && vec == q->hizq){
				q->siguiente->hizq = q->hizq;
			}else if(q->siguiente && vec == q->hder){
				q->hder->ponerPadreCasilla(q->siguiente);
			}
			vec->agregar(q);
			q->hder = NULL;
			q->hizq = NULL;
//---------------------------------Captura nueva--------------------------------------------------------------------------		
			// lo dificil
			while( vec->obtenerPadre()->obtenerCuantos() < this->minimo ){
				
				// lo nuevo
				if( vec->obtenerPadre() == raiz && vec->obtenerPadre()->obtenerCuantos() == 0){
					raiz = vec;
					vec->ponerPadre(NULL);
					vec->ponerPadreCasilla(NULL);
					cuantos--;
					delete p->direccionLista;
					delete p;
					return true;
				} else if( vec->obtenerPadre() == raiz){
					cuantos--;
					delete p->direccionLista;
					delete p;
					return true;
				}
									
				Casilla *padre;
				padre = vec->obtenerPadre()->obtenerPadreCasilla();
//-----------------------------------------------------------------------------------------------------------				
				// si esta del lado derecho
				if( padre->hder == vec->obtenerPadre() ){
					
					if( padre->hizq->obtenerCuantos() > this->minimo ){//CASO 1
						Casilla *c;
						c = padre->hizq->obtenerFinal();
						c->actual->desligar(c);
						padre->actual->agregar(c);
						padre->actual->desligar(padre);
						padre->hder->agregar(padre);
						
						c->hizq = padre->hizq;
						if(!(c->anterior))c->hizq->ponerPadreCasilla(c);
						padre->hizq = c->hder;
						padre->hizq->ponerPadre(padre->actual);
						padre->hizq->ponerPadreCasilla(padre);
						c->hder = padre->actual;
						c->hder->ponerPadreCasilla(c);
						// lo nuevo
						if(padre->siguiente){
							padre->hder = padre->siguiente->hizq;
							padre->hder->ponerPadreCasilla(padre);	
						}else{
							padre->hder = vec;
							vec->ponerPadreCasilla(padre); 
						}
//-----------------------------------------------------------------------------------------------------------
						//CASO 2
					}else if( padre->siguiente && padre->siguiente->hder->obtenerCuantos() > this->minimo ){ 

						padre= padre->siguiente;
						
						Casilla *c;
						c = padre->hder->obtenerPrincipio();
						c->actual->desligar(c);
						c->hder->ponerPadreCasilla(c->siguiente);
						padre->actual->agregar(c);
						padre->actual->desligar(padre);
						padre->hizq->agregar(padre);
						
						c->hder = padre->hder;
						c->hder->ponerPadreCasilla(c);
						padre->hder = c->hizq;
						padre->hder->ponerPadre(padre->actual);
						padre->hder->ponerPadreCasilla(padre);
						c->hizq = padre->actual;
						// LO NUEVO
						if(padre->anterior){
							padre->hizq = padre->anterior->hder;	
						}else{
							padre->hizq = vec;
							vec->ponerPadreCasilla(padre);
						}
//-----------------------------------------------------------------------------------------------------------								
					}else{//CASO 3
						if(padre->siguiente){
							padre->siguiente->hizq = padre->hizq;
							if(!(padre->anterior))padre->hizq->ponerPadreCasilla(padre->siguiente);
						} 
					
						// LO NUEVO
						traspasarCasillas(padre->hizq,padre->hder);						
						padre->actual->desligar(padre);
						padre->hizq->agregar(padre);
						if(padre->siguiente){
							padre->hder = padre->siguiente->hizq;
							padre->hizq = padre->anterior->hder;
							// los hijos que estan despues del padre tienen que 
							// acomodarse dado que apuntan a la pagina donde estaban
							padre->actual->acomodarHijos();
						}else{
							padre->hder = vec;
							vec->ponerPadre(padre->actual);
							vec->ponerPadreCasilla(padre);
							padre->hizq = padre->anterior->hder;
						}
						
					}
					
//-----------------------------------------------------------------------------------------------------------						
				}else{ // si esta del lado izquierdo //
			
					if( padre->hder->obtenerCuantos() > this->minimo ){//CASO 4
					
						Casilla *c;
						c = padre->hder->obtenerPrincipio();
						c->actual->desligar(c);
						c->hder->ponerPadreCasilla(c->siguiente);
						padre->actual->agregar(c);
						padre->actual->desligar(padre);
						padre->hizq->agregar(padre);
						
						c->hder = padre->hder;
						c->hder->ponerPadreCasilla(c);
						padre->hder = c->hizq;
						padre->hder->ponerPadre(padre->actual);
						padre->hder->ponerPadreCasilla(padre);
						c->hizq = padre->actual;
						c->hizq->ponerPadreCasilla(c);
						if(padre->anterior){
							padre->hizq = padre->anterior->hder;
						}else{
							padre->hizq = vec;
							vec->ponerPadreCasilla(padre);
						}
//-----------------------------------------------------------------------------------------------------------											
					}else{//CASO 5
						
						if(padre->siguiente) padre->hder->ponerPadreCasilla(padre->siguiente);
						// lo nuevo
						traspasarCasillas(padre->hder,padre->hizq);
						
						padre->actual->desligar(padre);
						padre->hder->agregar(padre);
						if(padre->anterior){
							padre->hizq = padre->anterior->hder;
							padre->hder = padre->siguiente->hizq;
							// los hijos que estan antes del padre tienen que 
							// acomodarse dado que apuntan a la pagina donde estaban
							padre->actual->acomodarHijos();
						}else{
							padre->hizq = vec;
							vec->ponerPadre(padre->actual);
							vec->ponerPadreCasilla(padre);
							padre->hder = padre->siguiente->hizq;
							padre->hder->ponerPadreCasilla(padre);	
						}
						
					}
				}
				vec = vec->obtenerPadre();
			} 
			
		}
		cuantos--;
		delete p->direccionLista;
		delete p;
		return true;
		
	}
	//si no encuentra el numero simplemente regresa false, pues no se puede borrar.
	return false;
}
/*  funcion que intercambia los valores de dos casillas, tiene como parametros las dos casillas 'p' y 'q' */
void ArbolB::intercambiar(Casilla *p, Casilla *q){
	
	int valor;
	// intercambiar valor
	valor = p->valor;
	p->valor = q->valor;
	q->valor = valor;
	
}
/* funcion que traspasa casillas, la pagina q traspasa sus casillas a la pagina p
   tiene como parametros la pagina p y q  */
void ArbolB::traspasarCasillas(Pagina *p,Pagina *q){
		
	Casilla *aux;
	aux = q->obtenerPrincipio();
	// paso las casillas a la otra pagina
	while(aux){
		q->desligar(aux); // se borra
		p->agregar( aux ); // se agrega en la pagina r
		aux = q->obtenerPrincipio();
	}

}
/* funcion que desliga a una casilla2 de la lista del arbol b, tiene como parametro la casilla2 'casilla' a desligar */
void ArbolB::desligar(Casilla2 *casilla){
	
	Casilla2 *p = casilla;
	
	if( p->siguiente && p->anterior ){
		p->anterior->siguiente = p->siguiente;
		p->siguiente->anterior = p->anterior;
	}else if( p->anterior && !p->siguiente){
		p->anterior->siguiente = NULL;
		final = p->anterior;
	}else if( !p->anterior && p->siguiente){
		p->siguiente->anterior = NULL;
		principio = p->siguiente;
	}else{
		principio = NULL;
		final = NULL;
	}

}
/* funcion que pinta cada casilla de la lista del arbol con sus caracteristicas*/
void ArbolB::pintarArbol(){
	
	Casilla2 *p;
	p=principio;
	//p=final;
	if(!p)cout<<"No se encuentra ningun numero en el arbol!"<<endl;	
	while(p){
		cout<<"valor: "<< p->direccionArbol->valor <<endl;
		//cout<<"MitadPag: "<< p->direccionArbol->actual->obtenerMitad()->valor <<endl;
		//cout<<"CuantosPag: "<<p->direccionArbol->actual->obtenerCuantos() <<endl;
		cout<<"HIzq: ";
		if( p->direccionArbol->hizq ){
			p->direccionArbol->hizq->pintar();
		}else cout<<"NO TIENE";
		
		cout<<"  HDer: ";
		if( p->direccionArbol->hder ){
			p->direccionArbol->hder->pintar();
		}else cout<<"NO TIENE";
		
		
		if(p->direccionArbol->actual->obtenerPadre()){
			cout<<"  Padre: ";
			p->direccionArbol->actual->obtenerPadre()->pintar();
		}else{
			cout<<"  Padre: No tiene padre es la raiz";	
		}
		
		if(p->direccionArbol->actual->obtenerPadreCasilla()){
			cout<<"  PadreCasilla: "<< p->direccionArbol->actual->obtenerPadreCasilla()->valor;
		}else{
			cout<<"  PadreCasilla: No tiene padre es la raiz";
		}
		
		/*if(p->direccionArbol->actual->obtenerPadreCasilla()){
			cout<<"  PadreCasilla: ";
			cout<< p->direccionArbol->actual->obtenerPadreCasilla()->valor;
		}*/
	
		cout<<endl<<endl;
		p = p->siguiente;
		//p = p->anterior;
	}
	
}
/* funcion que pinta el valor de cada casilla del arbol b*/
void ArbolB::pintar(){
	Casilla2 *p;
	p=principio;
	//p=final;
	if(!p)cout<<"No se encuentra ningun numero en el arbol!"<<endl;	
	while(p){
		if(p->siguiente)cout<<"|"<< p->direccionArbol->valor<<"|-";
		else cout<<"|"<< p->direccionArbol->valor<<"|"<<endl;
		p = p->siguiente;
	}
}
/* funcion que agrega numeros al arbol b por medio de un archivo*/
bool ArbolB::agregarArchivo(){
	int i=0,cantidad,a;
	char NombreArch[30]; //para capturar e nombre del archivo
    ifstream archivo;
    cout<<"Nombre del archivo: ";
    cin>>NombreArch;
    archivo.open(NombreArch);
    if(archivo.good()==false){// if(!Archivo)
        cout<<"Hubo problemas al abrir el archivo..."<<endl;
        system("pause");
        return false;
    }
    archivo>>cantidad;
    if(cantidad<1){//checa si la cantidad de numeros a agregar es mayor que 0
    	cout<<"Para poder agregar tienen que ser mas de 0 numeros!"<<endl;
    }else{
    	while(i<cantidad){
            archivo>>a;
			agregar(a);
		    cout<<a<<" se agrego."<<endl;
            i++;
    	}
    }
    archivo.close();
    return true;
}
/* funcion que guarda cada numero del arbol b de forma ordenada en un archivo*/
bool ArbolB::guardarArchivo(){
	Casilla2 *p;
	int valor;
	p=principio;
	
	char NombreArch[30]; //para capturar e nombre del archivo
    ofstream archivo;
    cout<<"Nombre del archivo: ";
    cin>>NombreArch;
	archivo.open(NombreArch);
	
	if(archivo.good()==false){// if(!Archivo)
        cout<<"Hubo problemas al abrir el archivo..."<<endl;
        system("pause");
        return false;
    }
	
	if(!raiz){
		cout<<"ARBOL VACIO"<<endl;
		return false;
	}
	archivo<<cuantos<<endl;
	while(p){
		valor=p->direccionArbol->valor;
		archivo<<valor<<" ";
		p=p->siguiente;
	}
	archivo.close();
    return true;
}
/* funcion que borra numeros del arbol b mediante un archivo*/
bool ArbolB::borrarArchivo(){
	int i=0,cantidad,a;
	char NombreArch[30]; //para capturar e nombre del archivo
    ifstream archivo;
    cout<<"Nombre del archivo: ";
    cin>>NombreArch;
    archivo.open(NombreArch);
    if(archivo.good()==false){// if(!Archivo)
        cout<<"Hubo problemas al abrir el archivo..."<<endl;
        system("pause");
        return false;
    }
    archivo>>cantidad;
    if(cantidad<1){//checa si la cantidad de numeros a agregar es mayor que 0
    	cout<<"Para poder borrar tienen que ser mas de 0 numeros!"<<endl;
    }else{
    	while(i<cantidad){
            archivo>>a;
			//borrar(a);
			//aqui ponerle un if, si se borro entonces poner esto, si no poner numero no encontrado
			if(borrar(a))cout<<a<<" se borro."<<endl;
			else cout<<a<<" no encontrado."<<endl;
            i++;
    	}
    }
    archivo.close();
    return true;
}
//------------------------------------------------------
//------------------------------------------------------
