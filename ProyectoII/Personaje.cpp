#include "Personaje.h"
#include <iostream>
#include "Mundo.h"
#include "Bala.h"
#include "Armas.h"

//Constructora
Personaje::Personaje(MundoVirtual * pM, int x, int y, JuegoSDL::Texturas_t textura, JuegoSDL::Efectos_t efecto) : Entidad(pM->getPJ(), x, y, 30, 40, textura, efecto, ONull)
{
	pMundo = pM;
	rect = { x, y, 31, 50 };
	rectAn = { 0, 128, 31, 50 };
	rectInforme = { pJuego->getWindowWidth() / 4, pJuego->getWindowHeight() / 20, 300, 600 };
	rectLlave = { 50, pJuego->getWindowWidth() - 100, 100, 100 };
	rectHUD = { 0, 0, 800, 640 };
	ultimaBala = ultimoInput = SDL_GetTicks();
	balaDestruida = false;
	llaveCogida = false;
	informeCogido = false;
	vida = 5;
	angulo = 90;
	empuje = false;
	posXAnt = x;
	posYAnt = y;
	retardo = 0;
	arma = nullptr;
	cinematica = false;
}

//Destructora
Personaje::~Personaje()
{
}

void Personaje::update()
{
	//pMundo->setCamera(rect.x - SCREEN_WIDTH / 2, rect.y - SCREEN_HEIGHT / 2); 
	if (pMundo->getIndiceMapa()  < 24 && !informeCogido && !cinematica && pierdesVida)
		vida -= 0.0005;

}


void Personaje::draw(int x, int y)const
{
	//Tiene animaci�n, sobrescribe la herencia
	pJuego->getResources()->getTextura(textura)->draw(pJuego->getRender(), rect,x,y, &rectAn);//Dibujamos la textura


	if (informeCogido)
		pJuego->getResources()->getTextura(informe)->draw(pJuego->getRender(), rectInforme, rectInforme.x, rectInforme.y, nullptr);

	if (pMundo->getLLavesCogidas(0))
		pJuego->getResources()->getTextura(JuegoSDL::TLlave)->draw(pJuego->getRender(), rectLlave, 50, SCREEN_HEIGHT - 100, nullptr);
	if (pMundo->getLLavesCogidas(1))
		pJuego->getResources()->getTextura(JuegoSDL::TLlave)->draw(pJuego->getRender(), rectLlave, 150, SCREEN_HEIGHT - 100, nullptr);
}

void Personaje::animacion(animar currentFrame){
	retardo++;
	if (retardo == 5){
		switch (currentFrame){
		case Personaje::derecha:
			rectAn.y = 192;
			retardo = 0;
			break;
		case Personaje::izquierda:
			rectAn.y = 64;
			retardo = 0;
			break;
		case Personaje::arriba:
			rectAn.y = 0;
			retardo = 0;
			break;
		case Personaje::abajo:
			rectAn.y = 128;
			retardo = 0;
			break;
		default:
			break;
		}
		frames();
		
	}
	
}

