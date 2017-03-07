class Pagina;

// representa la casilla que va en cada una de las listas de cada pagina
struct Casilla{
	int valor; // el numero que se guarda en cada casilla
	Pagina *hizq,*hder; // sus hijos los cuales son paginas
	Pagina *actual; // es la pagina en la que se encuentra
	Casilla *siguiente,*anterior; // el siguiente y el anterior de la casilla en
	// la lista de la pagina
	Casilla2 *direccionLista; // es la direccion a la lista, la cual esta 
	//ordenada en el arbol b
};

class Pagina{
	
	int n; // maximo de casillas que caben en una pagina
	// punteros a las casilla importantes de cada lista
	Casilla *principio,*final,*anterior,*mitad,*lugarEncontrado; 
 	Pagina *padre; // padre de la casilla, apunta a la pagina padre
 	Casilla *padreCasilla; // padre casilla de la pagina
	int cuantos; // el numero de casillas en la pagina
	Donde donde; // la localizacion de la casilla a la que se busca
	EsMenor esMenor; // se usa para saber si el valor agregado es menor o mayor 
	//que la mitad
	int menores,mayoresOiguales; // 'menores' son la cantidad de casillas antes 
	//de la mitad y 'mayoresOiguales' son la cantidad de casilla que son mayores
	// que la mitad y la mitad misma  
	
public:
	
	Pagina(int orden); // constructor de la pagina
	~Pagina(); // destructor de la pagina 
	void terminar(); // copia del destructor
	bool buscar(int a); // se usa para buscar en la lists
	Pagina* buscar2(int a); // se usa para buscar en el arbol
	bool agregar(int a); // se usa para agregar un numero a la lista
	void agregar(Casilla *p); // se usa para agregar una casilla a la lista
	void pintar(); // pinta la lista
	void acomodarHijos(); // acomoda los hijos de cada casilla de la pagina
	void desligar(Casilla *casilla); // desliga una casilla de la pagina
	int obtenerCuantos(); // regresa el numero de casillas de la lista
	Casilla* obtenerLugarEncontrado(); // regresa el lugar encontrado
	Casilla* obtenerMitad(); // regresa la mitad
	Pagina* obtenerPadre(); // regresa la pagina padre
	void ponerPadre(Pagina *padre); // asigna el padre a la pagina		 
	Casilla* obtenerPrincipio(); // regresa el principio
	Casilla* obtenerFinal(); // regresa el final
	Casilla* obtenerPadreCasilla(); // regresa la casilla padre de una pagina
	void ponerPadreCasilla(Casilla *p); // pone la casilla padre de la pagina
};

/* constructor de la pagina, tiene como parametro el 'orden' la pagina,
   el cual es el valor maximo de casillas que puede tener una pagina */
Pagina::Pagina(int orden){
	this->n = orden;
	cuantos = 0;
	principio = NULL;
	final = NULL;
	anterior = NULL;
	lugarEncontrado = NULL;
	mitad = NULL;
	donde = VACIO;
	padre = NULL;
	esMenor = NO;
	menores =0;
	mayoresOiguales = 0;
	padreCasilla=NULL;
}

/* destructor de la pagina */
Pagina::~Pagina(){

	Casilla *p;
	while(principio){
		p = principio;
		principio = p -> siguiente;
		delete p;
	}
	anterior = NULL;
	mitad = NULL;
	final = NULL;
	lugarEncontrado = NULL;
	donde = VACIO;
	cuantos = 0;
	n = 0;
	padre = NULL;
	esMenor = NO;
	menores =0;
	mayoresOiguales = 0;
	padreCasilla=NULL;
}
/* funcion copia del destructor */
void Pagina::terminar(){
	
	Casilla *p;
	while(principio){
		p = principio;
		principio = p -> siguiente;
		delete p;
	}
	anterior = NULL;
	mitad = NULL;
	final = NULL;
	lugarEncontrado = NULL;
	donde = VACIO;
	cuantos = 0;
	n = 0;
	padre = NULL;
	esMenor = NO;
	menores =0;
	mayoresOiguales = 0;
	padreCasilla=NULL;
}

/*  funcion que regresa la mitad de la lista de la pagina  */
Casilla* Pagina::obtenerMitad(){
	return this->mitad;
}

