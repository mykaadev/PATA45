#pragma once
#include "BaseAsteroid.h"
class StoneAsteroid : public BaseAsteroid
{
public: 
	StoneAsteroid(Properties* pros);


};

/**
 * class Asteroid {
  public:
	// Enum to represent the size of the asteroid
	enum Size { BIG, MEDIUM, SMALL };
	// Constructor
	Asteroid(Size size) : size_(size) {
	  switch (size_) {
		case BIG:
		  sprite_ = "SAster96.bmp";
		  break;
		case MEDIUM:
		  sprite_ = "SAster64.bmp";
		  break;
		case SMALL:
		  sprite_ = "SAster32.bmp";
		  break;
	  }
	}
	// Function to handle damage
	void TakeDamage(int damage) {
	  if (size_ != SMALL) {
		health_ -= damage;
		if (health_ <= 0) {
		  // Split into smaller asteroids
		  Split();
		}
	  } else {
		// Small asteroid explodes
		Explode();
	  }
	}
  private:
	Size size_;
	int health_;
	string sprite_;
	void Split() {
	  // Create three smaller asteroids
	  // You can use the same constructor to set the size and sprite
	  Asteroid a1(MEDIUM);
	  Asteroid a2(MEDIUM);
	  Asteroid a3(MEDIUM);
	  // Add code to handle the split asteroids in your game
	}
	void Explode() {
	  // Add code to handle the explosion in your game
	}
};
 */*/