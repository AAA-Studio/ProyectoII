#include "Personaje.h"


//Constructora
Personaje::Personaje(Juego*pJ, int x, int y, Texturas_t textura, Efectos_t efecto) : Entidad(pJ, x, y, textura, efecto)
{
}

//Destructora
Personaje::~Personaje()
{
}


//Actualiza el estado y devuelve false si el globo queda desinflado
void Personaje::update()
{

	
}

void Personaje::onInput(const char& c)
{


}

