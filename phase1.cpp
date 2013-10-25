//Se encarga de la fase primera
bool phase1_taking;
bool phase1_collision;
float phase1_initial_att[3];

void phase1_init() {
	phase1_taking = false;
	phase1_collision = false;
}

void phase1_loop() {
	const int target_item = (blue_sphere) ? 1 : 0;
	const int target_item2 = (blue_sphere) ? 0 : 1;

	if(!phase1_collision) {
		phase1_collision = game.wasCollisionActive();
	}

	if(phase1_collision || phase1_take(target_item)) {
		if(phase1_take(target_item2)) {
			phase2_prepare();
		}
	}
}

//phase1_take
//target_item -> Número de item a cojer
//return -> true si el item ya ha sido recojido, false en todos los demas casos
bool phase1_take(int target_item) {
	float zero[3] = {0.0f};
	float att_rate[3] = {0.0f};

	if(is_item_collected[target_item]) {
		phase1_taking = false;
		api.setVelocityTarget(zero);
		api.setAttRateTarget(zero);
		return true;
	}

	if(movement_moveto(item_position[target_item]) || phase1_taking) {
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
		angle = acosf(dot);

		if(!is_item_collected[target_item]) {
			if(angle > PI/2) {
				att_rate[POS_Z] = 0.0f;
				phase1_taking = false;
				#ifdef DEBUG_ACTIVE
				DEBUG(("phase1_take:Fail!"));
				#endif
			} else {
				if(angle < 3*PI/8) {
					att_rate[POS_Z] = MAX_ITEM_RATE/2;
				} else {
					att_rate[POS_Z] = 0.1f;
				}
			}
		} else {
			att_rate[POS_Z] = 0.0f;
		}

		api.setPositionTarget(item_position[target_item]);
		api.setVelocityTarget(zero);
		api.setAttRateTarget(att_rate);
	} else {
		phase1_taking = false;
	}

	return false;
}
