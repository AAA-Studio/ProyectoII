#include "Estado.h"


Estado::Estado(Juego * pJ)
{
	pJuego = pJ;
}


Estado::~Estado()
{
	freeObjetos();
}

void Estado::freeObjetos(){
	for (int i = 0; i < objetos.size(); i++)
	{
		delete(objetos[i]);
		objetos[i] = nullptr;
	}
}

void Estado::update()
{
	for (int i = 0; i < objetos.size(); i++)
		objetos[i]->update();

}


//Limpia el buffer y dibuja los objetos
void Estado::draw() const
{
	//Dibujar objetos del juego
	for (int i = objetos.size() - 1; i >= 0; i--)
		objetos[i]->draw();

}

//Pide a cada objeto que compruebe si lo han pinchado (m�ximo uno)
void Estado::onClick()
{
	int i = 0;
	bool click = false;
	while (!click && i < objetos.size())
	{
		if (objetos[i]->onClick())
			click = true;
		i++;
	}

}

