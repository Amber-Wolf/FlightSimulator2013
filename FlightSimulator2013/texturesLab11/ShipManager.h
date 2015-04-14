#pragma once

#include "OpenGLApplicationBase.h"
#include "ColoredPyramid.h"
#include "Pyramid.h"
#include "Controller.h"
#include "Spinner.h"
#include "WaypointController.h"
#include "OrbitController.h"
#include "WaypointController2.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Torus.h"
#include "Cone.h"
#include "Floor2.h"
#include "Wall.h"
#include <vector>

#include "SpaceShip.h"


#include "GeneralLightingTechnique.h"

#include "Box.h"
#include "ObjObject.h"

class ShipManager
{
public:

	// Constructor sets the position of the object,
	// the axis about which it will rotate, and the rate and direction
	// of rotation.
	ShipManager(OpenGLApplicationBase * app, int numOfShips, GLint shader);

	vector<VisualObject*>* getShips();

	void update(float timeElapsedSeconds);

	void draw();



protected:
	
	// Position at which the object is spinning in place
	int numOfShips;

	vector<VisualObject*>* shipVector;

	vector<WaypointController2*> wc;

	bool * beenHit;

};