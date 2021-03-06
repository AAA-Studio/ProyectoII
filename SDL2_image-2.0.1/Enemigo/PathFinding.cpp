#include "PathFinding.h"
#include <stdio.h> // Input and Output operations
// #include "TileManager.h"

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

	// Celda del enemigo
	SearchCell start;
	start.setX(floorf(pStartPos.GetX() / (float)1/*TileManager::TILE_WIDTH*/));
	start.setY(floorf(pStartPos.GetY() / (float)1/*TileManager::TILE_HEIGHT*/));

	// Celda objetivo
	SearchCell goal;
	goal.setX(floorf(pTargetPos.GetX() / (float)1/*TileManager::TILE_WIDTH*/));
	goal.setY(floorf(pTargetPos.GetY() / (float)1/*TileManager::TILE_HEIGHT*/));

	
	InitializaStartGoal(&start, &goal);

	m_pathState = SEARCHING;
}

void PathFinding::InitializaStartGoal(SearchCell* pStart, SearchCell* pGoal) {
	// if's para asegurar que se inicializan las celdas correspondientes
	if (!m_startCell) {
		
		m_startCell = new SearchCell(pStart->GetCellX(), pStart->GetCellY(), NULL);
	}

	if (!m_goalCell) {

		m_goalCell = new SearchCell(pGoal->GetCellX(), pGoal->GetCellY(), pGoal);
	}

	// Informacion de las celdas anteriores, coordenadas e id
	m_startCell->setX(pStart->GetCellX()); 
	m_startCell->setY(pStart->GetCellY());
	m_startCell->SetID(pStart->GetCellY() * /*TileManager::MAP_WIDTH*/ + pStart->GetCellX());

	m_goalCell->setX(pGoal->GetCellX());
	m_goalCell->setY(pGoal->GetCellY());
	m_goalCell->SetID(pGoal->GetCellY() * /*TileManager::MAP_WIDTH*/ + pGoal->GetCellX());

	// El padre del objetivo es la celda que ocupa
	m_goalCell->SetParent(pGoal);

	m_startCell->setH(m_startCell->ManHattanDistance(m_goalCell));
}

void PathFinding::Iterate() {

	if (m_openList.empty()) {
		m_pathState = ERROR_GOAL_NOT_FOUND;
		return;
	}
	// Siguiente celda a la que se dirige el enemigo, "avanzadilla"
	SearchCell * currentCell = GetNextCell();

	// Justo la celda siguiente a la que estamos es el personaje
	if (currentCell->GetID() == m_goalCell->GetID()) {
		m_pathState = FOUND_GOAL;

		m_goalCell->SetParent(currentCell);

		// Movimiento sin algoritmos, el iter es el personaje y guardas el camino hacia el objetivo
		for (SearchCell * iter = m_goalCell; iter; iter = iter->GetParent()) {
			m_closesPaths.push_back(Vector2(iter->GetCellX() * (float)/*(TileManager::TILE_WIDTH/2.0f)*/1, 
				iter->GetCellY() * (float)/*(TileManager::TILE_HEIGHT/2.0f)*/1));
		}
	}

	else {

		float x = currentCell->GetCellX();
		float y = currentCell->GetCellY();
		float g = currentCell->GetG();

		// comprueba izq
		PathOpened(x - 1, y, g + 1, currentCell);
		// comprueba drcha
		PathOpened(x + 1, y, g + 1, currentCell);
		// comprueba arriba
		PathOpened(x, y - 1, g + 1, currentCell);
		// comprueba abajo
		PathOpened(x, y + 1, g + 1, currentCell);

		// Borramos las celdas de la openlist porque hemos generado otros Path.
		for (int i = 0; i < (int)m_openList.size(); i++) {
			if (currentCell->GetID() == m_openList[i]->GetID()) {
				m_openList.erase(m_openList.begin() + i);
			}
		}
	}

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

		start.setX(m_GameWorld->GetCellX(currentPos.m_x));
		start.setY(m_GameWorld->GetCellY(currentPos.m_y));

		// Inicializacion del Goal
		SearchCell goal;

		goal.setX(m_GameWorld->GetCellX(currentPos.m_x));
		goal.setY(m_GameWorld->GetCellY(currentPos.m_y));

		m_foundGoal = false;
		SetStartAndGoal(start, goal);
		m_initializedStartGoal = true;
	}

	else {
		ContinuePath();
	}
}

void PathFinding::SetStartAndGoal(SearchCell start, SearchCell goal){
	
	m_startCell = new SearchCell(start.GetCellX(), start.GetCellY(), NULL);
	m_goalCell = new SearchCell(goal.GetCellX(), goal.GetCellY(), &goal);

	// Doy informacion a la nueva Celda, 
	// !!!!!!!!!!!!! No se crean celdas como tal, solo son para comprobar la informacion
	m_startCell->setG(0);
	m_startCell->setH(m_startCell->ManHattanDistance(m_goalCell));
	m_startCell->SetParent(nullptr);
	m_openList.push_back(m_startCell);

}

SearchCell * PathFinding::GetNextCell(){
	
	// Comprueba la mejor F de la lista, 
	// con este numero obligamos a que se cambie por lo menos 1 vez
	float bestF = 999999.0f;

	int cellIndex = -1;
	SearchCell * nextCell = nullptr;

    // Se encarga de buscar el indice con mejor distancia de la lista	
	for (int i = 0; i < m_openList.size(); i++) {
		if (m_openList[i]->GetF() < bestF){
			bestF = m_openList[i]->GetF();
			cellIndex = i;
		}
	}

	// Mete la informacion de la celda encontrada en el anterior for
	if (cellIndex >= 0) {
		nextCell = m_openList[cellIndex];
		m_visitedList.push_back(nextCell);
		m_openList.erase(m_openList.begin() + cellIndex);
	}
	return nextCell;
}

