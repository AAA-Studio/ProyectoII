#include "Mundo.h"
#include <typeinfo>
#include "Personaje.h"
#include <iostream>
#include <SDL.h>
#include "Bala.h"
#include "Boton.h"
#include <fstream>

Mundo::Mundo(Juego * pJ, string m)
{
	pJuego = pJ;

	nivel = 0;
	indiceMapa = 1;

	pausa = false;
	camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	mapa = new Mapa(this, m);
	initObjetos();

	cargaObjetos();
	
	balaDestruida = false;

	//pJuego->getMusica(MPlay)->play();

	for (int i = 0; i < TAMA�O_LLAVES; i++) //Se inicializan las llaves
		llavesCogidas[i] = false;

}


Mundo::~Mundo()
{
	freeObjetos();

}

void Mundo::cargaObjetos(){
	string objts = pJuego->getNombreObjetos();


	std::ifstream obj(objts);
	if (!obj.is_open())
	{
		printf("Unable to load map file!\n");
	}
	string nombre;
	int i = 1;
	int y, x, w, h, lvl, tipo, ancho, alto;
	alto = 640;
	while (obj.peek() != EOF){


		if (i == 1)
			obj >> nombre;


		int y, x,w,h, lvl, tipo,cadencia,balas;
		if (nombre == "NIVEL"){

			obj >> lvl;
			obj >> nombre;

			if (lvl < 24)
				ancho = 0;
			else
				ancho = 800;


			while (obj.peek() != EOF && nombre != "NIVEL"){
				if (nombre == "LLAVE"){
					obj >> x >> y >> w >> h;
					llaves.push_back(new Entidad(pJuego, x + ancho, y + alto*(lvl % 24), w, h, JuegoSDL::TLlave, JuegoSDL::ENull, OLlave));
				}
				else if (nombre == "INFORME"){
					obj >> x >> y >> w >> h >> tipo;
					if (tipo == 1)

						objetos.push_back(new Entidad(pJuego, x + ancho, y + alto*(lvl % 24), w, h, JuegoSDL::TInforme1, JuegoSDL::ENull, OInforme1));//Informe

					else if (tipo == 2)
						objetos.push_back(new Entidad(pJuego, x + ancho, y + alto*(lvl % 24), w, h, JuegoSDL::TInforme2, JuegoSDL::ENull, OInforme2));//Informe
				}
				else if (nombre == "PANEL"){

					obj >> x >> y >> w >> h;
					objetos.push_back(new Entidad(pJuego, x + ancho, y + alto*(lvl % 24), w, h, JuegoSDL::TTeclado, JuegoSDL::ENull, OTeclado));
				}
				else if (nombre == "ARMA"){

					obj >> x >> y >> w >> h >> balas >> cadencia;
					armas.push_back(new Armas(pJuego, x + ancho, y + alto*(lvl % 24), w, h, balas, cadencia, JuegoSDL::TPistola, JuegoSDL::ENull, OPistola));
				}
				else if (nombre == "ENEMIGO"){

					obj >> x >> y >> w >> h;
					enemigos.push_back(new Enemigo(this, x + ancho, y + alto*(lvl % 24), w, h, JuegoSDL::TLeon, JuegoSDL::ENull));

				}

				obj >> nombre;

			}
		}
		i++;
	}
	obj.close();
}
static void goPlay(Juego * pj){

};

