#ifndef Mapa_H
#define Mapa_H
#include <SDL.h>
#include "Tile.h"
#include "MundoVirtual.h"
#include <string>

class Mapa
{
public:
	Mapa(MundoVirtual * pM, string mapa);
	~Mapa();

	bool touchesWall(SDL_Rect box);
	bool touchesDoor(SDL_Rect box, int& tipo);

	void draw()const;
	void buscaSpawn();

	//--------------------GETTER----------------
	inline Tile** getTileMap() { return tileMap; };
	inline int getXSpawn() const{ return x; };
	inline int getYSpawn() const{ return y; };

	//--------------------SETTER----------------
	inline void setPulsado(bool p){ encendido = p; };
	inline void setPulsado2(bool p){ encendido2 = p; };

private:
	MundoVirtual * pMundo;
	Juego * pJuego;
	string nombreMapa;
	Tile* tileMap[TOTAL_TILES];
	bool Puerta1Abierta;

	bool encendido = false;	//para abrir la primera puerta 
	bool encendido2 = false; //para abrir la segunda puerta
	bool pulsados(); //metodo que comprueba si los 2 botones han sido pulsados

	int x, y;//Variables en las que se guarda la pos del Spawn del personaje
	//int nivel;

	bool cargarMapa();
	
	void setCamera();//Pone la c�mara en el centro de cada nivel cuando se pasa de una sala a otra
};
#endif
