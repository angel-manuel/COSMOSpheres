//Let's make those guys bite the dust once again

//Check if a kamikaze is coming, copied from Salok's kamikaze trolling program
bool predict_Kamikaze()
{
	//Relative position of both spheres
	float relative_position [3];
	mathVecSubtract(relative_position, &our_state[0], &their_state[0], 3);
	mathVecNormalize(relative_position, 3);

	//Velocity of enemy sphere
	float enemy_velocity[3];
	enemy_velocity[0] = their_state[3];
	enemy_velocity[1] = their_state[4];
	enemy_velocity[2] = their_state[5];
	mathVecNormalize(enemy_velocity, 3);

	//We compare both
	float result[3];
	mathVecSubtract(result, enemy_velocity, relative_position,3);

	//If less than the correction factor, we are being attacked.
	return (mathVecMagnitude(result, 3) < delta);
}

//Which is the nearest item?
bool isItItem1(){
	float vecTo1[3];
	float vecTo2[3];
	float distanceTo1;
	float distanceTo2;
	float firstItemPos[3]= {0.5f, 0.65f,0.0f};
	float seconItemPos[3]= {-0.5f,0.65f,0.0f};
	mathVecSubtract(vecTo1, firstItemPos, &our_state[POS], 3);
	mathVecSubtract(vecTo2, seconItemPos, &our_state[POS], 3);
	distanceTo1 = mathVecMagnitude(vecTo1, 3);
	distanceTo2 = mathVecMagnitude(vecTo2, 3);
	return(distanceTo1<distanceTo2);
}

//What it says, dodges and pushes
void dodgeAndPush()
{
	bool dodgeDone;
	bool isYequal;
	//Define the alternPos values depending on the nearest Item (when the other sphere goes kamikaze while picking the items)
	if(predictKamikaze){
		#ifdef DEBUG_ACTIVE
		DEBUG(("KAMIKAZE DETECTED\n"));
		#endif
		float alternPos[3];
		if(isItItem1){
			alternPos[POS_X] = firstItemPos[POS_X];
			alternPos[POS_Y] = firstItemPos[POS_Y];
			alternPos[POS_Z] = firstItemPos[POS_Z];
		}else{
			alternPos[POS_X] = seconItemPos[POS_X];
			alternPos[POS_Y] = seconItemPos[POS_Y];
			alternPos[POS_Z] = seconItemPos[POS_Z];
		}
		api.setPositionTarget(alternPos);
		dodgeDone = true;
		#ifdef DEBUG_ACTIVE
		DEBUG(("AT LEAST YOU TRIED\n"));
		#endif
	}
	
	//Is the Y coordinate equal? (+- 0.1)
	if((our_state[POS_Y] = their_state[POS_Y]) || (our_state[POS_Y] = their_state[POS_Y] + 0.1) || (our_state[POS_Y] = their_state[POS_Y] - 0.1)){
		isYequal = true;
	}
	
	//Once we've dodged them, we push them away
	if(dodgeDone && isYequal){
		api.setPositionTarget(&their_state[POS]);
		#ifdef DEBUG_ACTIVE
		DEBUG(("OUT OF MY WAY\n"));
		#endif
	}
}
