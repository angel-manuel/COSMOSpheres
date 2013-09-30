bool phase1_taking;
float phase1_initial_att[3];

void phase1_init() {
	phase1_taking = false;
}

void phase1_loop() {
	const int target_item = (blue_sphere) ? 0 : 1;

	if(phase1_take(target_item)) {
		float dst[3] = {our_comet_state[POS_X] + (blue_sphere) ? -0.18f : 0.18f, 0.7f, our_comet_state[POS_Z]};
		api.setPositionTarget(dst);
	}
	
}

//phase1_take
//target_item -> Número de item a cojer
//return -> true si el item ya ha sido recojido, false en todos los demas casos
bool phase1_take(int target_item) {
	float zero[3] = {0.0f};
	float dot;
	float angle;
	float att_rate[3] = {0.0f};

	if(is_item_collected[target_item]) {
		phase1_taking = false;
		api.setVelocityTarget(zero);
		api.setAttRateTarget(zero);
		return true;
	}

	if(phase1_moveto(&(item_position[target_item*3])) || phase1_taking) {
		if(!phase1_taking) {
			phase1_taking = true;
			mathVecCopy(phase1_initial_att, &our_state[ATT], 3);
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
	}

	return false;
}

//phase1_moveto
//dst -> Destino
//return -> true si ya se ha llegado, false en todos los demas casos
bool phase1_moveto(float dst[3]) {
	float delta[3];
	float d;
	float v;
	float r;
	float zero[3] = {0.0f};

	mathVecSubtract(delta, dst, &our_state[POS], 3);
	d = mathVecMagnitude(delta, 3);
	v = mathVecMagnitude(&our_state[VEL], 3);
	r = mathVecMagnitude(&our_state[RATE], 3);

	if(d < MAX_ITEM_START_DIST) {
		if(v < MAX_ITEM_START_VEL && r < MAX_ITEM_START_RATE) {
			return true;
		} else {
			api.setPositionTarget(dst);
			api.setVelocityTarget(zero);
			api.setAttRateTarget(zero);
			return false;
		}
	} else {
		//TODO: Sustituir setPositionTarget
		//iss5: Maniobrar entre las nubes
		api.setPositionTarget(dst);
		return false;
	}
}
