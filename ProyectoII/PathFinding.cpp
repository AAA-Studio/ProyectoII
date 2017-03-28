#include "PathFinding.h"


PathFinding::PathFinding(/*Mapa * pGameWorld*/) /*: m_GameWorld(pGameWorld)*/
{
	m_initializedStartGoal = false;
	m_foundGoal = false;
}

void PathFinding::Initialize(Vector2 pStartPos, Vector2 pTargetPos) {
	
	
	m_pathState = INITIALIZE;

	m_openList.clear();
	m_visitedList.clear();
	// m_closesPath.clear();

	SearchCell start;
	start.setX(floorf(pStartPos.GetX() / (float)Grid::GetCellSizeX()));
	start.setY(floorf(pStartPos.GetY() / (float)Grid::GetCellSizeY()));

	SearchCell goal;
	goal.setX(floorf(pTargetPos.GetX() / (float)Grid::GetCellSizeX()));
	goal.setY(floorf(pTargetPos.GetY() / (float)Grid::GetCellSizeY())); 

	SetStartAndGoal(&start, &goal);
}

void PathFinding::FindPath(Vector2 currentPos, Vector2 targetPos) {
	
	if (!m_initializedStartGoal) {

		// Resetea las listas de celdas para elegir el nuevo camino
		for (int i = 0; i < m_openList.size(); i++) {
			delete m_openList[i];
		}
		m_openList.clear();
		
		for (int i = 0; i < m_visitedList.size(); i++) {
			delete m_visitedList[i];
		}
		m_visitedList.clear();

		for (int i = 0; m_pathToGoal.size(); i++) {
			delete m_pathToGoal[i];
		}
		m_pathToGoal.clear();

		// Inicializacion del start
		SearchCell start;

		start.m_xcoord = m_GameWorld->GetCellX(currentPos.m_x);
		start.m_ycoord = m_GameWorld->GetCellY(currentPos.m_y);

		// Inicializacion del Goal
		SearchCell goal;

		goal.m_xcoord = m_GameWorld->GetCellX(currentPos.m_x);
		goal.m_ycoord = m_GameWorld->GetCellY(currentPos.m_y);

		m_foundGoal = false;
		SetStartAndGoal(start, goal);
		m_initializedStartGoal = true;
	}

	else {
		ContinuePath();
	}
}

void PathFinding::SetStartAndGoal(SearchCell start, SearchCell goal){
	
	m_startCell = new SearchCell(start.m_xcoord, start.m_ycoord, NULL);
	m_goalCell = new SearchCell(goal.m_xcoord, goal.m_ycoord, &goal);

	m_startCell->G = 0;
	m_startCell->H = m_startCell->ManHattanDistance(m_goalCell);
	m_startCell->pPadre = nullptr;
	m_openList.push_back(m_startCell);

}

SearchCell * PathFinding::GetNextCell(){
	
	// Comprueba la mejor F de la lista, 
	// con este numero obligamos a que se cambie por lo menos 1 vez
	float bestF = 999999.0f;

	int cellIndex = -1;
	SearchCell * nextCell = nullptr;

	for (int i = 0; i < m_openList.size(); i++) {
		if (m_openList[i]->GetF() < bestF){
			bestF = m_openList[i]->GetF();
			cellIndex = i;
		}
	}

	if (cellIndex >= 0) {
		nextCell = m_openList[cellIndex];
		m_visitedList.push_back(nextCell);
		m_openList.erase(m_openList.begin() + cellIndex);
	}
	return nextCell;
}

void PathFinding::PathOpened(int x, int y, float newCost, SearchCell *pPadre){
	
	// Cuando haya paredes ignora esas celdas
	/*if (m_GameWorld -> GetCellState(x, y) == CELL_BLOCKED)
	{
		return;
	}*/

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// Corregir para que quede como arriba
	if (m_GameWorld->GetCellState(x, y) == m_GameWorld->CELL_BLOCKED)
	{
		return;
	}

	int id = y * WORLD_SIZE + x;
	for (int i = 0; i < m_visitedList.size(); i++) {
		if (id == m_visitedList[i]->m_id) {
			return;
		}
	}

	SearchCell * newChild = new SearchCell(x, y, pPadre);
	newChild->G = newCost;
	newChild->H = pPadre->ManHattanDistance(m_goalCell);

	for (int i = 0; i < m_openList.size(); i++) {
		if (id == id == m_openList[i]->m_id) {

			float newF = newChild->G + newCost + m_openList[i]->H;

			if (m_openList[i]->GetF() > newF) {

				m_openList[i]->G = newChild->G + newCost;
				m_openList[i]->pPadre = newChild;
			}

			else {
				delete newChild;
				return;
			}
		}
	}

	m_openList.push_back(newChild);
}

