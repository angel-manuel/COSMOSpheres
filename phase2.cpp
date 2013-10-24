void phase2_init() {
}

void phase2_loop() {
	#ifdef DEGUB_ACTIVE
	DEBUG(("laser_shots_left = %i\n", laser_shots_left));
	#endif
	if(laser_shots_left > 0) {
		if(phase2_follow()) {
			#ifdef DEGUB_ACTIVE
			if(game.shootLaser()) {
				DEBUG(("phase2:Bang!\n"));
			} else {
				DEBUG(("phase2:Missed shot"));
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
	float target_pos[3] = {(blue_sphere) ? 0.3f : -0.3f, 0.2f, (blue_sphere) ? -0.6f : 0.6f};
	float target_att[3] = {(blue_sphere) ? -0.3f : 0.3f, 0.7f, (blue_sphere) ? 0.7f : -0.7f};
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

	#ifdef DEBUG_ACTIVE
	float angle = atan2f(raycast[4], raycast[3]);
	DEBUG(("phase2:angle = %f\n", angle));
	DEBUG(("phase2_dist = %f\n", raycast[4]));
	#endif

	float target_att[3];
	float fut_state[6];
	game.predictCometState(1, our_comet_state, fut_state);
	mathVecSubtract(target_att, &fut_state[POS], &our_state[POS], 3);
	mathVecNormalize(target_att, 3);

	float zero[3] = {0.0f};
	api.setVelocityTarget(zero);
	api.setAttitudeTarget(target_att);

	return false;
}
