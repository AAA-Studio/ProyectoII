#ifndef _H_MathCore_H_
#define _H_MathCore_H_

#include "checkML.h"
using namespace std;

class Vector2
{
public:
	Vector2(){ m_x = m_y = 0.0f; }
	Vector2(float _x, float _y) : m_x(_x), m_y(_y){}
	~Vector2();

	// sobrecarga necesaria para determinar distancias entre puntos
	Vector2 operator - (const Vector2 &v2) {
		return Vector2(m_x - v2.m_x, m_y - v2.m_y);
	}
	Vector2 operator * (const float incR) {
		return Vector2(m_x * incR, m_y * incR);
	}
	Vector2 operator /= (const float incR) {
		return Vector2(m_x = m_x/ incR, m_y = m_y / incR);
	}

	
	

	inline float GetX() const { return m_x; }
	inline float GetY() const { return m_y; }
	// tama�o del vector
	inline float GetLength() const { return sqrt((m_x*m_x) + (m_y*m_y)); }
	void SetX(float x) { m_x = x; }
	void SetY(float y) { m_y = y; }

private:
	float m_x;
	float m_y;
	
};

class Vector3
{
public:
	Vector3(){ m_x = m_y = m_z = 0.0f; }
	Vector3(float _x, float _y, float _z) : m_x(_x), m_y(_y), m_z(_z){}
	~Vector3();

	// sobrecarga necesaria para determinar distancias entre puntos
	Vector3 operator- (const Vector3 &v2) {
		return Vector3(m_x - v2.m_x, m_y - v2.m_y, m_z - v2.m_z);
	}

	
	// tama�o del vector
	inline float GetLength() const { return sqrt((m_x*m_x) + (m_y*m_y) + (m_z*m_z)) };

	inline float GetX() const { return m_x; }
	inline float GetY() const { return m_y; }
	inline float GetZ() const { return m_z; }
	void SetX(float x) { m_x = x; }
	void SetY(float y) { m_y = y; }
	void SetZ(float z) { m_z = z; }

private: 
	float m_x;
	float m_y;
	float m_z;
};

class MathCore
{
public:
	MathCore();
	~MathCore();
};
#endif
