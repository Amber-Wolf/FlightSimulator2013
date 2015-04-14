#include "MissileManager.h"
#include "MissileController.h"


MissileManager::MissileManager(void){
	root.initialize();
	missiles = new vector<MissileController*>();
}


MissileManager::~MissileManager(void){

}

vector<MissileController*> * MissileManager::getMissiles(){
	return missiles;
}

void MissileManager::update(float elapsedTimeSec){
	for(int x =0; x< missiles->size(); x++){
		missiles->at(x)->getTarget()->update(elapsedTimeSec);
		if(!missiles->at(x)->getLive()){
			missiles->erase(missiles->begin() + x);
			x--;
			cout << "Missile deleted!" << std::endl;
		}
	}
}

void MissileManager::draw(){
	for(int x =0; x< missiles->size(); x++){
		missiles->at(x)->getTarget()->draw();
	}
}

void MissileManager::addMissile(MissileController* obj){
	missiles->push_back(obj);
}