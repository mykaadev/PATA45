#pragma once
#include <iostream>

class Vector2
{
public:
	float X, Y;
	
	Vector2(float x = 0 , float y = 0 ) : X(x), Y(y) { };

public:

	inline Vector2 operator+(const Vector2& v2) const { return Vector2(X + v2.X, Y + v2.Y); }
	inline Vector2 operator-(const Vector2& v2) const { return Vector2(X - v2.X, Y - v2.Y); }
	inline Vector2 operator*(const float scalar) const { return Vector2(X * scalar, Y * scalar); }

	//print the vector
	void Log(std::string msg = "")
	{
		std::cout << msg << "(X,Y) = " << X << " " << Y << ")" << std::endl;
	}
};