void Personaje::frames(){
	if (rectAn.x >= 240){
		rectAn.x = 0;
	}
	else{
		rectAn.x += 30;
	}
}
void Personaje::onInput()
{
	int x = 0, y = 0;
	const Uint8 * keyStatesActuales = SDL_GetKeyboardState(NULL);

	if (!informeCogido && !empuje)
	{
		int x = 0, y = 0;

		if (keyStatesActuales[SDL_SCANCODE_UP] && keyStatesActuales[SDL_SCANCODE_LEFT])
		{
			y += -1;
			x += -1;
			animacion(izquierda);
			angulo = 135;
		}
		else if (keyStatesActuales[SDL_SCANCODE_UP] && keyStatesActuales[SDL_SCANCODE_RIGHT])
		{
			y += -1;
			x += 1;
			animacion(derecha);
			angulo = 45;

		}
		else if (keyStatesActuales[SDL_SCANCODE_DOWN] && keyStatesActuales[SDL_SCANCODE_LEFT])
		{
			y += 1;
			x += -1;
			animacion(izquierda);
			angulo = 225;
		}
		else if (keyStatesActuales[SDL_SCANCODE_DOWN] && keyStatesActuales[SDL_SCANCODE_RIGHT])
		{
			y += 1;
			x += 1;
			animacion(derecha);
			angulo = 315;
		}
		else if (keyStatesActuales[SDL_SCANCODE_UP])
		{
			y += -1;
			animacion(arriba);
			angulo = 90;
		}
		else if (keyStatesActuales[SDL_SCANCODE_LEFT])
		{
			x += -1;
			animacion(izquierda);
			angulo = 180;
		}
		else if (keyStatesActuales[SDL_SCANCODE_DOWN])
		{
			y += 1;
			animacion(abajo);
			angulo = 270;
		}
		else if (keyStatesActuales[SDL_SCANCODE_RIGHT])
		{
			x += 1;
			animacion(derecha);
			angulo = 0;
		}
		//Disparo
		if (keyStatesActuales[SDL_SCANCODE_SPACE])
		{
			disparo();
		}
		//Caso en el que se coge un objeto
		if (keyStatesActuales[SDL_SCANCODE_E]){
			if (SDL_GetTicks() - ultimoInput >= tiempoInput)//Se pide la hora y se compara con la �ltima 
			{
				coger();
				ultimoInput = SDL_GetTicks();
			}
		}


		move(x, y);
	}

	else if (informeCogido)
	{
		if (SDL_GetTicks() - ultimoInput >= tiempoInput)//Se pide la hora y se compara con la �ltima 
		{
			if (keyStatesActuales[SDL_SCANCODE_E]){
				soltarInforme();
				ultimoInput = SDL_GetTicks();
			}
		}

	}

	else if (empuje)
	{
		move(-dir.x, -dir.y);

		if (SDL_GetTicks() - ultimoEmpuje >= tiempoEmpuje)//Se pide la hora y se compara con la �ltima 
			empuje = false;

	}
}
void Personaje::setPosChocando(int x, int y){
	rect.x = x;
	rect.y = y;
}
void Personaje::move(int x, int y)
{

	posXAnt = rect.x;
	posYAnt = rect.y;

	rect.x += 3*x;
	rect.y += 3*y;
}
void Personaje::disparo(){

	if (arma != nullptr && arma->getBalas() > 0 && pMundo->getIndiceMapa() >23){
		if (SDL_GetTicks() - ultimaBala >= arma->getCadencia() )//Se pide la hora y se compara con la �ltima 
		{
			pMundo->insertaBala(LBalasPersonaje, new Bala(pMundo, rect.x + 5, rect.y + 20, rect.w / 10, rect.h / 10, JuegoSDL::TBala, JuegoSDL::ENull, angulo, LBalasPersonaje));
			arma->restaBalas();
			ultimaBala = SDL_GetTicks();
			pJuego->getResources()->getEfecto(0)->play(0);
			pMundo->getTexturaBala()->loadFromText(pJuego->getRender(), "Balas : " + to_string(getBalas()), { 255, 255, 255, 1 }, *pMundo->getFuente());
		
		}
	}
}
//no hace falta
void Personaje::empujeHaciaAtras(){
	empuje = true;
	ultimoEmpuje = SDL_GetTicks();
	restaVida(0); //whaaaaat
}
void Personaje::restaVida(float da�o){
	vida -= da�o;
}
void Personaje::coger(){
	EntidadJuego * objeto;
	objeto = pMundo->compruebaColisionObjetos();//Compruebo si estoy colisionando con el obj para poder cogerlo
	if (objeto != nullptr){
		switch (objeto->getType())
		{
		case OInforme1:
			informe = JuegoSDL::TInforme1;
			informeCogido = true;
			pJuego->getResources()->getEfecto(8)->play(0);
			break;
		case OInforme2:
			informe = JuegoSDL::TInforme2;
			informeCogido = true;
			pJuego->getResources()->getEfecto(8)->play(0);
			break;
		case OInforme3:
			informe = JuegoSDL::TInforme3;
			informeCogido = true;
			pJuego->getResources()->getEfecto(8)->play(0);
			break;
		case OInforme4:
			informe = JuegoSDL::TInforme4;
			informeCogido = true;
			pJuego->getResources()->getEfecto(8)->play(0);
			break;
		case OInforme5:
			informe = JuegoSDL::TInforme5;
			informeCogido = true;
			pJuego->getResources()->getEfecto(8)->play(0);
			break;
		case OInforme6:
			informe = JuegoSDL::TInforme6;
			informeCogido = true;
			pJuego->getResources()->getEfecto(8)->play(0);
			break;
		case OInforme7:
			informe = JuegoSDL::TInforme7;
			informeCogido = true;
			pJuego->getResources()->getEfecto(8)->play(0);
			break;
		case OLlave:
			pMundo->destruyeLlave(objeto);
			pJuego->getResources()->getEfecto(9)->play(0);
			break;
		case OTeclado:
			pJuego->setBorraEstado(true);
			pJuego->setEstadoEnum(ECombinaciones);
			break;
		case OPistola:
			pMundo->destruyeArma();
			pJuego->getResources()->getEfecto(7)->play(0);
			break;
		case OPildoras:
			pMundo->pildoraCogida();
			pJuego->getResources()->getEfecto(7)->play(0);
			break;
		case OInterruptor1:
			pMundo->setPulsado();
			pJuego->getResources()->getEfecto(5)->play(0);
			//pMundo->destruyeInterruptor();
			break;
		case OInterruptor2:
			pMundo->setPulsado2();
			pJuego->getResources()->getEfecto(5)->play(0);
			//pMundo->destruyeInterruptor();
			break;
		}
	}
}
void Personaje::cogeArma(Armas* arma){
	//Borra el arma anterior y a�ade la currente
	delete this->arma;
	this->arma = nullptr;
	this->arma = arma;
}
void Personaje::soltarInforme(){
	informeCogido = false;
	informe = JuegoSDL::Texturas_t_SIZE;
}

void Personaje::sumaVida(int vida){
	if (vida + this->vida > 5)
		this->vida = 5;
	else
		this->vida += vida;
}
int Personaje::getBalas(){
	if (arma != nullptr){
		return arma->getBalas();
	}
	else
		return 0;
}