/*  funcion que regresa la pagina padre de la pagina */
Pagina* Pagina::obtenerPadre(){
	return this->padre;
}
/*  funcion que pone un padre a la pagina */
void Pagina::ponerPadre(Pagina *padre){
	this->padre = padre;
}
/*  funcion que regresa el principio de la pagina */
Casilla* Pagina::obtenerPrincipio(){
	return this->principio;
}
/* funcion que regresa el final de la pagina*/
Casilla* Pagina::obtenerFinal(){
	return this->final;
}

/* funcion que busca un numero en la lista de la pagina , tiene como parametro el numero 'a'
   a buscar, regresa un booleano dependiendo si lo encontro o no lo encontro (true o false) */
bool Pagina::buscar(int a){	

	Casilla *p;
	
	if(principio){
		
		if(a >= mitad->valor){
			p = mitad;
			anterior = p->anterior;
			esMenor =  NO;
		}else{
			p = principio;
			anterior = NULL;
			esMenor = SI;
		}
		
		while(p){
			
			if( (p->valor) == a){
		
				if(principio == p){
					donde = PRINCIPIO;
				}else if( p->siguiente == NULL){
					donde = FINAL;
				}else{
					donde = ENMEDIO;
				}
				
				return true;
			
			}else if( (p->valor) < a){
				anterior = p;
				p = p -> siguiente;
				
			}else{
				
				if(principio == p){
					donde = PRINCIPIO;
				}else{
					donde = ENMEDIO;
				} 
				return false;
			}
				
		}
		
	}else{
		
		donde = VACIO;
		anterior = NULL;
		esMenor = NO;
		return false;
				
	}
	donde = FINAL;
	return false;

}

/* funcion que buscar un numero en la lista de la pagina, tiene como argumento 
	el numero 'a'a buscar, regresa la pagina donde seguira buscando hizq o hder,
	o actual si es que ya encontro el numero */
Pagina* Pagina::buscar2(int a){
	
	Casilla *p;

	if(principio){
		if(a >= mitad -> valor){
			p = mitad;
			//anterior = p -> anterior;
		}else{
			p = principio;
			//anterior = NULL;
		}
		while(p){	
			if( (p -> valor) == a){
				// si encontro el numero regresa la casilla y la pagina 
				lugarEncontrado = p;// la casilla 
				return p->actual;// la pagina
			
			}else if( (p -> valor) < a){
				//anterior = p;
				p = p -> siguiente;
			
			}else{
				
				// si no entrontro el numero, regresa la casilla padre y la 
				//pagina donde seguira buscando
				lugarEncontrado = p;// la casilla "padre"
				return p->hizq;// la pagina hijo
			}
		}
		
	}else{
		lugarEncontrado = NULL;
		return NULL;
	}
	// si no entrontro el numero, regresa la casilla padre y la pagina donde 
	//seguira buscando
	lugarEncontrado = final; // la casilla "padre"
	return final->hder; // la pagina hijo
}

/* funcion que agrega un numero a la lista de la pagina, tiene como parametro 
	el numero 'a' a agregar regresa un booleano dependiendo si se pudo agregar 
	o no */
bool Pagina::agregar(int a){
	
	Casilla *p;
	
	if( buscar(a) ) return false;

	p = new Casilla;
	p->valor = a;
	p->actual = this;
	p->hder = NULL;
	p->hizq = NULL;
		
	if(donde == VACIO){
		p -> siguiente = NULL;
		p -> anterior = NULL;
		principio = p;
		final = p;
	} else if( donde == PRINCIPIO){
		p -> siguiente = principio;
		p -> anterior = NULL;
		principio -> anterior = p;
		principio = p;
	}else if(donde == FINAL){
		p -> siguiente = NULL;
		p -> anterior = final;
		final -> siguiente = p;
		final = p;
	} else {
		p -> siguiente = anterior -> siguiente;
		p -> anterior = anterior;
		(anterior -> siguiente) -> anterior = p;
		anterior -> siguiente = p;
	}
	
	
	if( !(principio->siguiente)  ){
		mitad = p;
		esMenor = SI;
		mayoresOiguales++;
	}else{
	
		if(esMenor == SI){
			menores++;
		}else{
			mayoresOiguales++;
		}
	
		if( (menores + 2) == mayoresOiguales){
			mitad = mitad -> siguiente;
			menores++;
			mayoresOiguales--;
		}else if(menores > mayoresOiguales){
			mitad = mitad -> anterior;
			menores--;
			mayoresOiguales++;
		}
	}
	cuantos++;
	lugarEncontrado = p;
	return true;
	
}

