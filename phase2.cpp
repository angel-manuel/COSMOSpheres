void phase2_init() {
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
	}
}

//phase2_prepare
//Se coloca en posición para disparar al cometa por primera vez
void phase2_prepare() {
	float target_pos[3] = {(blue_sphere) ? 0.5f : -0.5f, 0.5f, (blue_sphere) ? 0.4f : -0.4f};
	float target_att[3] = {(blue_sphere) ? -0.5f : 0.5f, 0.3f, (blue_sphere) ? -0.4f : 0.4f};
	mathVecNormalize(target_att, 3);

	api.setPositionTarget(target_pos);
	api.setAttitudeTarget(target_att);
}

bool phase2_follow() {
	float raycast[5];
	float head[3];
	mathVecAdd(head, &our_state[POS], &our_state[ATT], 3);
	distanceToDebris(&our_state[POS], head, &our_comet_state[POS], raycast);

	if(raycast[4] < COMET_RADIUS) {
		return true;
	}

	
	float fut_comet_state[6];
	game.predictCometState(1, our_comet_state, fut_comet_state);

	float target_vel[3] = {0.0f, our_comet_state[VEL_Y], our_comet_state[VEL_Z]};
	api.setVelocityTarget(target_vel);

	float target_att[3];
	float fut_state[3];
	mathVecAdd(fut_state, &our_state[POS], &our_state[VEL], 3);
	mathVecSubtract(target_att, &fut_comet_state[POS], &fut_state[POS], 3);
	mathVecNormalize(target_att, 3);

	api.setAttitudeTarget(target_att);

	return false;
}
