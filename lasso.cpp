//lasso
//Contains functions to perform lassoing

bool lasso_picking;
int lasso_last_debris;
float lasso_initial_rel_pos[3];

void lasso_init() {
	lasso_picking = false;
	lasso_last_debris = -1;
}

bool lasso_pick(int debris_number) {
	if(is_debris_collected[debris_number]) {
		lasso_picking = false;
		return true;
	}

	if(debris_number != lasso_last_debris) {
		lasso_last_debris = debris_number;
	}

	float delta[3];
	mathVecSubtract(delta, debris_position[debris_number], &our_state[POS], 3);
	float d = mathVecNormalize(delta, 3);

	if(lasso_picking || (d < MAX_LASSO_DIST && d > MIN_LASSO_DIST)) {
		if(!lasso_picking) {
			int i;
			for(i = 0; i < 3; ++i) {
				lasso_initial_rel_pos[i] = delta[i];
			}
			lasso_picking = true;
			game.startLasso(debris_number);
		}

		float head_vel[3];
		mathVecCross(head_vel, &our_state[VEL], delta);
		float side_vel[3];
		mathVecSubtract(side_vel, &our_state[VEL], head_vel, 3);
		float head_speed = mathVecNormalize(head_vel, 3);
		float side_speed = mathVecNormalize(side_vel, 3);

		float normal_impulse = (head_speed*head_speed/d);
		float head_impulse = 0.001f;

		float force[3];
		float normal_force[3];
		float head_force[3];
		mathVecScalarMult(normal_force, side_vel, normal_impulse, 3);
		mathVecScalarMult(head_force, head_vel, head_impulse, 3);
		mathVecAdd(force, normal_force, head_force, 3);
		api.setForces(force);
	} else {
		api.setPositionTarget(debris_position[debris_number]);
	}

	return false;
}

int lasso_next() {
	float min_cost = -1000000.0f;
	int best_debris = -1;

	float delta[3];
	float cost;

	int debris;
	for(debris = 0; debris < NUMBER_OF_DEBRIS; ++debris) {
		if(!is_debris_collected[debris]) {
			mathVecSubtract(delta, &our_state[POS], debris_position[debris], 3);
			cost = mathVecMagnitude(delta, 3);
			if(cost < min_cost) {
				cost = min_cost;
				best_debris = debris;
			}
		}
	}

	return best_debris;
}
