#ifndef ___WAYPOINT_CONTROLLER_2_H___
#define ___WAYPOINT_CONTROLLER_2_H___

#include "controller.h"

class WaypointController2 :	public Controller
{
public:
	WaypointController2( vector<vec3> points, GLfloat spd = 1.0f );
	
	// Update method that is call by the Visual Object.
	virtual void update(float elapsedTimeSeconds);
	
	virtual vec3 getPosition();

protected:

	int getNexWaypointIndex();

	void updateVeolcityForNewWaypoint();

	GLfloat distanceToTargetWaypoint();

	vector<vec3> waypoints;

	vec3 position;

	vec3 velocity;

	GLfloat speed;

	int targetWaypointIndex;
};

#endif /* #ifndef ___WAYPOINT_CONTROLLER_2_H___ */


