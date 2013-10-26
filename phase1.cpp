//Se encarga de la fase primera
bool phase1_taking;
int phase1_last_item;
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
	if(!phase1_collision) {
		phase1_collision = game.wasCollisionActive();
		if(phase1_collision) {
			phase1_prefered_item = (phase1_prefered_item == 1) ? 0 : 1;
		}
	}

	if(seconds < 80 || phase1_taking) {
		if(phase1_take(phase1_prefered_item, false) && (phase1_collision || phase1_take((phase1_prefered_item == 1) ? 0 : 1, true))) {
			phase2_prepare();
		}
	} else {
		phase2_prepare();
	}
}

//phase1_take
//target_item -> Número de item a cojer
//return -> true si el item ya ha sido recojido, false en todos los demas casos
bool phase1_take(int target_item, bool direct) {
	if(is_item_collected[target_item]) {
		if(target_item == phase1_last_item)
			phase1_taking = false;
		return true;
	}

	float zero[3] = {0.0f};

	phase1_last_item = target_item;
	#ifdef DEBUG_ACTIVE
	DEBUG(("phase1:Going to item %i\n", target_item));
	#endif

	if(movement_moveto(item_position[target_item], direct) || phase1_taking) {
		float dot;
		float angle;
		if(!phase1_taking) {
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
		angle = PI/2.0f - dot - (dot*dot*dot)/6.0f; //Series de taylor

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
