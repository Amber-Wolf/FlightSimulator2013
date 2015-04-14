#pragma once
#include "controller.h"
class PlaneController :
	public Controller
{
public:
	PlaneController(void);
	~PlaneController(void);

	virtual void update(float elapsedTimeSeconds);
	
	vec3 getPosition();

	vec3 getVelocity();

	void inThrust(float degree);

	void deThrust(float degree);

	void addChild(VisualObject* child);

	float getRadius();

	void turnRight();

	void turnLeft();

	void turnUp();

	void turnDown();

	vec3 getThrust();

	float getSpeed();

private:

	vec3 position;

	vec3 thrust;

	vec3 lift;

	vec3 velocity;

	vec3 actualVelocity;

	vec3 trueThrust;



	float rotateTurn;
	float rotateUp;
};

