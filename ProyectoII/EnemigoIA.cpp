#include "EnemigoIA.h"
#include "SearchCell.h"
#include "IdleState.h"
#include "EnemigoIAGlobalState.h"

#include <math.h>
#include <windows.h>
#include <utility>
#include <iostream>

EnemigoIA::EnemigoIA(MundoVirtual*pM, Entidad* target, int x, int y, int w, int h,
	JuegoSDL::Texturas_t textura, JuegoSDL::Efectos_t efecto, vector < pair<float, float>> waypoints)
	: Entidad(pM->getPJ(), x, y, w, h, textura, efecto, ONull), pMundo(pM), vida(3), m_target(target),
	m_currentIndex(0), m_waypoints(waypoints)
{
	std::cout << m_currentIndex << endl;
	Initialize();
}


EnemigoIA::~EnemigoIA()
{
	if (m_stateMachine) {
	
		delete m_stateMachine;
		m_stateMachine = NULL;
	}
}

void EnemigoIA::Initialize() {
	
	//m_target ser� el personaje
	m_maxVelocity = 0.0f;
	m_stateMachine = new StateMachine<EnemigoIA>(this);
	m_stateMachine->ChangeState(new IdleState());
	m_stateMachine->SetGlobalState(new EnemigoIAGlobalState());
}

void EnemigoIA::update() {

	Entidad::update();
	m_stateMachine->Update();
}

void EnemigoIA::ChaseTarget() {
	m_maxVelocity = 2.0f;
	SDL_Rect targetTransform = m_target->getRect();

	//Vectores auxiliares
	pair<float, float> targetPosition = make_pair(targetTransform.x, targetTransform.y);
	pair <float,float> position = make_pair(rect.x, rect.y);

	pair <float, float> toTarget = make_pair(targetPosition.first - position.first, targetPosition.second - position.second);
	float distance = sqrt((toTarget.first*toTarget.first) + (toTarget.second*toTarget.second));

	if (!IsWithinRangeOfTarget(0)) {
		
		toTarget.first /= distance;
		toTarget.second /= distance;
	}

	pair <float, float> velocity = make_pair(toTarget.first * m_maxVelocity, toTarget.second * m_maxVelocity);

	position.first = position.first + velocity.first;
	position.second = position.second + velocity.second;

	SDL_Rect posRect = { position.first, position.second, rect.w, rect.h };
	setRect(posRect);

}

StateMachine<EnemigoIA>*EnemigoIA::GetStateMachine() {
	return m_stateMachine;
}

const pair <float, float> &EnemigoIA::findNextWayPoints(){

	std::cout << m_currentIndex << endl;

	if (m_waypoints.size() != 0)
	{
		const pair <float, float>& waypoint = m_waypoints[m_currentIndex];
		m_currentIndex = (int)(rand() % m_waypoints.size());

		return waypoint;
	}
}

bool EnemigoIA::IsWithinRangeOfTarget(float minDistance) {

	SDL_Rect targetTransform = m_target->getRect();

	//Vectores auxiliares
	pair<float, float> targetPosition = make_pair(targetTransform.x, targetTransform.y);
	pair <float, float> position = make_pair(rect.x, rect.y);

	pair <float, float> toTarget = make_pair(targetPosition.first - position.first, targetPosition.second - position.second);

	float distance = sqrt((toTarget.first*toTarget.first) + (toTarget.second*toTarget.second));

	return (distance <= minDistance);
}