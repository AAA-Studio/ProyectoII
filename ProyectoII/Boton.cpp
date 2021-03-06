#include "Boton.h"
#include <iostream>

Boton::Boton(Juego*pJ, int x, int y, int w, int h, JuegoSDL::Texturas_t textura, JuegoSDL::Efectos_t efecto, CallBack_t * cb) 
	: Entidad(pJ, x, y, w, h, textura, efecto, ONull)
, cboton(cb){}

Boton::~Boton(){}

void Boton::onInput(){
	int posMouseX, posMouseY;
	pJuego->getMousePos(posMouseX, posMouseY);

	//Comprobamos si la pos del rat�n coincide con la pos de la textura
	if (dentro(posMouseX, posMouseY))
		cboton(pJuego);
}

bool Boton::dentro(int x, int y)const
{
	return (x >= rect.x && x <= (rect.x + rect.w) && y >= rect.y && y <= (rect.y + rect.h));
}

