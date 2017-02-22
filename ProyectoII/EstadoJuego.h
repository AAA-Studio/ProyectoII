#ifndef EstadoJuego_H
#define EstadoJuego_H
#include "checkML.h"

class EstadoJuego
{
	//M�todos publicos abstractos

public:

	EstadoJuego(){}

	virtual ~EstadoJuego(){}

	virtual void draw() const = 0;

	virtual void update() = 0;

	virtual void onInput(SDL_Event &e) = 0;

};

#endif

