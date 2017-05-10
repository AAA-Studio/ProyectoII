#ifndef MundoVirtual_H
#define MundoVirtual_H
#include "checkML.h"
#include "EstadoJuego.h"
#include "EntidadJuego.h"
#include "Juego.h"
#include <list>


//Clase abstracta que hereda de la raiz EstadoJuego
class MundoVirtual : public EstadoJuego
{
public:
	virtual bool checkCollision(SDL_Rect a, SDL_Rect b) = 0;

	virtual void añadeObjeto(EntidadJuego * obj) = 0;

	virtual EntidadJuego * compruebaColisionObjetos() = 0;

	virtual Juego* getPJ()const = 0;

	virtual list<EntidadJuego*> getListaBalas(ListaBalas_t lista) const = 0;

	virtual void destruyeLlave(EntidadJuego * llave) = 0;

	virtual void destruyeBala(list <EntidadJuego*> &lista, list<EntidadJuego*>::iterator & it) = 0;

	virtual void insertaBala(ListaBalas_t lista, EntidadJuego * bala) = 0;

	virtual void ponmeArma()=0;
};

#endif