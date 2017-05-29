#include "Enemigo2.h"
#include "Personaje.h"
using namespace std;

Enemigo2::Enemigo2(MundoVirtual* pM, int x, int y, int w, int h, JuegoSDL::Texturas_t textura, JuegoSDL::Efectos_t efecto) : Entidad(pM->getPJ(), x, y, w, h, textura, efecto, ONull)
{
	pMundo = pM;
	this->x = x;
	this->y = y;
	vida = 3;
	maxX = 100;
	direccion = true;
	pasivo = true;
	volviendo = false;
}

void Enemigo2::update(){
	checkPersonaje(); //comprobacion de la posicion del personaje

	if (volviendo && !pasivo)
		volver();
	else if (pasivo)
		patrulla();
	else
		ataque();
}

void  Enemigo2::patrulla(){
	cout << "patrullando";
	if (direccion) //camina hacia la derecha
	{
		rect.x += 1;
		if (rect.x >= x + maxX)
			direccion = false;
	}
	else //camina hacia la izquierda
	{
		rect.x -= 1;
		if (rect.x <= x)
			direccion = true;
	}
	
}

void Enemigo2::ataque(){
	SDL_Rect rectPJ = pMundo->getPersonaje()->getRect();
	angulo = atan2((float)(rect.y - (rectPJ.y + rectPJ.h)), -(float)(rect.x - (rectPJ.x + rectPJ.w / 2))) * 180 / 3.14;
	incrX = cos(angulo);
	incrY = -sin(angulo);

	rect.x += incrX;
	rect.y += incrY;
}

void Enemigo2::volver(){
	cout << "volviendo";
	if (rect.y > y)
		rect.y -= 1;
	else if (rect.y < y)
		rect.y += 1;

	if (rect.x > x)
		rect.x -= 1;
	else if (rect.x < x)
		rect.x += 1;

	if (rect.y == y && rect.x == x)
	{
		
		pasivo = true;
		volviendo = false;
		cout << "he vuelto";
	}
}

void Enemigo2::checkPersonaje(){
	SDL_Rect rectPJ = pMundo->getPersonaje()->getRect();
	//si el jugador est� dentro de un radio de 100 el enemigo le detecta
	if (rectPJ.x <= rect.x + 100 && rectPJ.x >= rect.x - 100 && rectPJ.y <= rect.y + 100 && rectPJ.y >= rect.y - 100)
	{
		pasivo = false;
		cout << "pillado";
		volviendo = false;
	}
	else
	{
		volviendo = true;
	}
}
Enemigo2::~Enemigo2()
{
}
