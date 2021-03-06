#ifndef _H_Combinaciones_H_
#define _H_Combinaciones_H_
#include "Menu.h"
#include "EntidadJuego.h"
#include "Juego.h"
#include <string>
#include "MundoVirtual.h"

class Combinaciones : public Menu
{
public:
	Combinaciones(Juego* juego, string combinacion, int puerta);
	~Combinaciones(){};

	virtual void draw() const;
	virtual void update();
	virtual void onInput(SDL_Event &e);

private:
	void initObjetos();
	string combTecleada; //combinacion tecleada por el jugador
	string combinacion; //combinacion correcta
	bool acierto;
	int puerta;
	int boton;
	int intentos;
	bool enterPulsado;
	void pulsaEnter(); //metodo que se llama al pulsar el bot�n enter
	void pulsaSalir(); //metodo que se llama al pulsar el boton salir
	std::vector<JuegoSDL::Texturas_t> texturas;

	TexturasSDL textFuente, textIndicacion;
	Fuente* font_;
};
#endif

