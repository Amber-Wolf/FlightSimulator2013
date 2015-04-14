#pragma once
#include "controller.h"
#include "VisualObject.h"
class MissileController :
	public Controller
{
public:
	MissileController(vec3 velocity, vec3 position, int allyNum);
	MissileController();
	MissileController(float speed, float turnX, float turnY, vec3 pos, int allyNum);
	~MissileController(void);

	vec3 getVelocity();
	vec3 getPosition();
	void destroy();
	void update(float elapsedTimeSec);
	bool getCollide(Controller* other);
	bool getCollide(VisualObject* other);
	bool getLive();
	int getAlly();

	static const int NO_ALLIEGENCE = 0;
	static const int PLANE = 1;
	static const int SHIP = 2;

private:
	float turnX;  //currently unused
	float turnY;
	float turnZ;
	vec3 position;
	vec3 velocity;
	bool live;
	int allyNum;
	float liveTime;
};

