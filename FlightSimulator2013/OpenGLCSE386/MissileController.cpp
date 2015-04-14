#include "MissileController.h"
const float RADIUS = 5.0f;
const float GROUND_LEVEL = -5.0f;
const float PI = 3.14f;

MissileController::MissileController(void){
	position = vec3(0,0,0);
	turnX = 0.0f;
	turnY = 0.0f;
	turnZ = 0.0f;
	velocity = vec3(0.0f,-1.0f,0.0f);
	live = true;
	allyNum = NO_ALLIEGENCE;
	liveTime = 30.0f;
}

MissileController::MissileController(vec3 velocity, vec3 position, int allyNum){
	this->velocity = velocity;
	this->position = position;
	this->allyNum = allyNum;
	vec3 normVel =  glm::normalize(velocity);
	turnY = (asin(normVel.z) * 180.0f)/PI;  //JANK MATH CONVERSION CRAP THAT DOESNT WORK YET
	turnZ = (asin(normVel.y) * 180.0f)/PI;
	cout << "turn Y:"  << turnY << std::endl;
	cout << "turn Z:"  << turnZ << std::endl;
	if(normVel.x == -1){
		turnY = -180;
	}
	live = true;
	liveTime = 30.0f;
}

MissileController::MissileController(float speed, float turnY, float turnZ, vec3 pos, int allyNum){
	vec3 speedV = vec3(speed,0.0f,0.0f);
	velocity = (vec4(speedV,1.0f) * rotate(mat4(1.0f), fmod(turnZ, 360.0f), vec3(0.0f, 0.0f, 1.0f))  *rotate(mat4(1.0f), fmod(turnY, 360.0f), vec3(0.0f, 1.0f, 0.0f))).xyz;
	position = pos;
	live = true;
	this->allyNum = allyNum; 
	this->turnX = 0.0f;
	this->turnY = turnY;
	this->turnZ = turnZ;
	liveTime = 30.0f;
}

MissileController::~MissileController(void){

}

void MissileController::update(float elapsedTimeSeconds){
	mat4 matTurnY = rotate(mat4(1.0f), fmod(-turnY, 360.0f), vec3(0.0f, 1.0f, 0.0f));
	mat4 matTurnZ = rotate(mat4(1.0f), fmod(-turnZ, 360.0f), vec3(0.0f, 0.0f, 1.0f));
	position += velocity * elapsedTimeSeconds;
	if(position.y < GROUND_LEVEL){
		destroy();
	}
	target->modelMatrix =  translate(mat4(1.0f), position) * matTurnY * matTurnZ;
	liveTime -= elapsedTimeSeconds;
	if(liveTime < 0.0f){
		live = false;
	}
}

void MissileController::destroy(){
	live = false;
}

vec3 MissileController::getVelocity(){
	return velocity;
}

vec3 MissileController::getPosition(){
	return position;
}
	
bool MissileController::getCollide(Controller* other){
	return (glm::distance(other->getTarget()->getWorldPosition(),position) < (RADIUS));
}

bool MissileController::getCollide(VisualObject* other){
	return (glm::distance(other->getWorldPosition(),position) < (RADIUS));
}

bool MissileController::getLive(){
	if(!live){
			cout << "returning false" << std::endl;
	}
	return live;
}

