#ifndef _H_PathFinding_H_
#define _H_PathFinding_H_

#include "checkML.h"
#include "SearchCell.h"
#include "MathCore.h"
#include "Entidad.h"
#include "Enemigo.h"
// #include "Mapa.h"
#include <vector>
using namespace std;

/*
Pathfinding por NathPoints para que sea rapido
Hay que ver si hay linea recta hacia el personaje, buscar lo que hay que esquivar
NathPoints, hago una lista de nathpoints y eso es lo que comprueba el enemigo
*/

class PathFinding
{
public:
	PathFinding(/*Mapa * pGameWorld*/);
	~PathFinding();

	// Estado en el que puede estar en cada iteracion el Pathfinding
	enum State {
		INITIALIZE, 
		SEARCHING, 
		ERROR_GOAL_NOT_FOUND,
		FOUND_GOAL, // He encontrado objetivo, asi que voy hacia el.
		NONE,
	};

	// Este metodo utiliza las coordenadas del enemigo y las del jugador para crear el camino optimo
	void FindPath(Vector2 currentPos, Vector2 targetPos);
	// Devuelve la posicion del camino mas corto
	Vector2 NextPathPos(Enemigo *enemigo);
	// Limpia las diferentes listas
	void ClearOpenList() { m_openList.clear(); }
	void ClearVisitedList() { m_visitedList.clear(); }
	void ClearPathToGoal() { m_pathToGoal.clear(); }

	void Initialize(Vector2 pStartPos, Vector2 pTargetPos);

	// Comprueba que se han inicializado el personaje
	bool m_initializedStartGoal;

	// Encontrada la celda del personaje
	bool m_foundGoal;

	State GetpathState() { return m_pathState; }

	Vector2 GetNextClosesPoint();

	int GetClosesPathSize();

	void Iterate();

	void Clear();

	vector<Vector2> GetClosesPath();

private:

	void SetStartAndGoal(SearchCell start, SearchCell goal);

	// Comprueba que la actual celda esta en m_openList
	void PathOpened(float x, float y, float newCost, SearchCell *pPadre);

	// Busca a lo largo del mundo el Goal para seguir en el camino optimo
	void ContinuePath();

	void InitializaStartGoal(SearchCell* pStart, SearchCell* pGoal);

	// Mapa * _pGameWorld;

	SearchCell * GetNextCell();

	// Primera celda donde se encuentra el enemigo
	SearchCell * m_startCell;

	// Celda donde se encuentra el personaje
	SearchCell * m_goalCell;

	// Mapa * _pGameWorld;
	SearchCell * m_GameWorld;

	SearchCell * iter;

	// Celdas sin visitar
	vector <SearchCell*> m_openList;

	// Celdas visitadas
	vector <SearchCell*> m_visitedList;

	// Camino optimo al personaje
	vector <Vector2 *> m_pathToGoal;

	// Camino mas corto
	vector <Vector2> m_closesPaths;

	State m_pathState;

};
#endif

