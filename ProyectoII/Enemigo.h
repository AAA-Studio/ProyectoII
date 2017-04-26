#ifndef Enemigo_H
#define Enemigo_H
#include "checkML.h"
#include "Entidad.h"
#include "PersonajeVirtual.h"
#include <list>
#include "MundoVirtual.h"


class Enemigo : public Entidad, PersonajeVirtual
{
public:
	Enemigo(MundoVirtual*pJ, int x, int y,int w,int h, Texturas_t textura, Efectos_t efecto);
	~Enemigo();
	virtual void update();
	inline void restaVida() { vida--; };
	inline float getVida()const { return vida; };

private:
	MundoVirtual* pMundo;
	void disparo();
	int vida;
	const float tiempoBala = 1000;
	Uint32 ultimaBala;
	int angulo;

};

#endif