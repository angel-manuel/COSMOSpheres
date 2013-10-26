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
		api.setPositionTarget(&our_comet_state[POS]);
	}
}

//phase2_prepare
//Se coloca en posición para disparar al cometa por primera vez
void phase2_prepare() {
	if(phase2_strategy == -1) {
		phase2_strategy = (laser_shots_left == 20) ? 1 : 0;
	}

	float target_pos[3] = {(blue_sphere) ? 0.5f : -0.5f, (phase2_strategy == 1) ? 0.5f : 0.2f, (blue_sphere) ? 0.4f : -0.4f};
	float target_att[3] = {(blue_sphere) ? -0.5f : 0.5f, (phase2_strategy == 1) ? 0.3f : 0.6f, (blue_sphere) ? -0.4f : 0.4f};
	if(phase2_strategy == 0) {
		target_pos[2] *= -1.0f;
		target_att[2] *= -1.0f;
	}
	mathVecNormalize(target_att, 3);

	api.setPositionTarget(target_pos);
	api.setAttitudeTarget(target_att);
}

bool phase2_follow() {
	bool ret;
	float raycast[6];
	float tmp[3];
	//tmp = head
	mathVecAdd(tmp, &our_state[POS], &our_state[ATT], 3);
	distanceToDebris(&our_state[POS], tmp, &our_comet_state[POS], raycast);

	ret = raycast[4] < COMET_RADIUS;

	//raycast = fut_comet_state
	float fut_comet_state[6];
	game.predictCometState(1, our_comet_state, fut_comet_state);
	//tmp = fut_state
	mathVecAdd(tmp, &our_state[POS], &our_state[VEL], 3);

	if(phase2_strategy == 1) {
		float target_vel[3] = {0.0f, our_comet_state[VEL_Y], our_comet_state[VEL_Z]};
		api.setVelocityTarget(target_vel);
	} else {
		float target_vel[3] = {0.0f};
		api.setVelocityTarget(target_vel);
	}

	//raycast = target_att
	mathVecSubtract(raycast, &fut_comet_state[POS], &tmp[POS], 3);
	mathVecNormalize(raycast, 3);

	api.setAttitudeTarget(raycast);

	return ret;
}
