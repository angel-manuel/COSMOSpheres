//Avoid kamikazes
void predict_kamikaze(){
	//Loop for checking their state
	for(int j = 0; j < 3; j++){
		watch_other_sphere(j);
	}
}

void watch_other_sphere(int timing){	
	
	//Variables
	float myState[12];
	float otherState[12];
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
	
	//Are we closer to Item 1 or 2?
	bool isItItem1;
	float referencePos[3]= {0.0f, 0.65f, 0.0f};
	float vecTo1[3];
	float vecTo2[3];
	float distanceTo1;
	float distanceTo2;
	float firstItemPos[3]= {0.5f, 0.65f,0.0f};
		float seconItemPos[3]= {-0.5f,0.65f,0.0f};
	mathVecSubtract(vecTo1, firstItemPos, myPos, 3);
	mathVecSubtract(vecTo2, seconItemPos, myPos, 3);
	distanceTo1 = mathVecMagnitude(vecTo1, 3);
	distanceTo2 = mathVecMagnitude(vecTo2, 3);
	if(distanceTo1<distanceTo2){
		isItItem1 = true;
	}else if(distanceTo2>distanceTo1){
		isItItem1 = false;
	}else{
		api.setPositionTarget(referencePos);//Reference point, in case of bugs
	}
	
	//If the distances are shortening, we run away to one of the items depending on our pos
	bool dodgeDone;
	bool isYequal;
	if((distance3 < distance2 && distance2 < distance1 && distance1 < distance0){
		#ifdef DEBUG_ACTIVE
		DEBUG(("KAMIKAZE DETECTED\n"));
		#endif
		float alternPos[3];
		if(isItItem1){
			alternPos[0] = firstItemPos[0];
			alternPos[1] = firstItemPos[1];
			alternPos[2] = firstItemPos[2];
		}else{
			alternPos[0] = seconItemPos[0];
			alternPos[1] = seconItemPos[1];
			alternPos[2] = seconItemPos[2];
		}
		api.setPositionTarget(alternPos);
		dodgeDone = true;
	}
	
	//Is the Y coordinate equal? (+- 0.1)
	if((myPos[1] = otherPos[1]) || (myPos[1] = otherPos[1] + 0.1) || (myPos[1] = otherPos[1] - 0.1)){
		isYequal = true;
	}
	
	//Once we've dodged them, we push them away
	if(dodgeDone && isYequal){
		#ifdef DEBUG_ACTIVE
		DEBUG(("DODGE DONE\n"));
		#endif
		api.setPositionTarget(otherPos);
		DEBUG(("OUT OF MY WAY\n"));
	}
}
