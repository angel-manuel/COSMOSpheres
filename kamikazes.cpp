//Avoid kamikazes
void predict_kamikaze(){
	//Loop for checking their state
	for(int j = 0; j < 3; j++){
		watch_other_sphere(j);
	}
}

void watch_other_sphere(int timing){	
	
	//Variables
	float myState[3];
	float otherState[3];
	float myPos[3];
	float otherPos[3];
	float between[3];
	float distance0;
	float distance1;
	float distance2;
	float distance3;
	
	//Get our position and theirs
	api.getMyZRState(myState);
	api.getOtherZRState(otherState);
	for(int i = 0; i<3; i++){
		myPos[i] = myState[i];
		otherPos[i] = otherState[i];
	}
	//Calculate vector between them and us
	mathVecSubtract(between, otherPos, myPos, 3);
	float actualDistance = mathVecMagnitude(between, 3);
	//Set distance variables
	switch(timing){
		case 0:
		distance0 = actualDistance;
		case 1:
		distance1 = actualDistance;
		case 2:
		distance2 = actualDistance;
		case 3:
		distance3 = actualDistance;
			break;
	}
	//If the distances are shortening, we run away (in development, need to establish the alternPos)
	if(distance3 < distance2 < distance1 < distance0){
		float alternPos[3] = {0.0f,0.0f,0.0f};
		api.setPositionTarget(alternPos);	
	}
}

