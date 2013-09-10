bool phase1_taking;
float phase1_initial_att[3];

void phase1_init() {
	phase1_taking = false;
}

void phase1_loop() {
	const int target_item = 0;
	const int target_item_2 = 1;

	if(phase1_take(target_item)) {
		if(phase1_take(target_item_2)) {
			while(laser_shots_left > 0) {
				DEBUG(("Bang!"));
				game.shootLaser();
				laser_shots_left--;
			}
		}
	}

bool phase1_take(int target_item) {
	int i;
	float dist[3];
	float zero[3] = {0.0f};
	float d;
	float v;
	float r;
	float dot;
	float angle;
	float att_rate[3] = {0.0f};

	if(is_item_collected[target_item]) {
		phase1_taking = false;
		api.setVelocityTarget(zero);
		api.setAttRateTarget(zero);
		return true;
	}

	mathVecSubtract(dist, &(item_position[target_item*3]), &(our_state[POS]), 3);
	d = mathVecMagnitude(dist, 3);
	v = mathVecMagnitude(&(our_state[VEL]), 3);
	r = mathVecMagnitude(&(our_state[RATE]), 3);

	if((d < MAX_ITEM_START_DIST && v < MAX_ITEM_START_VEL && r < MAX_ITEM_START_RATE) || phase1_taking) {
		if(!phase1_taking) {
			phase1_taking = true;
			for(i = 0; i < 3; ++i) {
				phase1_initial_att[i] = our_state[ATT+i];
			}
		}

		api.setPositionTarget(&(item_position[target_item*3]));
		api.setVelocityTarget(zero);

		dot = mathVecInner(phase1_initial_att, &our_state[ATT], 3);
		angle = acosf(dot);

		if(!is_item_collected[target_item]) {
			if(angle > PI/2) {
				att_rate[POS_Z] = 0.0f;
				phase1_taking = false;
				DEBUG(("Fail!"));
			} else {
				att_rate[POS_Z] = MAX_ITEM_RATE/2;
			}
		} else {
			att_rate[POS_Z] = 0.0f;
		}

		api.setPositionTarget(&(item_position[target_item*3]));
		api.setVelocityTarget(zero);
		api.setAttRateTarget(att_rate);
	} else {
		phase1_taking = false;
		api.setPositionTarget(&(item_position[target_item*3]));
		api.setVelocityTarget(zero);
		api.setAttRateTarget(zero);
	}

	return false;
}
