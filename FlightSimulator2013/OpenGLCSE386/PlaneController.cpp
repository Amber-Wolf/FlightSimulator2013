#include "PlaneController.h"
const float dragConst = .05f;
const float liftConst = .1f;
const float PI = 3.14;
const float gravConst = 1.0f;

PlaneController::PlaneController(void)
{
		lift = vec3(0.0f, 0.0f, 0.0f);
		velocity = vec3(0.0f, 0.0f, 0.0f);
		thrust = vec3(0.5f, 0.0f, 0.0f);
		rotateTurn = 0.0f;
		rotateUp = 0.0f;
}

PlaneController::~PlaneController(void){
}

vec3 PlaneController::getPosition(){
	return position;
}

vec3 PlaneController::getVelocity(){  //TODO Change to be accurate representation
	return velocity;
}

float PlaneController::getSpeed(){
	return thrust.x;
}

vec3 PlaneController::getThrust(){
	return trueThrust;
}

void PlaneController::update(float elapsedTimeSeconds){

	//rotate velocity vector
	trueThrust = (vec4(thrust,1.0f) * rotate(mat4(1.0f), fmod(rotateUp, 360.0f), vec3(0.0f, 0.0f, 1.0f))  *rotate(mat4(1.0f), fmod(rotateTurn, 360.0f), vec3(0.0f, 1.0f, 0.0f))).xyz;

	lift.y = sqrtf( pow(velocity.x,2) + pow(velocity.z,2)) * liftConst;
	vec3 drag = -velocity * dragConst;
	vec3 grav = vec3(0.0f, -gravConst, 0.0f);

	velocity += lift + drag + grav + trueThrust;

	//rotate the object
	mat4 turnY = rotate(mat4(1.0f), fmod(-rotateTurn, 360.0f), vec3(0.0f, 1.0f, 0.0f));
	mat4 turnZ = rotate(mat4(1.0f), fmod(-rotateUp, 360.0f), vec3(0.0f, 0.0f, 1.0f));

	position = position + velocity * elapsedTimeSeconds;

	//check ground
	if(position.y < 0){
		position.y = 0;
	}

	//

	target->modelMatrix =  translate(mat4(1.0f), position) * turnY * turnZ;
}

void PlaneController::inThrust(float degree){
	thrust.x += degree /20.0f;
}

void PlaneController::deThrust(float degree){
	thrust.x -= degree/20.0f;
}

void PlaneController::turnRight(){
	rotateTurn = rotateTurn + 3.0f;
	cout << "turning right : " << rotateTurn << endl;
}

void PlaneController::turnLeft(){
	rotateTurn = rotateTurn - 3.0f;
	cout << "turning left: " << rotateTurn << endl;
}

void PlaneController::turnUp(){
	rotateUp += 3.0f;
}

void PlaneController::turnDown(){
	rotateUp -= 3.0f;
}

void PlaneController::addChild(VisualObject* child){
	child->modelMatrix = inverse(target->modelMatrix) * child->modelMatrix;
	child->detachFromParent();
	target->addChild(child);
}

float PlaneController::getRadius(){
	return 1.0f;
}
