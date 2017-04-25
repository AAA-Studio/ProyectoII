#include "Mundo.h"
#include <typeinfo>
#include "Personaje.h"
#include <iostream>
#include <SDL.h>
#include "Bala.h"
#include "Boton.h"


Mundo::Mundo(Juego * pJ, string m)
{
	pJuego = pJ;
	pausa = false;
	mapa = new Mapa(this, m);
	initObjetos();
	cargaObjetos();
	//pJuego->getMusica(MPlay)->play();
}


Mundo::~Mundo()
{
	freeObjetos();
}

static void goPlay(Juego * pj){

};
void Mundo::cargaObjetos(){
	string objetos = pJuego->dameObjetos();
	int nivel = pJuego->indiceMapas;
	std::ifstream obj(objetos);
	if (!obj.is_open())
	{
		printf("Unable to load map file!\n");
	}
	hayObj = false;
	bool hecho = false;
	string nombre;
	int i = 1;
	while (obj.peek() != EOF && !hecho){	
		
		
		if (i == 1)
			obj >> nombre;

		int y, x, lvl;
		if (nombre == "NIVEL"){

			obj >> lvl;

			if (nivel == lvl){

				obj >> nombre;

				while (obj.peek() != EOF && nombre != "NIVEL"){
					if (nombre == "LLAVE"){

						obj >> x >> y;
						llaves.push_back(new Entidad(pJuego, x, y, TLlave, ENull, OLlave));
						hayObj = true;
					}
					else if (nombre == "Enemy"){

						obj >> x >> y;
						llaves.push_back(new Entidad(pJuego, x, y, TLlave, ENull, OLlave));
						hayObj = true;

					}

					obj >> nombre;

				}

				hecho = true;
			}
			else{
				obj >> nombre;
				while (obj.peek() != EOF && nombre != "NIVEL"){
					if (nombre == "LLAVE"){
						obj >> x >> y;
					}
					else if (nombre == "Enemy"){
						obj >> x >> y;
					}
					obj >> nombre;
				}
			}
		}
		i++;
	}
	obj.close();
}
//Crea las texturas para los globos y todos los globos
void Mundo::initObjetos()
{
	// Personaje
	//Entidad de prueba para colisiones
	/*
	objetos.push_back(new Boton(pJuego, 500, 500, TPlay, ENull, goPlay));//Puerta
	objetos.push_back(new Entidad(pJuego, 200, 300, TInforme1, ENull, OInforme1));//Informe

	llaves.push_back(new Entidad(pJuego, 400, 300, TLlave, ENull, OLlave));//Llave
	*/
	//al principio del juego



	//HACER UN SWITCH
	int x = 0, y = 0;//Posiciones del jugador para cuando no encuentre el spawn

	if (pJuego->getNivel() == 0){
		x = 300;
		y = 300;
		psj = new Personaje(this, x, y, TJugador, ENull);
	}
	//sale en el spawn gris
	if (pJuego->getNivel() == -1){

		mapa->buscaSpawn(185, x, y);
		psj = new Personaje(this, x, y, TJugador, ENull);
		
	}//spawn rojo
	if (pJuego->getNivel() == 1){

		mapa->buscaSpawn(180, x, y);
		psj = new Personaje(this, x, y, TJugador, ENull);

	}
	//spawn rosa
	if (pJuego->getNivel() == 2){

		mapa->buscaSpawn(191, x, y);
		psj = new Personaje(this, x, y, TJugador, ENull);

	}
	//spawn morado
	if (pJuego->getNivel() == -2){

		mapa->buscaSpawn(184, x, y);
		psj = new Personaje(this, x, y, TJugador, ENull);

	}
	//spawn azul oscuro
	if (pJuego->getNivel() == 3){

		mapa->buscaSpawn(190, x, y);
		psj = new Personaje(this, x, y, TJugador, ENull);

	}
	//spawn pistacho
	if (pJuego->getNivel() == -3){

		mapa->buscaSpawn(188, x, y);
		psj = new Personaje(this, x, y, TJugador, ENull);

	}
	//spawn marr�n
	if (pJuego->getNivel() == 4){

		mapa->buscaSpawn(183, x, y);
		psj = new Personaje(this, x, y, TJugador, ENull);

	}
	//spawn burdeos
	if (pJuego->getNivel() == -4){

		mapa->buscaSpawn(189, x, y);
		psj = new Personaje(this, x, y, TJugador, ENull);

	}
	//spawn verde
	if (pJuego->getNivel() == 5){

		mapa->buscaSpawn(181, x, y);
		psj = new Personaje(this, x, y, TJugador, ENull);

	}
	//spawn azul
	if (pJuego->getNivel() == -5){

		mapa->buscaSpawn(182, x, y);
		psj = new Personaje(this, x, y, TJugador, ENull);

	}

	//objetos.push_back (new Boton(pJuego, 0, 0, TPlay, ENull, goPlay));
}
void Mundo::freeObjetos(){
	delete psj;
	psj = nullptr;

	for (size_t i = 0; i < objetos.size(); i++)//Se destruyen los objetos
	{
		delete(objetos[i]);
		objetos[i] = nullptr;
	}
	list<EntidadJuego*>::const_iterator it = llaves.cbegin();
	for (; it != llaves.cend() && llaves.empty(); it++){
		destruyeLlave(*it);
	}
}


