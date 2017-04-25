#include "Combinaciones.h"
#include "Boton.h"
#include <iostream>
using namespace std;


static void cero(Juego* ju){ ju->setNumero(0); }
static void uno(Juego* ju){ ju->setNumero(1); }
static void dos(Juego* ju){ ju->setNumero(2); }
static void tres(Juego* ju){ ju->setNumero(3); }
static void cuatro(Juego* ju){ ju->setNumero(4); }
static void cinco(Juego* ju){ ju->setNumero(5); }
static void seis(Juego* ju){ ju->setNumero(6); }
static void siete(Juego* ju){ ju->setNumero(7); }
static void ocho(Juego* ju){ ju->setNumero(8); }
static void nueve(Juego* ju){ ju->setNumero(9); }
static void salir(Juego* ju){ ju->popState(); }

Combinaciones::Combinaciones(Juego* juego, string combinacion, int puerta) : Menu(juego)
{
	acierto = false;
	this->combinacion = combinacion;
	this->puerta = puerta;
	intentos = 0;
	combTecleada = "";
	initObjetos();
}

void Combinaciones::update(){
	
	for (int i = 0; i < objetos.size(); i++)
		objetos[i]->update();

	if (combinacion == combTecleada)
	{
		acierto = true;
	}

	if (intentos == 4)
	{
		combTecleada = "";
		intentos = 0;
	}

	if (acierto)
	{
		pJuego->setPuerta(puerta, acierto);
		pJuego->popState();		
	}
}
void Combinaciones::initObjetos(){
	objetos.emplace_back(new Boton(pJuego, 600, 500, TCero, ENull, cero));
	objetos.emplace_back(new Boton(pJuego, 500, 200, TUno, ENull, uno));
	objetos.emplace_back(new Boton(pJuego, 600, 200, TDos, ENull, dos));
	objetos.emplace_back(new Boton(pJuego, 700, 200, TTres, ENull, tres));
	objetos.emplace_back(new Boton(pJuego, 500, 300, TCuatro, ENull, cuatro));
	objetos.emplace_back(new Boton(pJuego, 600, 300, TCinco, ENull, cinco));
	objetos.emplace_back(new Boton(pJuego, 700, 300, TSeis, ENull, seis));
	objetos.emplace_back(new Boton(pJuego, 500, 400, TSiete, ENull, siete));
	objetos.emplace_back(new Boton(pJuego, 600, 400, TOcho, ENull, ocho));
	objetos.emplace_back(new Boton(pJuego, 700, 400, TNueve, ENull, nueve));
	objetos.emplace_back(new Boton(pJuego, 900, 200, TCero, ENull, salir));
	}

void Combinaciones::draw() const{
	for (int i = objetos.size() - 1; i >= 0; i--)
		objetos[i]->draw();
	
}

void Combinaciones::onInput(SDL_Event &e){

	if (e.type == SDL_MOUSEBUTTONUP)
	{
		if (e.button.button == SDL_BUTTON_LEFT)
		{
			for (int j = 0; j < objetos.size(); j++)
			{
				objetos[j]->onInput();
			}
			int i = 0;
			bool pulsado = false;
			
			while (i < objetos.size() && !pulsado)
			{
				//objetos[i]->onInput();
				if (i == 10)
					pulsado == true;
				else if (pJuego->getNumero() == i)
				{
					combTecleada += to_string(pJuego->getNumero());
					intentos++;
					pulsado = true;
					cout << " intentos: " + intentos;
					cout << " combinacion: " + combTecleada;
				}
				
				i++;
			}
			pulsado = false;
			i = 0;
		}
	}

}

Combinaciones::~Combinaciones()
{
}