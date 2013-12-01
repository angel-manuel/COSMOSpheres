#define PHASE2_TOLERANCE 0.01f
#define PHASE2_PREDICTION_TIME 2
#define PHASE2_VEL_PREP_TIME 88

void phase2_init() {
	phase2_strategy = PHASE2_STRATEGY_NONE;
}

//Choose a strategy by checking the number of shots left
void phase2_set_strategy() {
	switch(laser_shots_left) {
		case 0:
			phase2_strategy = PHASE2_STRATEGY_GRAVITY;
			break;
		case 10:
			phase2_strategy = PHASE2_STRATEGY_10_SHOTS;
			break;
		case 20:
			phase2_strategy = PHASE2_STRATEGY_20_SHOTS;
			break;
		default:
			if(laser_shots_left > 0) {
				phase2_strategy = PHASE2_STRATEGY_10_SHOTS;
			} else {
				phase2_strategy = PHASE2_STRATEGY_GRAVITY;
			}
	}

	#ifdef DEBUG_ACTIVE
	switch(phase2_strategy) {
		case PHASE2_STRATEGY_20_SHOTS:
			DEBUG(("phase2: PHASE2_STRATEGY_20_SHOTS\n"));
			break;
		case PHASE2_STRATEGY_10_SHOTS:
			DEBUG(("phase2: PHASE2_STRATEGY_10_SHOTS\n"));
			break;
		case PHASE2_STRATEGY_GRAVITY:
			DEBUG(("phase2: PHASE2_STRATEGY_GRAVITY\n"));
			break;
	}
	#endif
}

void phase2_loop() {
	//If we dont have a strategy we decide what to do
	if(phase2_strategy == PHASE2_STRATEGY_NONE) {
		phase2_set_strategy();
	}

	#ifdef DEBUG_ACTIVE
	DEBUG(("phase2:laser_shots_left = %i\n", laser_shots_left));
	#endif

	if(phase2_aim()) {
		#ifdef DEBUG_ACTIVE
		if(game.shootLaser()) {
			DEBUG(("phase2:Bang!\n"));
		} else {
			DEBUG(("phase2:Missed shot\n"));
		}
		#else
		game.shootLaser();
		#endif
	}
}

//phase2_prepare
//Prepares the SPHERE before phase2 starts
void phase2_prepare() {
	float target_pos[3] = {
		blue_sphere ? -0.2f : 0.2f,
		0.5f,
		blue_sphere ? 0.25f : -0.25f
	};
	float target_att[3] = {
		blue_sphere ? 0.5f : -0.5f,
		0.3f,
		blue_sphere ? -0.3f : 0.3f
	};
	float target_vel[3] = {
		0.0f,
		0.0f,
		0.0f
	};

	mathVecNormalize(target_att, 3);

	api.setAttitudeTarget(target_att);

	if(seconds < PHASE2_VEL_PREP_TIME) {
		api.setPositionTarget(target_pos);
	} else {
		api.setVelocityTarget(target_vel);
	}
}

bool phase2_follow() {
	bool ret;

	if(laser_shots_left > 0 && !(phase2_strategy == PHASE2_STRATEGY_GRAVITY)) {
		float raycast[6];
		//debris_position[0] = head
		mathVecAdd(debris_position[0], &our_state[POS], &our_state[ATT], 3);
		distanceToDebris(&our_state[POS], debris_position[0], &our_comet_state[POS], raycast);

		ret = raycast[4] < (COMET_RADIUS - PHASE2_TOLERANCE);
	} else {
		ret = false;
	}

	float fut_comet_state[6];
	game.predictCometState(PHASE2_PREDICTION_TIME, our_comet_state, fut_comet_state);
	
	float fut_sphere_pos[3];
	float fut_sphere_delta[3];
	mathVecScalarMult(fut_sphere_delta, &our_state[VEL], PHASE2_PREDICTION_TIME + 0.5f, 3);
	mathVecAdd(fut_sphere_pos, &our_state[POS], fut_sphere_delta, 3);

	float fut_att[3];
	mathVecSubtract(fut_att, &fut_comet_state[POS], fut_sphere_pos, 3);
	mathVecNormalize(fut_att, 3);
	
	float target_vel[3] = {our_comet_state[VEL_X], our_comet_state[VEL_Y], our_comet_state[VEL_Z]};

	if(our_comet_state[POS_Y] < (our_state[POS_Y] + 0.1f)) {
		#ifdef DEBUG_ACTIVE
		DEBUG(("phase2: Catching the comet(%f < %f)\n", our_comet_state[POS_Y], our_state[POS_Y] + 0.1f));
		#endif
		api.setVelocityTarget(target_vel);
	} else {
		#ifdef DEBUG_ACTIVE
		DEBUG(("phase2: Staying(%f > %f)\n", our_comet_state[POS_Y], our_state[POS_Y] + 0.1f));
		#endif
	}

	api.setAttitudeTarget(fut_att);

	return ret;
}