void PathFinding::ContinuePath() {
	
	if (m_openList.empty()) {
		return;
	}

	SearchCell * currentCell = GetNextCell();

	if (currentCell->m_id == m_goalCell->m_id) {
		m_goalCell->pPadre = currentCell->pPadre;

		SearchCell * getPath;

		// Va por todas las celdas comprobando cual es el camino mas corto y las mete en la lista
		for (getPath = m_goalCell; getPath != NULL; getPath = getPath->pPadre) {

			m_pathToGoal.push_back(new Vector2(getPath->m_xcoord * CELL_SIZE, getPath->m_ycoord));
		}

		m_foundGoal = true;
	}
	else {
		// Celda derecha
		PathOpened(currentCell->m_xcoord + 1, currentCell->m_ycoord, currentCell->G + 1, currentCell);

		//Celda izquierda
		PathOpened(currentCell->m_xcoord - 1, currentCell->m_ycoord, currentCell->G + 1, currentCell);
		
		// Celda superior
		PathOpened(currentCell->m_xcoord, currentCell->m_ycoord - 1, currentCell->G + 1, currentCell);

		// Celda inferior
		PathOpened(currentCell->m_xcoord, currentCell->m_ycoord + 1, currentCell->G + 1, currentCell);

		// Celda diagonal izquierda superior
		PathOpened(currentCell->m_xcoord - 1, currentCell->m_ycoord - 1, currentCell->G + 1.4f, currentCell);

		// Celda diagonal izquierda inferior
		PathOpened(currentCell->m_xcoord - 1, currentCell->m_ycoord + 1, currentCell->G + 1.4f, currentCell);
		
		// Celda diagonal derecha superior
		PathOpened(currentCell->m_xcoord + 1, currentCell->m_ycoord - 1, currentCell->G + 1.4f, currentCell);

		// Celda diagonal derecha inferior
		PathOpened(currentCell->m_xcoord + 1, currentCell->m_ycoord + 1, currentCell->G + 1.4f, currentCell);
		
		for (int i = 0; i < m_openList.size(); i++)
		{	
			if (currentCell->m_id == m_openList[i]->m_id){
				
				m_openList.erase(m_openList.begin() + i);

			}
				
		}
	}
}

Vector2 PathFinding::NextPathPos(Enemigo  *enemigo) {
	
	int index = 1; 

	Vector2 nextPos;
	nextPos.m_x = m_pathToGoal[m_pathToGoal.size() - index]->m_x;
	nextPos.m_y = m_pathToGoal[m_pathToGoal.size() - index]->m_y;

	// pos es la actual posicion del enemigo
	Vector2 distance = nextPos - enemigo->pos;
	if (distance.Length < m_pathToGoal.size()) {

		// Si el enemigo consigue avanzar en el radio de celdas elimina la que ya estaba
		if (distance.Length < enemigo->radius) {
			m_pathToGoal.erase(m_pathToGoal.end() - index);
		}
	}

	return nextPos;
}

void PathFinding::DrawDebug() {
	// static void *****::DrawSquare(int posX, int posY, tColor color) || static void *****::DrawSquare(float posX, float posY, tColor color)

	for (unsigned int i = 0; i < m_openList.size(); i++) {
		m_GameWorld->DrawSquare(m_openList[i]->m_xcoord, m_openList[i]->m_ycoord, m_GameWorld->VERDE/*tColor(0.0f, 1.0f. 0.0f)*/);
	}
	for (unsigned int i = 0; i < m_visitedList.size(); i++) {
		m_GameWorld->DrawSquare(m_visitedList[i]->m_xcoord, m_visitedList[i]->m_ycoord, m_GameWorld->AZUL/*tColor(0.0f, 0.0f. 1.0f)*/);
	}
	for (unsigned int i = 0; i < m_pathToGoal.size(); i++) {
		m_GameWorld->DrawSquare(m_pathToGoal[i]->m_x, m_pathToGoal[i]->m_y, m_GameWorld->ROJO/*tColor(1.0f, 0.0f. 0.0f)*/);
	}
}

PathFinding::~PathFinding()
{
	// Clear();
}
