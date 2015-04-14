#include "ShipManager.h"
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
#include "ObjObject.h"

#include "SpaceShip.h"


#include "GeneralLightingTechnique.h"

#include "Box.h"
#include "ObjObject.h"

ShipManager::ShipManager(OpenGLApplicationBase* app, int numOfShips, GLint shader)
	:numOfShips(numOfShips), wc(wc), beenHit(beenHit)
{
	shipVector = new vector<VisualObject*>();
	for(int i = 0; i < numOfShips; i++){
		shipVector->push_back(new ObjObject( app, "ship.obj","ship.bmp")); 
		shipVector->at(i)->initialize();
		shipVector->at(i)->setShader(shader);
		shipVector->at(i)->fixedTransformation = rotate(mat4(1.0f), 180.0f, vec3(1.0f, 0.0f, 0.0f));
	}

	 for(int i = 0; i < numOfShips; i++){
            /* generate secret number between 1 and 10: */
            float firstX = (rand() % 60 + 1) - 30;
            float firstZ = (rand() % 80 + 1) -80;
            float secondX = (rand() % 60 + 1) - 30;
            float secondZ = (rand() % 80 + 1) - 80;
            //beenHit[i] = false;
            vector<vec3> waypoints;
            //-80.0f + i * 10.0f
            waypoints.push_back(vec3( firstX, -3.0f, firstZ));
            waypoints.push_back(vec3( secondX, -3.0f, secondZ));

            wc.push_back(new WaypointController2(waypoints, 5));
            shipVector->at(i)->addController( wc[i] );
            waypoints.clear();
      }
}

vector<VisualObject*>* ShipManager::getShips(){
	return shipVector;
}

void ShipManager::update(float elapsedTimeSec){
	for(int i = 0; i < shipVector->size(); i++){
		shipVector->at(i)->update(elapsedTimeSec);
	}
}

void ShipManager::draw(){
	for(int i = 0; i < shipVector->size(); i++){
		shipVector->at(i)->draw();
	}
}