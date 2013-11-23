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

	if(laser_shots_left == 0) {
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
	
	//If we don't have more laser shots, we use the gravity strategy (in development)
	if(phase2_strategy = PHASE2_STRATEGY_GRAVITY){
		our_comet_state[POS_X] += (blue_sphere) ? -0.20f: 0.20f;
		our_comet_state[POS_Y] += 0.1f;
		api.setPositionTarget(&our_comet_state[POS]);
	}
}

//phase2_prepare
//Se coloca en posición para disparar al cometa por primera vez
void phase2_prepare() {
	//If we dont have a strategy we decide what to do
	float target_pos[3] = {(blue_sphere) ? 0.5f : -0.5f, 0.4f, (blue_sphere) ? 0.4f : -0.4f};
	float target_att[3] = {(blue_sphere) ? -0.5f : 0.5f, 0.4f, (blue_sphere) ? -0.4f : 0.4f};
	
	//Target and attitude position in each strategy except gravity
	switch(phase2_strategy){
		case PHASE2_STRATEGY_NONE:
			phase2_set_strategy();
		case PHASE2_STRATEGY_FOLLOW_AND_SHOOT:
			target_pos[POS_X] = (blue_sphere) ? 0.1f : -0.1f;
			target_pos[POS_Z] *= -1.0f;
			target_att[POS_X] = (blue_sphere) ? -0.1f : 0.1f;
			target_att[POS_Z] *= -1.0f;
			api.setPositionTarget(target_pos);
			mathVecNormalize(target_att, 3);
			api.setAttitudeTarget(target_att);
			break;
		case PHASE2_STRATEGY_STAY_AND_SHOOT:
			target_pos[POS_Y] = 0.5f;
			target_att[POS_Y] = 0.3f;
			api.setPositionTarget(target_pos);
			mathVecNormalize(target_att, 3);
			api.setAttitudeTarget(target_att);
			break;
		case PHASE2_STRATEGY_GRAVITY:
			target_pos[POS_X] = target_pos[POS_Z] = 0.0f;
			target_pos[POS_Y] = 0.7f;
			api.setPositionTarget(target_pos);
			break;
	}
}

bool phase2_follow() {
	bool ret;
	float raycast[6];
	//debris_position[0] = head
	mathVecAdd(debris_position[0], &our_state[POS], &our_state[ATT], 3);
	distanceToDebris(&our_state[POS], debris_position[0], &our_comet_state[POS], raycast);

	ret = raycast[4] < (COMET_RADIUS - 0.002f);

	//raycast = fut_comet_state
	float fut_comet_state[6];
	game.predictCometState(PHASE2_PREDICTION_TIME, our_comet_state, fut_comet_state);
	//debris_position[1] = fut_comet_state
	//debris_position[2] will be the velocity vector * 3;
	mathVecScalarMult(debris_position[2], &our_state[VEL], PHASE2_PREDICTION_TIME, 3);
	mathVecAdd(debris_position[1], &our_state[POS], debris_position[2], 3);

	//debris_position[2] = target_vel
	debris_position[2][POS_X] = 0.0f;
	debris_position[2][POS_Y] = our_comet_state[VEL_Y];
	debris_position[2][POS_Z] = (phase2_strategy == 1) ? our_comet_state[VEL_Z] : 0.0f;
	api.setVelocityTarget(debris_position[2]);

	//raycast = target_att
	mathVecSubtract(raycast, &fut_comet_state[POS], debris_position[1], 3);
	mathVecNormalize(raycast, 3);

	api.setAttitudeTarget(raycast);

	return ret;
}