void PathFinding::PathOpened(float x, float y, float newCost, SearchCell *pPadre){
	
	if (x <= 0 || x >=/*TileManager::MAP_WIDTH*/1 || y <= 0 || y >=/*TileManager::MAP_HEIGHT*/1) {
		return;
	}

	Entidad * tile = /*TileManager::GetInstance()->GetTileAt((int)x, (int)y)*/nullptr;

	if (tile == NULL /*|| tile->hasTag(blocked)*/) return;  // bool hasTag (char * pTag) El tag se pone en Tiled y se extrae la informacion del fichero
	

	/*GetTileAt(int x, int y) {
		if (m_tiles == NULL || x < 0 ||x>= MAP_WIDTH || y < 0 ||y>= MAP_HEIGHT) {
			return NULL;
		}

		return m_tiles[y * MAP_WIDTH + x];
	}*/

	// este id es solo para comprobar, no cambia el id de ninguna celda
	float id = y * WORLD_SIZE/*TileManager::MAP_WIDTH*/ + x;

	// recorro la lista de visitadas hasta que coincida con el id de arriba.
	for (int i = 0; i < (int)m_visitedList.size(); i++) {
		if (id == m_visitedList[i]->GetID()) {
			return;
		}
	}

	// Celda para comprobar informacion, si cumple requisitos el Padre sera sustituido
	SearchCell * newChild = new SearchCell(x, y, pPadre);
	
	// id == m_visitedList[i]->GetID()
	newChild->SetID(id);
	newChild->setG(newCost);
	newChild->setH(pPadre->ManHattanDistance(m_goalCell));

	for (int i = 0; i < (int)m_openList.size(); i++) {
		if (newChild->GetID() == m_openList[i]->GetID()) {

			float newF = newChild->GetG() + newCost + m_openList[i]->GetH();

			if (m_openList[i]->GetF() > newF) {

				m_openList[i]->setG(newChild->GetG() + newCost);
				m_openList[i]->SetParent(newChild);
			}

			else {
				delete newChild;
				return;
			}
		}
	}

	m_openList.push_back(newChild);
}

void PathFinding::Clear() {
	
	m_pathState = NONE;

	if (m_startCell) {
		delete m_startCell;
		m_startCell = NULL;
	}

	if (m_goalCell) {
		delete m_goalCell;
		m_goalCell = NULL;
	}

	m_openList.clear();
	m_visitedList.clear();
	m_closesPaths.clear();

}

void PathFinding::ContinuePath() {
	
	if (m_openList.empty()) {
		return;
	}

	SearchCell * currentCell = GetNextCell();

	if (currentCell->GetID() == m_goalCell->GetID()) {
		m_goalCell->SetParent(currentCell->GetParent());

		SearchCell * getPath;

		// Va por todas las celdas comprobando cual es el camino mas corto y las mete en la lista
		// va hasta NULL porque la celda Start es la unica que lo tiene a NULL
		for (getPath = m_goalCell; getPath != NULL; getPath->GetParent()) {

			m_pathToGoal.push_back(new Vector2(getPath->GetCellX() * CELL_SIZE, getPath->GetCellY()));
		}

		m_foundGoal = true;
	}
	else {
		// Celda derecha
		PathOpened(currentCell->GetCellX() + 1, currentCell->GetCellY(), currentCell->GetG() + 1, currentCell);

		//Celda izquierda
		PathOpened(currentCell->GetCellX() - 1, currentCell->GetCellY(), currentCell->GetG() + 1, currentCell);
		
		// Celda superior
		PathOpened(currentCell->GetCellX(), currentCell->GetCellY() - 1, currentCell->GetG() + 1, currentCell);

		// Celda inferior
		PathOpened(currentCell->GetCellX(), currentCell->GetCellY() + 1, currentCell->GetG() + 1, currentCell);

		// Celda diagonal izquierda superior								/* 1.4f es lo que vale la diagonal de la Celda */
		PathOpened(currentCell->GetCellX() - 1, currentCell->GetCellY() - 1, currentCell->GetG() + 1.4f, currentCell);

		// Celda diagonal izquierda inferior
		PathOpened(currentCell->GetCellX() - 1, currentCell->GetCellY() + 1, currentCell->GetG() + 1.4f, currentCell);
		
		// Celda diagonal derecha superior
		PathOpened(currentCell->GetCellX() + 1, currentCell->GetCellY() - 1, currentCell->GetG() + 1.4f, currentCell);

		// Celda diagonal derecha inferior
		PathOpened(currentCell->GetCellX() + 1, currentCell->GetCellY() + 1, currentCell->GetG() + 1.4f, currentCell);
		
		for (int i = 0; i < m_openList.size(); i++)
		{	
			if (currentCell->GetID() == m_openList[i]->GetID()){
				
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

Vector2 PathFinding::GetNextClosesPoint() {
	// Siguiente paso en la lista de celdas del camino
	Vector2 nextPath = m_closesPaths[m_closesPaths.size()-1];
	// borras esa celda
	m_closesPaths.erase(m_closesPaths.begin() + m_closesPaths.size() - 1);

	return nextPath;
}

int PathFinding::GetClosesPathSize() {

	return (int)m_closesPaths.size();
}

vector<Vector2> PathFinding::GetClosesPath() {

	return m_closesPaths;
}

PathFinding::~PathFinding()
{
	Clear();
}
