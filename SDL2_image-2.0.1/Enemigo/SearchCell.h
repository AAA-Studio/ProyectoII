#ifndef _H_SearchCell_H_
#define _H_SearchCell_H_

#include "checkML.h"
#include <SDL.h>
#include <math.h>

// Tama�o del mundo
#define WORLD_SIZE 80 // El Tama�o del mundo se cambia
// El mundo se divide en celdas donde el enemigo puede estar
#define CELL_SIZE 20  // Cuando lo implementemos sera el tama�o del TILE

class SearchCell
{
public:

	// Cuando se meta en el proyecto se cambia por int que son los tipos de celdas.
	enum CellState {
		CELL_BLOCKED
	};

	

	SearchCell() : pPadre(0){};
	SearchCell(int x, int y, SearchCell * parent = nullptr) : m_xcoord(x), m_ycoord(y),
		pPadre(parent), m_id(y * WORLD_SIZE + x), G(0), H(0){};
	~SearchCell();      /* WORLD_SIZE no vale */

	//GETTERS
	// GetF: Siendo H la distancia optima y G la acumulada,
	// la F es la distancia que se puede recorrer si no se cambia de destino.
	inline float GetF() const { return G + H; }
	inline float GetG() const { return G; }
	inline float GetH() const { return H; }
	inline int GetID() const { return m_id; }
	inline int GetCellX() const { return (m_xcoord / CELL_SIZE); }
	inline int GetCellY() const { return (m_ycoord / CELL_SIZE); }
	inline int GetCellX(float x) const { return floor(x / CELL_SIZE); }
	inline int GetCellY(float y) const { return floor(y / CELL_SIZE); }

	SearchCell * GetParent() { return pPadre;  }

	//SETTERS
	void setX(float x) { m_xcoord = x; }
	void setY(float y) { m_ycoord = y; }
	void SetParent(SearchCell* pParent) { pPadre = pParent; }
	void setH(float distance) { H = distance; }
	void setG(float distAcum) { G = distAcum; }
	void SetID(int id) { m_id = id; }

	// Tiene que devolver un int
	inline CellState GetCellState(float x, float y) const { 

		// de momento, luego devolvera el estado en el que se encuentra la celda
		return CELL_BLOCKED;
	}
	
	
	// �por que ManHattanDistance?: https://es.wikipedia.org/wiki/Geometr%C3%ADa_del_taxista
	float ManHattanDistance(SearchCell * nodeEnd) { // Distancia en la que esta el enemigo al personaje
		float x = (float)(fabsf((this->m_xcoord - nodeEnd->m_xcoord)));
		float z = (float)(fabsf((this->m_ycoord - nodeEnd->m_ycoord)));
		return x + z;
	}
	// ChebySev Distance: https://en.wikipedia.org/wiki/Chebyshev_distance
	float ChebySevDistance(SearchCell * nodeEnd) { // Distancia en la que esta el enemigo al personaje
		float x = (float)(fabsf((this->m_xcoord - nodeEnd->m_xcoord)));
		float z = (float)(fabsf((this->m_ycoord - nodeEnd->m_ycoord)));
		return floor(x - z);
	}

private:
	int m_xcoord, m_ycoord; //Coordenadas del enemigo
	int m_id; //id del enemigo
	SearchCell * pPadre; // puntero a la celda.
	float G; // distancia acumulada desde la primera celda hasta la celda en la que te encuentras
	float H; // distancia estimada al objetivo
	
	
};
#endif