//Crea las texturas para los globos y todos los globos
void Mundo::initObjetos()
{	

	int x = 360;
	int y = 900;
	psj = new Personaje(this, x, y, JuegoSDL::TJugador, JuegoSDL::ENull);
		
		

			//HACER UN SWITCH
			/*int x = 0, y = 0;//Posiciones del jugador para cuando no encuentre el spawn

			x = mapa->getXSpawn();
			y = mapa->getYSpawn();
			psj = new Personaje(this, x, y, JuegoSDL::TJugador, JuegoSDL::ENull);
			//objetos.push_back(new Entidad(pJuego, 350, 300, TTeclado, ENull, OTeclado));
			//llaves.push_back(new Entidad(pJuego, 400, 300, TLlave, ENull, OLlave));//Llave
			//enemigos.push_back(new Enemigo(this, x + 100, y + 100, TLeon, ENull));


			//objetos.push_back (new Boton(pJuego, 0, 0, TPlay, ENull, goPlay));*/
		
}

	void Mundo::freeObjetos(){
		delete psj;
		psj = nullptr;

		for (size_t i = 0; i < objetos.size(); i++)//Se destruyen los objetos
		{
			delete(objetos[i]);
			objetos[i] = nullptr;
		}

		list<Enemigo*>::iterator itEnemigo = enemigos.begin();
		while (!enemigos.empty() && itEnemigo != enemigos.end())
		{
			delete(*itEnemigo);
			itEnemigo = enemigos.erase(itEnemigo);
		}

		list<EntidadJuego*>::const_iterator it = llaves.cbegin();
		for (; it != llaves.cend() && llaves.empty(); it++){
			destruyeLlave(*it);
		}

		//Balas
		list<EntidadJuego*>::iterator itBalasPsj = balasPsj.begin();
		while (!balasPsj.empty() && itBalasPsj != balasPsj.end())
		{
			delete(*itBalasPsj);
			itBalasPsj = balasPsj.erase(itBalasPsj);
		}

		list<EntidadJuego*>::iterator itBalasEnem = balasEnems.begin();
		while (!balasEnems.empty() && itBalasEnem != balasEnems.end())
		{
			delete(*itBalasEnem);
			itBalasEnem = balasEnems.erase(itBalasEnem);
		}
		list<Armas*>::iterator itArmas = armas.begin();
		while (!armas.empty() && itArmas != armas.end())
		{
			delete(*itArmas);
			itArmas = armas.erase(itArmas);
		}

	}


	void Mundo::draw()const{

			//Render level
			//DIBUJAR MAPA
			mapa->draw();

			//Dibujar objetos del juego
			list<Armas*>::const_iterator itArmas = armas.begin();
			while (!armas.empty() && itArmas != armas.end())
			{
				(*itArmas)->draw((*itArmas)->getRect().x - camera.x, (*itArmas)->getRect().y - camera.y);
				itArmas++;
			}
			for (int i = objetos.size() - 1; i >= 0; i--)
				objetos[i]->draw(objetos[i]->getRect().x - camera.x, objetos[i]->getRect().y - camera.y);

			list<EntidadJuego*>::const_iterator it = llaves.cbegin();

			while (!llaves.empty() && it != llaves.cend())
			{
				(*it)->draw((*it)->getRect().x - camera.x, (*it)->getRect().y - camera.y);
				it++;
			}

			list<Enemigo*>::const_iterator itEnemigos = enemigos.cbegin();
			while (!enemigos.empty() && itEnemigos != enemigos.cend())
			{
				(*itEnemigos)->draw((*itEnemigos)->getRect().x - camera.x, (*itEnemigos)->getRect().y - camera.y);
				itEnemigos++;
			}
			psj->draw(psj->getRect().x - camera.x, psj->getRect().y - camera.y);
			/*
			//Balas
			list<EntidadJuego*>::const_iterator itBalasPsj = balasPsj.cbegin();
			while (!balasPsj.empty() && itBalasPsj != balasPsj.cend())
			{
			(*itBalasPsj)->draw();
			itBalasPsj++;
			}*/

			for (auto bala : balasPsj) {
				bala->draw(bala->getRect().x - camera.x, bala->getRect().y - camera.y);
			}

			/*list<EntidadJuego*>::const_iterator itBalasEnem = balasEnems.cbegin();
			while (!balasEnems.empty() && itBalasEnem != balasEnems.cend())
			{
			(*itBalasEnem)->draw();
			itBalasEnem++;
			}*/

			for (auto bala : balasEnems) {
				bala->draw(bala->getRect().x - camera.x, bala->getRect().y - camera.y);
			}


			pJuego->getResources()->getTextura(JuegoSDL::TBlood)->setAlpha(255 - psj->getAlpha());
			pJuego->getResources()->getTextura(JuegoSDL::TBlood)->draw(pJuego->getRender(), psj->getHUD(), 0, 0, nullptr);
			SDL_Rect a = getCamera();
			a.h = 200;
			a.w = 400;
			pJuego->getResources()->getTextura(JuegoSDL::TControles)->draw(pJuego->getRender(), a, 0, 0, nullptr);
			//pJuego->escribir("HOLA :)",50, 50);
		
	}


	void Mundo::update(){
		psj->update();//Update de personaje
		balaDestruida = false;
		if (indiceMapa == 0 && !(psj->getempiezaPerderVida()))
			psj->empiezaPerderVida();
		if (psj->getVida() <= 0){
			pJuego->setEstadoEnum(MGameOver);
			pJuego->setBorraEstado(true);
		}
		//Balas
		list<EntidadJuego*>::const_iterator itBalasPsj = balasPsj.cbegin();
		while (!balaDestruida && !balasPsj.empty() && itBalasPsj != balasPsj.cend())
		{
			balaDestruida = false;
			if (checkCollision(camera, (*itBalasPsj)->getRect())){
				(*itBalasPsj)->update();
				if (!balaDestruida && mapa->touchesWall((*itBalasPsj)->getRect())){
					delete (*itBalasPsj);
					itBalasPsj = balasPsj.erase(itBalasPsj);
					balaDestruida = true;
				}
				else
					itBalasPsj++;
			}
			else
				itBalasPsj++;
		}

		list<EntidadJuego*>::iterator itBalasEnem = balasEnems.begin();
		while (!balaDestruida && !balasEnems.empty() && itBalasEnem != balasEnems.end())
		{
			if (checkCollision(camera, (*itBalasEnem)->getRect())){
				balaDestruida = false;
				(*itBalasEnem)->update();
				if (!balaDestruida)//Si no ha sido destruida en el update, avanzo
					itBalasEnem++;
				else
					itBalasEnem = balasEnems.erase(itBalasEnem);
			}
			else
				itBalasEnem++;
		}

		list<Enemigo*>::const_iterator citEnemigo = enemigos.cbegin();//Update de enemigos
		while (!enemigos.empty() && citEnemigo != enemigos.cend())
		{
			if (checkCollision(camera, (*citEnemigo)->getRect())){
				(*citEnemigo)->update();
			}
			citEnemigo++;
		}


		for (size_t i = 0; i < objetos.size(); i++)//Update de objetos
			if (checkCollision(camera,objetos[i]->getRect()))
				objetos[i]->update();


		list<EntidadJuego*>::const_iterator cit = llaves.cbegin();
		while (!llaves.empty() && cit != llaves.cend())//Update de las llaves
		{
			if (checkCollision(camera, (*cit)->getRect())){
				(*cit)->update();
			}
				cit++;
		}

		//COLISIONES
		colBalaEnemigo();
		colBalaPersonaje();
		
	}

	void Mundo::colBalaEnemigo(){

		list<Enemigo*>::iterator itEnemigo = enemigos.begin();

		//Recorremos los enemigos
		while (!enemigos.empty() && itEnemigo != enemigos.cend())
		{
			list<EntidadJuego*>::iterator it = balasPsj.begin();
			if (checkCollision(camera, (*itEnemigo)->getRect())){
				//Recorremos las balas
				while (!balasPsj.empty() && it != balasPsj.cend())
				{
					//Detectamos la colision de la bala con el enemigo
					if (checkCollision((*it)->getRect(), (*itEnemigo)->getRect()))
					{
						(*itEnemigo)->restaVida();
						destruyeBala(balasPsj, it);

						//Caso en el que el enemigo muere
						if ((*itEnemigo)->getVida() <= 0)
						{
							//Recorremos los enemigos para saber cual tiene que eliminarse
							delete (*itEnemigo);
							itEnemigo = enemigos.erase(itEnemigo);
						}
					}
					else
						it++;
				}
				//Incrementamos el iterador si la lista de enemigos no est� vac�a
				if (!enemigos.empty())
					itEnemigo++;
			}
			else
				itEnemigo++;
		}
	}


	void Mundo::colBalaPersonaje(){

		list<EntidadJuego*>::iterator itBalasEnems = balasEnems.begin();

		//Colision balas con personaje 
		while (!balasEnems.empty() && itBalasEnems != balasEnems.end())
		{
			//Si el psj colisiona con el enemigo
			if (checkCollision(psj->getRect(), (*itBalasEnems)->getRect())){
				//Se pide la hora y se compara con la �ltima 
				if (SDL_GetTicks() - time >= duracion)
				{
					time = SDL_GetTicks();
					psj->restaVida();
				}

				destruyeBala(balasEnems, itBalasEnems);

				if (psj->getVida() <= 0){
					pJuego->setBorraEstado(true);
					pJuego->setEstadoEnum (MGameOver);
				}
			}
			else
				itBalasEnems++;
		}
	}

	//Detecta el input del jugador y la pausa
	void Mundo::onInput(SDL_Event &e){

		

			//Declaramos el array con los estados de teclado
			const Uint8 * keyStatesActuales = SDL_GetKeyboardState(NULL);

			//Pausa
			if (keyStatesActuales[SDL_SCANCODE_ESCAPE]){
				pJuego->setEstadoEnum (MPausa);
			}

			//Personaje
			psj->onInput();
			compruebaPersonaje();

		
		

	}
	void Mundo::compruebaPersonaje(){
		SDL_Rect rect, rect2, rect3;

		int x, y;

		rect.x = psj->getX();
		rect.y = psj->getY();

		rect2.x = psj->DamePosAntX();
		rect2.y = psj->DamePosAntY();

		rect2.w = rect.w = rect2.h = rect.h = 20;

		//Rect3 = rect de colision
		rect3.h = 10;
		rect3.w = 10;

		x = rect.x - rect2.x;
		y = rect.y - rect2.y;

		rect3.x = rect.x + 10;
		rect3.y = rect.y + 40;


		Direccion dir;
		dir.x = x;
		dir.y = y;
		psj->setDir(dir);

		int tipo;
		mapa->touchesDoor(rect3, tipo);

		//comprueba la X
		if (mapa->touchesWall(rect3)){
			rect.x -= x;
		}
		// comprueba la Y
		if (mapa->touchesWall(rect3)){
			rect.y -= y;
		}

		if (tipo != 0
			&& tipo != 1
			&& tipo != 2
			&& tipo != 3
			&& tipo != 4
			&& tipo != 5
			&& tipo != 6
			&& tipo != 7
			&& tipo != 16
			&& tipo != 17
			&& tipo != 18
			&& tipo != 19
			&& tipo != 20
			&& tipo != 21
			&& tipo != 22
			&& tipo != 27
			&& tipo != 28
			&& tipo != 29
			&& tipo != 30
			&& tipo != 31
			&& tipo != 32
			&& tipo != 33
			&& tipo != 42
			&& tipo != 43
			&& tipo != 44
			&& tipo != 54
			&& tipo != 55
			&& tipo != 57
			&& tipo != 58
			&& tipo != 59
			&& tipo != 114
			&& tipo != 140
			&& tipo != 150
			&& tipo != 151
			&& tipo != 152
			&& tipo != 153
			&& tipo != 154
			&& tipo != 155
			&& tipo != 157
			&& tipo != 158
			&& tipo != 159
			&& tipo != 165
			&& tipo != 169
			&& tipo != 174
			&& tipo != 180
			&& tipo != 181
			&& tipo != 182
			&& tipo != 183
			&& tipo != 184
			&& tipo != 185
			&& tipo != 186
			&& tipo != 187
			&& tipo != 188
			&& tipo != 189
			&& tipo != 190
			&& tipo != 191
			&& tipo != 281
			&& tipo != 282
			&& tipo != 338
			&& tipo != 339
			&& tipo != 340
			//------------------------------------------------------------------------------------//
			//                                      MUNDO OSCURO								  //
			//------------------------------------------------------------------------------------//

			&& tipo != 345
			&& tipo != 346
			&& tipo != 347
			&& tipo != 348
			&& tipo != 349
			&& tipo != 350
			&& tipo != 351
			&& tipo != 352
			&& tipo != 361
			&& tipo != 362
			&& tipo != 363
			&& tipo != 364
			&& tipo != 365
			&& tipo != 366
			&& tipo != 367
			&& tipo != 372
			&& tipo != 373
			&& tipo != 374
			&& tipo != 375
			&& tipo != 376
			&& tipo != 377
			&& tipo != 378
			&& tipo != 387
			&& tipo != 388
			&& tipo != 389
			&& tipo != 399
			&& tipo != 400
			&& tipo != 402
			&& tipo != 403
			&& tipo != 404
			&& tipo != 459
			&& tipo != 485
			&& tipo != 495
			&& tipo != 496
			&& tipo != 497
			&& tipo != 498
			&& tipo != 499
			&& tipo != 500
			&& tipo != 501
			&& tipo != 502
			&& tipo != 503
			&& tipo != 504
			&& tipo != 510
			&& tipo != 514
			&& tipo != 519
			&& tipo != 525
			&& tipo != 526
			&& tipo != 527
			&& tipo != 528
			&& tipo != 529
			&& tipo != 530
			&& tipo != 531
			&& tipo != 532
			&& tipo != 533
			&& tipo != 534
			&& tipo != 535
			&& tipo != 536
			&& tipo != 626
			&& tipo != 627
			&& tipo != 683
			&& tipo != 684
			&& tipo != 685)
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
		size_t i = 0;
		SDL_Rect rect, rect3;

		rect.x = psj->getX();
		rect.y = psj->getY();

		//Rect3 = rect de colision
		rect3.h = 10;
		rect3.w = 10;
		rect3.x = rect.x + 10;
		rect3.y = rect.y + 40;


		while (i < objetos.size() && !checkCollision(psj->getRect(), objetos[i]->getRect()))
			i++;

		//Si lo he encontrado en los informes
		if (i != objetos.size())
			return objetos[i];

		//Si no, sigo buscando en la lista de llaves
		list<EntidadJuego*>::const_iterator it = llaves.cbegin();

		while (!llaves.empty() && it != llaves.cend() && !checkCollision(psj->getRect(), (*it)->getRect()))
		{
			it++;

		}
		list<Armas*>::const_iterator itArmas = armas.cbegin();

		while (!armas.empty() && itArmas != armas.cend() && !checkCollision(psj->getRect(), (*itArmas)->getRect()))
		{
			itArmas++;
		}
		if (it == llaves.cend() && itArmas == armas.cend())
			return nullptr;
		else if (itArmas != armas.cend())
			return (*itArmas);
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
		it = llaves.erase(it);
		delete (llave);
		llave = nullptr;
		pJuego->getResources()->getEfecto(7)->play(0);
			

		if (getLLavesCogidas(0))
			setLlaveCogida(1);//Pone a true la llave a eliminar en el array de booleanos de las llaves de juego
		else
			setLlaveCogida(0);//Pone a true la llave a eliminar en el array de booleanos de las llaves de juego
	}
	void Mundo::ponmeArma(){
		list<Armas*>::iterator it = armas.begin();
		while (it != armas.end() && !checkCollision((*it)->getRect(),psj->getRect()))//Recorre todas las llaves hasta encontrar la llave que tiene que destruir
		{
			it++;
		}
		psj->cogeArma((*it));
		it = armas.erase(it);
		pJuego->getResources()->getEfecto(7)->play(0);
	}
	void Mundo::destruyeBala(list <EntidadJuego*> & lista, list<EntidadJuego*>::iterator & it)
	{
		delete (*it);
		it = lista.erase(it);
		balaDestruida = true;
	}

	void Mundo::insertaBala(ListaBalas_t lista, EntidadJuego * bala)
	{
		if (lista == LBalasPersonaje)
			balasPsj.push_back(bala);
		else
			balasEnems.push_back(bala);
	}


