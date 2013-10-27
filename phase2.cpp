#define PHASE2_PREDICTION_TIME 3

int phase2_strategy;

void phase2_init() {
	phase2_strategy = -1;
}

void phase2_loop() {
	#ifdef DEGUB_ACTIVE
	DEBUG(("phase2:laser_shots_left = %i\n", laser_shots_left));
	#endif

	if(laser_shots_left > 0) {
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
	} else {
		our_comet_state[POS_X] += (blue_sphere) ? -0.20f: 0.20f;
		our_comet_state[POS_Y] += 0.1f;
		api.setPositionTarget(&our_comet_state[POS]);
	}
}

//phase2_prepare
//Se coloca en posición para disparar al cometa por primera vez
void phase2_prepare() {
	if(phase2_strategy == -1) {
		phase2_strategy = (laser_shots_left == 20) ? 1 : 0;
	}

	float target_pos[3] = {(blue_sphere) ? 0.5f : -0.5f, (phase2_strategy == 1) ? 0.5f : 0.4f, (blue_sphere) ? 0.4f : -0.4f};
	float target_att[3] = {(blue_sphere) ? -0.5f : 0.5f, (phase2_strategy == 1) ? 0.3f : 0.4f, (blue_sphere) ? -0.4f : 0.4f};
	if(phase2_strategy == 0) {
		target_pos[POS_X] = (blue_sphere) ? 0.1f : -0.1f;
		target_att[POS_X] = (blue_sphere) ? -0.1f : 0.1f;
		target_pos[POS_Z] *= -1.0f;
		target_att[POS_Z] *= -1.0f;
	}
	mathVecNormalize(target_att, 3);

	api.setPositionTarget(target_pos);
	api.setAttitudeTarget(target_att);
}

bool phase2_follow() {
	bool ret;
	float raycast[6];
	//debris_position[0] = head
	mathVecAdd(debris_position[0], &our_state[POS], &our_state[ATT], 3);
	distanceToDebris(&our_state[POS], debris_position[0], &our_comet_state[POS], raycast);

	ret = raycast[4] < (COMET_RADIUS - 0.001f);

	//raycast = fut_comet_state
	float fut_comet_state[6];
	game.predictCometState(PHASE2_PREDICTION_TIME, our_comet_state, fut_comet_state);
	//debris_position[1] = fut_state
	mathVecAdd(debris_position[1], &our_state[POS], &our_state[VEL], 3);

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
