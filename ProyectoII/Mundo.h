#ifndef Mundo_H
#define Mundo_H
#include "checkML.h"
#include "MundoVirtual.h"
#include "EntidadJuego.h"
#include "Juego.h"

#include <SDL.h>
#include "Mapa.h"
#include "Personaje.h"
#include "Enemigo.h"
#include <list>


using namespace std;

//Clase abstracta que hereda de la raiz ObjetoJuego e implementa utilidades para las subclases
class Mundo : public MundoVirtual
{
public:
	Mundo(Juego * pJ, string mapa);
	virtual ~Mundo();

	virtual void draw() const;
	virtual void onInput(SDL_Event &e);
	virtual void update();
	bool checkCollision(SDL_Rect a, SDL_Rect b);
	inline void añadeObjeto(EntidadJuego * obj){ objetos.push_back(obj); };
	EntidadJuego * compruebaColisionObjetos();
	void destruyeLlave(EntidadJuego * llave);
	void destruyeBala(list <EntidadJuego*> & lista, list<EntidadJuego*>::iterator & it);

		//------------------GETTERS---------------------
		inline Mapa* getMapa() const { return mapa; };
	inline Juego* getPJ() const{ return pJuego; };
	list<EntidadJuego*> getListaBalas(ListaBalas_t lista) const
	{
		if (lista == LBalasPersonaje)
			return balasPsj;
		else
			return balasEnems;
	};

	void insertaBala(ListaBalas_t lista, EntidadJuego * bala);

	void compruebaPersonaje();

	void cambiaPosPSJ(int x,int y){
		psj->setPosChocando(x, y);
	}

	inline virtual SDL_Rect getCamera() const{ return camera; };

	virtual void setCamera(int x, int y){ camera.x = x; camera.y = y; };


private:
	//-------------------ATRIBUTOS---------------
	Juego * pJuego;
	Personaje * psj;
	bool abierto;
	vector<bool> cerraduras;
	vector<int> puertas;
	vector <EntidadJuego*> objetos;
	list <EntidadJuego*> llaves;
	list <Enemigo*> enemigos;
	list <EntidadJuego*> balasPsj;
	list <EntidadJuego*> balasEnems;
	Mapa * mapa;
	bool pausa, balaDestruida;
	const Uint32 duracion = 500;
	Uint32 time;
	SDL_Rect camera;

	//-------------------METODOS-------------------
	void cargaObjetos();
	void initObjetos();
	void freeObjetos();
	void colBalaEnemigo();
	void colBalaPersonaje();
};

#endif
