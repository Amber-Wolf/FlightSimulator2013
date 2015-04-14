#pragma once
#include <vector>
#include "MissileController.h"
#include "MissileController.h"



class MissileManager
{
public:
	MissileManager(void);
	~MissileManager(void);
	vector<MissileController*>* getMissiles();
	void update(float elapsedTimeFloat);
	void draw();
	void addMissile(MissileController*);

private:
	vector<MissileController*> * missiles;
	VisualObject root;
};

