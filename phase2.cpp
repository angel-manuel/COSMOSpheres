#define PHASE2_PREDICTION_TIME 2
#define PHASE2_STRATEGY_GRAVITY 2
#define PHASE2_STRATEGY_STAY_AND_SHOOT 1
#define PHASE2_STRATEGY_FOLLOW_AND_SHOOT 0
#define PHASE2_STRATEGY_NONE -1

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
			phase2_strategy = PHASE2_STRATEGY_STAY_AND_SHOOT;
			break;
		case 20:
			phase2_strategy = PHASE2_STRATEGY_FOLLOW_AND_SHOOT;
			break;
		default:
			phase2_strategy = PHASE2_STRATEGY_GRAVITY;
	}
}

void phase2_loop() {
	//If we dont have a strategy we decide what to do
	if(phase2_strategy == PHASE2_STRATEGY_NONE) {
		phase2_set_strategy();
	}

	#ifdef DEGUB_ACTIVE
	DEBUG(("phase2:laser_shots_left = %i\n", laser_shots_left));
	#endif

	if(laser_shots_left == 0 && phase2_strategy != PHASE2_STRATEGY_FOLLOW_AND_SHOOT) {
		phase2_strategy = PHASE2_STRATEGY_GRAVITY;
	}

	if(phase2_follow()) {
		#ifdef DEGUB_ACTIVE
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
		case PHASE2_STRATEGY_FOLLOW_AND_SHOOT:
			target_pos[POS_X] = our_state[POS_X];
			target_pos[POS_Y] = 0.4f;
			target_pos[POS_Z] = 0.4f;
			target_att[POS_X] = -our_state[POS_X];
			target_att[POS_Y] = 0.4f;
			target_att[POS_Z] = -0.4f;
			movement_moveto(target_pos, false);
			mathVecNormalize(target_att, 3);
			api.setAttitudeTarget(target_att);
			break;
		case PHASE2_STRATEGY_STAY_AND_SHOOT:
			target_pos[POS_X] = target_pos[POS_Z] = (blue_sphere) ? -0.1f: 0.1f;
			target_pos[POS_Y] = 0.2f;
			target_att[POS_Y] = target_att[POS_Z] = 0.0f;
			target_att[POS_Y] = 0.6f;
			movement_moveto(target_pos, false);
			mathVecNormalize(target_att, 3);
			api.setAttitudeTarget(target_att);
			break;
		case PHASE2_STRATEGY_GRAVITY:
			float dist = sqrtf((COMET_RADIUS + SPHERE_RADIUS)*(COMET_RADIUS + SPHERE_RADIUS)/2) + 0.01f;
			target_pos[POS_X] = target_pos[POS_Z] = blue_sphere ? dist : dist;
			target_pos[POS_Y] = 0.7f;
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

		ret = raycast[4] < (COMET_RADIUS - 0.002f);
	} else {
		ret = false;
	}

	float fut_comet_state[6];
	game.predictCometState(PHASE2_PREDICTION_TIME, our_comet_state, fut_comet_state);
	
	float fut_sphere_pos[3];
	float fut_sphere_delta[3];
	mathVecScalarMult(fut_sphere_delta, &our_state[VEL], PHASE2_PREDICTION_TIME, 3);
	mathVecAdd(fut_sphere_pos, &our_state[POS], fut_sphere_delta, 3);

	float fut_att[3];
	mathVecSubtract(fut_att, &fut_comet_state[POS], fut_sphere_pos, 3);
	mathVecNormalize(fut_att, 3);
	
	float target_vel[3] = {our_comet_state[VEL_X], our_comet_state[VEL_Y], our_comet_state[VEL_Z]};

	switch(phase2_strategy) {
		case PHASE2_STRATEGY_FOLLOW_AND_SHOOT:
			target_vel[POS_X] = target_vel[POS_Z] = 0.0f;
			api.setVelocityTarget(target_vel);
			api.setAttitudeTarget(fut_att);
			break;
		case PHASE2_STRATEGY_STAY_AND_SHOOT:
			target_vel[POS_X] = target_vel[POS_Z] = 0.0f;
			api.setVelocityTarget(target_vel);
			api.setAttitudeTarget(fut_att);
			break;
		case PHASE2_STRATEGY_GRAVITY:
			//TODO: api.setVelocityTarget
			float target_pos[3];
			float dist = sqrtf((COMET_RADIUS + SPHERE_RADIUS)*(COMET_RADIUS + SPHERE_RADIUS)/2) + 0.01f;
			float rel_target_pos[3] = {
				(blue_sphere) ? dist : -dist,
				0.0f,
				(blue_sphere) ? dist : -dist
			};

			mathVecAdd(target_pos, &our_comet_state[POS], rel_target_pos, 3);
			api.setPositionTarget(target_pos);
			break;
	}

	return ret;
}
