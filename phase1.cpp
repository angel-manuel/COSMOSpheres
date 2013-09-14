bool phase1_taking;
float phase1_initial_att[3];

void phase1_init() {
	phase1_taking = false;
}

void phase1_loop() {
	float *dst = &item_position[0];
	phase1_moveto(dst);

	/*
	const int target_item = 0;
	const int target_item_2 = 1;

	if(phase1_take(target_item)) {
		if(phase1_take(target_item_2)) {
			return;
		}
	}
	*/
}

//phase1_take
//target_item -> Número de item a cojer
//return -> true si el item ya ha sido recojido, false en todos los demas casos
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

void phase1_moveto(float *dst) {
	float d_pos[DIMENSION];
	float dist;
	float ahead[DIMENSION];
	float ahead_vel;
	float ahead_vel_vector[DIMENSION];
	float aside[DIMENSION];
	float aside_vel;
	float aside_vel_vector[DIMENSION];
	float x, t, half_way, half_t, dt;
	bool speed_up;
	float force[3] = {0};
	float ahead_force[DIMENSION];
	float aside_force[DIMENSION];

	mathVecSubtract(d_pos, dst, &our_state[POS], DIMENSION); 						//d_pos = dst - pos
	dist = mathVecMagnitude(d_pos, DIMENSION);										//dist = |d_pos|
	mathVecScalarDiv(ahead, d_pos, mathVecMagnitude(d_pos, DIMENSION), DIMENSION);	//ahead = d_pos/|d_pos|
	ahead_vel = mathVecInner(ahead, &our_state[VEL], DIMENSION);					//ahead_vel = ahead X vel
	mathVecScalarMult(ahead_vel_vector, ahead, ahead_vel, DIMENSION);				//ahead_vel_vector = ahead * ahead_vel

	mathVecSubtract(aside_vel_vector, &our_state[VEL], ahead_vel_vector, DIMENSION);//aside_vel_vector = vel - ahead_vel_vector
	mathVecScalarDiv(aside, ahead_vel_vector, mathVecMagnitude(aside_vel_vector, DIMENSION), DIMENSION);
	aside_vel = mathVecMagnitude(aside_vel_vector, DIMENSION);

	//x = (a*t^2)/2
	//sqrt(2*x/a) = t
	//v^2 = 2*a*x
	//x = v^2/(2*a)

	x = ahead_vel*ahead_vel/(2*MAX_FORCE);
	t = sqrtf(2*x/MAX_FORCE);
	half_way = (x + dist)/2;
	half_t = sqrtf(2*half_way/MAX_FORCE);
	dt = half_t*2 - t;

	DEBUG(("t = %f\n half_t = %f\n", t, half_t));
	speed_up = t < half_t;
	mathVecScalarMult(ahead_force, ahead, (speed_up) ? MAX_FORCE : -MAX_FORCE, DIMENSION);
	mathVecScalarMult(aside_force, aside, aside_vel*(-MAX_FORCE/dt), DIMENSION);
	mathVecAdd(force, ahead_force, aside_force, DIMENSION);

	api.setForces(force);
}
