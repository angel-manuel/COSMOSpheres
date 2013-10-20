float target_att[3];

void phase2_init() {
	target_att[0] = (blue_sphere) ? -1.0f : 1.0f;
	target_att[1] = (blue_sphere) ? -1.0f : 1.0f;
	target_att[2] = 0.0f;
	mathVecNormalize(target_att, 3);
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
	float target_pos[3] = {(blue_sphere) ? 0.4f : -0.4f, 0.7f, (blue_sphere) ? 0.4f : -0.4f};
	//float target_pos[3] = {our_state[POS_X], 0.5f, our_state[POS_Z]};

	api.setPositionTarget(target_pos);
	//api.setAttitudeTarget(target_att);
}

bool phase2_follow() {
	float raycast[5];
	distanceToDebris(&our_state[POS], &our_state[ATT], &our_comet_state[POS], raycast);

	if(raycast[4] < COMET_RADIUS) {
		api.setVelocityTarget(&our_comet_state[VEL]);
		//api.setAttitudeTarget(target_att);
		return true;
	}

	#ifdef DEBUG_ACTIVE
	float angle = atan2f(raycast[4], raycast[3]);
	DEBUG(("angle = %f\n", angle));
	#endif

	api.setVelocityTarget(&our_comet_state[VEL]);
	//api.setAttitudeTarget(target_att);
	return false;
}
