#pragma once
#include "Vector2.h"

class Transform
{
public:
	float X, Y;

public:
	Transform(float x = 0, float y = 0) : X(x), Y(y) {};
	//print the vector
	void Log(std::string msg = "")
	{
		std::cout << msg << "(X,Y) = " << X << " " << Y << ")" << std::endl;
	}
	inline void TranslateX(float x)  {X += x;}
	inline void TranslateY(float y)  {Y += y;}
	inline void Translate(Vector2 v) {X += v.X; Y += v.Y;}
	inline void Interp(Vector2 v, Vector2 vTarget, float interpSpeed) {	v.X += (vTarget.X - v.X) * interpSpeed;  v.Y += (vTarget.Y - v.Y) * interpSpeed; }
	inline void SetPosition(Vector2 v, Vector2 vTarget) { v.X = vTarget.X; v.Y = vTarget.Y; }

protected:
};