#pragma once


class EnemySpawner
{
public:
	EnemySpawner(
		//Vector2i gridPos,
		bool randomSpawnPos,
		int maxVelocity,
		int type,
		int levelInterval,
		int nrOfEnemies
	);

	virtual ~EnemySpawner();



private:
	bool used;
	//Vector2i gridPosition;
	bool randomSpawnPos;
	int maxVelocity; // -1 = random
	int type; // -1 = random
	int levelInterval;
	int nrOfEnemies; // -1 = random ( 1 - 10 )



};

