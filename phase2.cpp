#define PHASE2_PREDICTION_TIME 2
#define PHASE2_STRATEGY_GRAVITY 2
#define PHASE2_STRATEGY_10_SHOTS 1
#define PHASE2_STRATEGY_20_SHOTS 0
#define PHASE2_STRATEGY_NONE -1
#define PHASE2_TOLERANCE 0.015f

int phase2_strategy;
//phase2_strategy
//0 -> Stay in place and shoot(10 laser shots)
//1 -> Follow the comet and shoot(20 laser shots)
//2 -> Only gravity

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

	
	if(phase2_strategy != PHASE2_STRATEGY_GRAVITY && laser_shots_left == 0) {
		#ifdef DEBUG_ACTIVE
		DEBUG(("phase2: PHASE2_STRATEGY_GRAVITY\n"));
		#endif
		phase2_strategy = PHASE2_STRATEGY_GRAVITY;
	}
	

	if(phase2_follow()) {
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
//Se coloca en posición para disparar al cometa por primera vez
void phase2_prepare() {
	//If we dont have a strategy we decide what to do
	float target_pos[3];
	float target_att[3];
	
	//Target and attitude position in each strategy except gravity
	switch(phase2_strategy){
		case PHASE2_STRATEGY_NONE:
			phase2_set_strategy();
		case PHASE2_STRATEGY_20_SHOTS:
			target_pos[POS_X] = (blue_sphere) ? 0.4f : -0.4f;
			target_pos[POS_Y] = 0.6f;
			target_pos[POS_Z] = (blue_sphere) ? 0.4f : -0.4f;
			target_att[POS_X] = ((blue_sphere) ? 0.1f : -0.1f) - target_pos[POS_X];
			target_att[POS_Y] = 0.1f;
			target_att[POS_Z] = -target_pos[POS_Z];
			movement_moveto(target_pos, false);
			mathVecNormalize(target_att, 3);
			api.setAttitudeTarget(target_att);
			break;
		case PHASE2_STRATEGY_10_SHOTS:
			target_pos[POS_X] = (blue_sphere) ? -0.1f: 0.1f;
			target_pos[POS_Y] = 0.2f;
			target_pos[POS_Z] = (blue_sphere) ? 0.15f : -0.15f;
			target_att[POS_X] = ((blue_sphere) ? 0.1f : -0.1f) - target_pos[POS_X];
			target_att[POS_Y] = 0.5f;
			target_att[POS_Z] = -target_pos[POS_Z];
			movement_moveto(target_pos, false);
			mathVecNormalize(target_att, 3);
			api.setAttitudeTarget(target_att);
			break;
		case PHASE2_STRATEGY_GRAVITY:
			float dist = sqrtf((COMET_RADIUS + SPHERE_RADIUS)*(COMET_RADIUS + SPHERE_RADIUS)/2) + 0.01f;
			target_pos[POS_X] = 0.18f;
			target_pos[POS_Y] = 0.7f;
			target_pos[POS_Z] = 2.0f*(blue_sphere ? dist : -dist);
			api.setPositionTarget(target_pos);
			break;
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

	switch(phase2_strategy) {
		case PHASE2_STRATEGY_20_SHOTS:
			if(ABS(our_state[POS_X]) > 0.55f && target_vel[POS_X]*our_state[POS_X] > 0.0f) {
				target_vel[POS_X] = 0.0f;
			}
			if(ABS(our_state[POS_Z]) > 0.55f && target_vel[POS_Z]*our_state[POS_Z] > 0.0f) {
				target_vel[POS_Z] = 0.0f;
			}
			if(laser_shots_left == 0) {
				target_vel[POS_X] = -2.0f*target_vel[POS_X];
			}
			api.setVelocityTarget(target_vel);
			api.setAttitudeTarget(fut_att);
			break;
		case PHASE2_STRATEGY_10_SHOTS:
			target_vel[POS_X] = target_vel[POS_Y] = target_vel[POS_Z] = 0.0f;
			api.setVelocityTarget(target_vel);
			api.setAttitudeTarget(fut_att);
			break;
		case PHASE2_STRATEGY_GRAVITY:
			//TODO: api.setVelocityTarget
			float target_pos[3];
			float rel_target_pos[3] = {
				0.0f,
				0.08f,
				0.0f
			};

			mathVecAdd(target_pos, &our_comet_state[POS], rel_target_pos, 3);
			api.setPositionTarget(target_pos);
			break;
	}

	return ret;
}
