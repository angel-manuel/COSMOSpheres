//lasso
//Contains functions to perform lassoing

bool lasso_picking;	//true if we are currently lassoing
int lasso_last_debris;
float lasso_initial_rel_pos[3];

//lasso_init
//Gives a initial value for the vars on lasso.cpp
void lasso_init() {
	lasso_picking = false;
	lasso_last_debris = -1;
}

//lasso_pick
//Tries to lasso the item with number 'debris_number' and returns true when finished
bool lasso_pick(int debris_number) {
	#ifdef DEBUG_ACTIVE
	DEBUG(("lasso: %i\n", debris_number));
	#endif

	if(is_debris_collected[debris_number]) {
		//If the debris is collected, return true
		lasso_picking = false;
		return true;
	}

	if(debris_number != lasso_last_debris) {
		//If we change the debris number we abort the lassoing
		lasso_last_debris = debris_number;
		lasso_picking = false;
	}

	float delta[3];	//Holds the relative position
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
			#ifdef DEBUG_ACTIVE
			DEBUG(("lasso: Starting lassoing item %i\n", debris_number));
			#endif
		}
		#ifdef DEBUG_ACTIVE
		DEBUG(("lasso: Lassoing item %i\n", debris_number));
		#endif

		float head_vel[3];
		mathVecCross(head_vel, &our_state[VEL], delta);
		float side_vel[3];
		mathVecSubtract(side_vel, &our_state[VEL], head_vel, 3);
		float head_speed = mathVecNormalize(head_vel, 3);
		float side_speed = mathVecNormalize(side_vel, 3);

		float normal_impulse = -(head_speed*head_speed/d);
		float head_impulse = 0.001f;

		float force[3];
		float normal_force[3];
		float head_force[3];
		mathVecScalarMult(normal_force, side_vel, normal_impulse, 3);
		mathVecScalarMult(head_force, head_vel, head_impulse, 3);
		mathVecAdd(force, normal_force, head_force, 3);
		api.setForces(force);

		#ifdef DEBUG_ACTIVE
		DEBUG(("force = "));debug_print_vector(force, 3);
		#endif
	} else {
		#ifdef DEBUG_ACTIVE
		DEBUG(("lasso: Going for item %i\n", debris_number));
		#endif
		float target_pos[3];
		float out[3] = {MAX_LASSO_DIST - 0.01f, 0.0f, 0.0f};

		mathVecAdd(target_pos, debris_position[debris_number], out, 3);

		api.setPositionTarget(target_pos);
	}

	return false;
}

//lasso_next
//Returns the number of the nearest debris
int lasso_next() {
	float min_cost = 1000000.0f;
	int best_debris = -1;

	float delta[3];
	float cost;

	int debris;
	for(debris = 0; debris < NUMBER_OF_DEBRIS; ++debris) {
		if(!is_debris_collected[debris]) {
			mathVecSubtract(delta, &our_state[POS], debris_position[debris], 3);
			cost = mathVecMagnitude(delta, 3);

			if(cost < min_cost) {
				min_cost = cost;
				best_debris = debris;
			}
		}
	}

	return best_debris;
}