/* funcion que agrega una casilla a la lista de la pagina, tiene como parametro la casilla 'p' a agregar */
void Pagina::agregar(Casilla *p){
	
	buscar(p->valor);

	p->actual = this;
	
	if(donde == VACIO){
		p -> siguiente = NULL;
		p -> anterior = NULL;
		principio = p;
		final = p;
	
	} else if( donde == PRINCIPIO){
		
		p -> siguiente = principio;
		p -> anterior = NULL;
		principio -> anterior = p;
		principio = p;
		
	}else if(donde == FINAL){
		p -> siguiente = NULL;
		p -> anterior = final;
		anterior -> siguiente = p;
		final = p;
		
	} else {
		
		p -> siguiente = anterior -> siguiente;
		p -> anterior = anterior;
		(anterior -> siguiente) -> anterior = p;
		anterior -> siguiente = p;
	}
	
	
	if( !(principio->siguiente) ){
		mitad = p;
		esMenor = SI;
		mayoresOiguales++;
	}else{
	
		if(esMenor == SI){
			menores++;
		}else{
			mayoresOiguales++;
		}
	
		if( (menores + 2) == mayoresOiguales){
			mitad = mitad -> siguiente;
			menores++;
			mayoresOiguales--;
		}else if(menores > mayoresOiguales){
			mitad = mitad -> anterior;
			menores--;
			mayoresOiguales++;
		}
	}
	cuantos++;
	lugarEncontrado = p;
}

/* funcion que pinta los numero de la lista de la pagina*/
void Pagina::pintar(){
	
	Casilla *p;
	p = principio;
	if(principio){
		
		while(p){
			cout << p -> valor <<" ";
			p = p -> siguiente;
		}
	}else{
		cout<<"Pagina Vacia!";
	}
}

/* funcino que regresa la cantidad de casillas de la pagina*/
int Pagina::obtenerCuantos(){
	return this->cuantos;
}
/* funcion que regresa el lugar encontrado de la pagina*/
Casilla* Pagina::obtenerLugarEncontrado(){
	return lugarEncontrado;
}

/* funcion que acomoda los hijos de las casillas de cada pagina, esta se usa cuando se divide
   una pagina y se tengan que acomodar los hijos de nuevo*/
void Pagina::acomodarHijos(){
	
	if(principio){
		Casilla *p;
		p = principio;
		if(p->hizq ){
			p->hizq->ponerPadre(this);
			p->hizq->ponerPadreCasilla(p);
		} 
		
		while(p){
			
			if(p->hder ){
				p->hder->ponerPadre(this);
				p->hder->ponerPadreCasilla(p);
			}
			 
			p = p->siguiente;
				
		}
		
	}
	
}

/* funcion que desliga una casilla de la lista de la pagina, tiene como parametro 
   la casilla 'casilla' a desligar*/
void Pagina::desligar(Casilla *casilla){	
	
	Casilla *p = casilla;
	
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
	
	if( p->valor >= mitad->valor ) esMenor = NO;
	else esMenor = SI;
 	
	if(esMenor == SI){
		menores--;
	}else{
		mayoresOiguales--;	
	} 
	
	if( (menores + 2) == mayoresOiguales){
		mitad = mitad->siguiente;
		menores++;
		mayoresOiguales--;
	}else if(menores > mayoresOiguales){
		mitad = mitad->anterior;
		menores--;
		mayoresOiguales++;
	}else if(menores == mayoresOiguales && p == mitad ){
		mitad = mitad->siguiente;
	}
	cuantos--;
	
}
/* funcion que regresa la casilla padre una una pagina 'hijo' */
Casilla* Pagina::obtenerPadreCasilla(){
	return this->padreCasilla;
}
/* funcion que pone el padreCasilla de una pagina*/
void Pagina::ponerPadreCasilla(Casilla *p){
	this->padreCasilla = p;
}

 

