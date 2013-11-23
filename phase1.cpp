//phase1
//It contains the logic for the phase 1
bool phase1_taking;			//true if we are spinning on the item
int phase1_last_item;		//The last item we were trying to pick up
int phase1_prefered_item;
bool phase1_collision;
float phase1_initial_att[3];

void phase1_init() {
	phase1_taking = false;
	phase1_last_item = -1;
	phase1_collision = false;
	phase1_prefered_item = (blue_sphere) ? 1 : 0;
}

void phase1_loop() {
	if(!phase1_collision) {	//If we haven't collided yet
		phase1_collision = game.wasCollisionActive(); //We check if we have collided
		#ifdef PHASE2_FORCE_STRATEGY_STAY_AND_SHOOT
		phase1_collision = true;
		#endif
		if(phase1_collision) {
			//If we have collided we change our target item
			phase1_prefered_item = (phase1_prefered_item == 1) ? 0 : 1;
		}
	}

	if(seconds < 80 || phase1_taking) {
		//If there is plenty of time or if we are currently spinning we take the items
		if(phase1_take(phase1_prefered_item, false) && (phase1_collision || phase1_take((phase1_prefered_item == 1) ? 0 : 1, true))) {
			phase2_prepare();
		}
	} else {
		phase2_prepare();
	}
}

//phase1_take
//Tries to pick up a item
//target_item -> Item number
//return -> true if the item has already been picked up, false otherwise
bool phase1_take(int target_item, bool direct) {
	if(is_item_collected[target_item]) {
		//If the item has been picked up we return true
		phase1_taking = false;
		return true;
	}

	float zero[3] = {0.0f};

	phase1_last_item = target_item;
	#ifdef DEBUG_ACTIVE
	DEBUG(("phase1:Going to item %i\n", target_item));
	#endif

	if(movement_moveto(item_position[target_item], direct) || phase1_taking) {
		//If we have arrived or if we are currenly there spinning
		float dot;
		float angle;
		if(!phase1_taking) {
			//If we weren't spinning we record our current attitude to measure our rotation
			phase1_taking = true;
			int i;
			for(i = 0; i < 3; ++i) {
				phase1_initial_att[i] = our_state[ATT+i];
			}
		}

		api.setPositionTarget(item_position[target_item]);
		api.setVelocityTarget(zero);

		dot = mathVecInner(phase1_initial_att, &our_state[ATT], 3);
		//angle = acosf(dot);
		angle = PI/2.0f - dot - (dot*dot*dot)/6.0f; //Taylor expansion of acos

		api.setVelocityTarget(zero);

		//zero = att_rate
		if(!is_item_collected[target_item]) {
			if(angle > PI/2) {
				//zero[POS_Z] = 0.0f;
				phase1_taking = false;
				#ifdef DEBUG_ACTIVE
				DEBUG(("phase1_take:Fail!"));
				#endif
			} else {
				if(angle < 3*PI/8) {
					zero[POS_Z] = MAX_ITEM_RATE/2;
				} else {
					zero[POS_Z] = 0.1f;
				}
			}
		}/* else {
			zero[POS_Z] = 0.0f;
		}*/

		api.setPositionTarget(item_position[target_item]);
		api.setAttRateTarget(zero);
	} else {
		phase1_taking = false;
	}

	return false;
}