void Mundo::draw()const{

	//Render level
	//DIBUJAR MAPA
	mapa->draw();
	//Dibujar objetos del juego
	for (int i = objetos.size() - 1; i >= 0; i--)
		objetos[i]->draw();

	list<EntidadJuego*>::const_iterator it = llaves.cbegin();
	
	while (!llaves.empty() && it != llaves.cend())
	{
		(*it)->draw();
		it++;

	}

	psj->draw();

	pJuego->getTextura(TFuente)->render(pJuego->getRender(), 50, 50, "HOLA :)", pJuego->getFuente());

}


void Mundo::update(){
	psj->update();//Update de personaje

	for (size_t i = 0; i < objetos.size(); i++)//Update de objetos
		objetos[i]->update();

	list<EntidadJuego*>::const_iterator it = llaves.cbegin();

	while (!llaves.empty() && it != llaves.cend())//Update de las llaves
	{
		(*it)->update();
		it++;

	}


	//ESTO ES PARA ENEMIGO
	/*
	if (checkCollision(psj->getRect(), objetos[0]->getRect()) && pJuego->getLLavesCogidas(0)){//Si el psj colisiona con el enemigo
		
		if (SDL_GetTicks() - time >= duracion)//Se pide la hora y se compara con la �ltima 
		{
			time = SDL_GetTicks();
			psj->restaVida();

		}
		if (psj->getVida() == 0){
			pJuego->borraEstado = true;
			pJuego->estadoEnum = MGameOver;
		}
	}
	*/
}

//Detecta el input del jugador y la pausa
void Mundo::onInput(SDL_Event &e){
	
	//Declaramos el array con los estados de teclado
	const Uint8 * keyStatesActuales = SDL_GetKeyboardState(NULL);
	
	//Pausa
	if (keyStatesActuales[SDL_SCANCODE_ESCAPE]){
		pJuego->borraEstado = true;
		pJuego->estadoEnum = MPausa;
	}
	
	//Personaje
	psj->onInput();

	compruebaPersonaje();
	//psj->setCamera(mapa->getCamera());

}
void Mundo::compruebaPersonaje(){
	SDL_Rect rect, rect2;

	int x, y;

	rect = psj->getCollider();

	rect2.x = psj->DamePosAntX();
	rect2.y = psj->DamePosAntY();

	rect2.h = rect.h = 20;

	x = rect.x - rect2.x;
	y = rect.y - rect2.y;


	Direccion dir = Direccion::Derecha;
	if (x > 0) dir = Direccion::Izquierda;
	psj->setDir(dir);

	//comprueba la X
	if (mapa->touchesWall(rect)){
		rect.x -= x;
	}
	// comprueba la Y
	if (mapa->touchesWall(rect)){
		rect.y -= y;
	}

	psj->setPosChocando(rect.x, rect.y);

}


bool Mundo::checkCollision(SDL_Rect a, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	return !(bottomA <= topB || topA >= bottomB || rightA <= leftB || (leftA >= rightB));

}

EntidadJuego * Mundo::compruebaColisionObjetos(){
	int i = 0;
	
	while (i < objetos.size() && !checkCollision(psj->getCollider(), objetos[i]->getRect()))
		i++;

	//Si lo he encontrado en los informes
	if (i != objetos.size())
		return objetos[i];

	//Si no, sigo buscando en la lista de llaves
	list<EntidadJuego*>::const_iterator it = llaves.cbegin();

	while (!llaves.empty() && it != llaves.cend() && !checkCollision(psj->getCollider(), (*it)->getRect()))
	{
		it++;

	}

	if (it == llaves.cend())
		return nullptr;
	else
		return (*it);

}

void Mundo::destruyeLlave(EntidadJuego * llave)
{
	list<EntidadJuego*>::iterator it = llaves.begin();
	while (it != llaves.end() && (*it) != llave)//Recorre todas las llaves hasta encontrar la llave que tiene que destruir
	{
		it++;
	}

	//Elimina la llave
	llaves.erase(it);
	delete (llave);
	llave = nullptr;

	
	pJuego->setLlaveCogida(0);//Pone a true la llave a eliminar en el array de booleanos de las llaves de juego


}
