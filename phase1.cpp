bool phase1_taking;
float phase1_initial_att[3];

void phase1_init() {
	phase1_taking = false;
}

float phase1_moveto_t_stop;
float phase1_moveto_dist_stop;
float phase1_moveto_dist;
void phase1_loop() {
	/*
	phase1_moveto_t_stop = 0.0f;
	phase1_moveto_dist_stop = 0.0f;
	phase1_moveto_dist = 0.0f;
	debug_track(4, &phase1_moveto_t_stop, (char*)"t_stop");
	debug_track(5, &phase1_moveto_dist_stop, (char*)"dist_stop");
	debug_track(6, &phase1_moveto_dist, (char*)"dist");

	float *dst = &item_position[0];
	phase1_moveto(dst);
	*/

	
	const int target_item = 0;
	const int target_item_2 = 1;

	if(phase1_take(target_item)) {
		return;
	}
	
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
	float *pos = &our_state[POS];
	float *vel = &our_state[VEL];
	float *att = &our_state[ATT];
	float *rate = &our_state[RATE];
	float forces[3] = {0.0f};

	float dist[DIMENSION];
	mathVecSubtract(dist, dst, pos, DIMENSION);

	float d;
	d = mathVecMagnitude(dist, DIMENSION);

	float ahead[DIMENSION];
	mathVecGetNormal(ahead, dist, DIMENSION);

	float ahead_vel;
	ahead_vel = mathVecInner(ahead, vel, DIMENSION);

	float ahead_vel_vector[DIMENSION];
	mathVecScalarMult(ahead_vel_vector, ahead, ahead_vel, DIMENSION);

	float aside_vel_vector[DIMENSION];
	mathVecSubtract(aside_vel_vector, vel, ahead_vel_vector, DIMENSION);

	float aside_vel;
	aside_vel = mathVecMagnitude(aside_vel_vector, DIMENSION);

	float aside[DIMENSION];
	mathVecGetNormal(aside, aside_vel_vector, DIMENSION);

	float t_stop;
	t_stop = absf(ahead_vel / MAX_FORCE);
	phase1_moveto_t_stop = t_stop;

	float dist_stop;
	dist_stop = (MAX_FORCE*t_stop*t_stop/2)*4;
	phase1_moveto_dist_stop = dist_stop;
	phase1_moveto_dist = d;

	bool slow_down;
	slow_down = (dist_stop > d);

	if(seconds > 45)
		DEBUG(("%i:%s\n", seconds, slow_down ? "Frenando..." : "Acelerando..."));

	float ahead_force[DIMENSION];
	mathVecScalarMult(ahead_force, ahead, (slow_down) ? -MAX_FORCE : MAX_FORCE, DIMENSION);

	float aside_force[DIMENSION];
	mathVecScalarMult(aside_force, aside, -1, DIMENSION);

	//mathVecAdd(forces, aside_force, ahead_force, DIMENSION);
	mathVecAdd(forces, forces, ahead_force, DIMENSION);

	api.setForces(forces);
}
