//movement
//Deals with the movement acroos the debris_field
#define MOVEMENT_MARGIN 0.03f
#define MOVEMENT_CORRECTION 0.06f

#define HEAD_DIST 3
#define SIDE_DIST 4

#ifdef DEBUG_ACTIVE
float movement_last_debris;
float movement_distance;
#endif

#ifdef DEBUG_ACTIVE
void movement_init() {
	movement_last_debris = -1.0f;
	movement_distance = 0.0f;
	debug_track(5, &movement_distance, (char*)"movement_debris_distance");
	debug_track(6, &movement_last_debris, (char*)"movement_last_debris");
}
#endif

//movement_moveto
//dst -> Destiny
//direct -> If true does not uses debris avoidance
//return -> true if we have arrived, false otherwise
bool movement_moveto(float dst[3], bool direct) {
	float delta[3];
	float head[3];

	mathVecSubtract(delta, dst, &our_state[POS], 3);		//delta = dst - pos
	mathVecScalarMult(head, delta, 1.0f/mathVecMagnitude(delta, 3), 3);  //head = delta / |delta|

	if(mathVecMagnitude(delta, 3) < MAX_ITEM_START_DIST && mathVecMagnitude(&our_state[VEL], 3) < MAX_ITEM_START_VEL) {
		return true;
	} else {
		//delta = zero
		delta[0] = delta[1] = delta[2] = 0.0f;
		api.setAttRateTarget(delta);
	}

	float head_vel[3];
	mathVecScalarMult(head_vel, head, mathVecInner(head, &our_state[VEL], 3), 3);
	float head_speed = mathVecMagnitude(head_vel, 3);
	float side_vel[3];
	mathVecSubtract(side_vel, &our_state[VEL], head_vel, 3);

	const float danger_radius = (SPHERE_RADIUS + DEBRIS_RADIUS) + MOVEMENT_MARGIN;
	const float correction = danger_radius + MOVEMENT_CORRECTION;

	if(direct || seconds >= 90) {
		api.setPositionTarget(dst);
		return false;
	}

	int debris_number;
	float debris_vector[5];
	int nearest_debris = -1;
	float nearest_debris_distance = 1000000.0f;
	float nearest_debris_vector[5];
	for(debris_number = 0; debris_number < NUMBER_OF_DEBRIS; debris_number++) {
		if(!is_debris_collected[debris_number]) {
			distanceToDebris(&our_state[POS], dst, debris_position[debris_number], debris_vector);
			if((debris_vector[SIDE_DIST] - (danger_radius + ((mathVecInner(side_vel, debris_vector, 3)/head_speed)*debris_vector[3]))) < 0.0f) {
				//Colisión
				if(debris_vector[HEAD_DIST] < nearest_debris_distance && debris_vector[HEAD_DIST] > 0.0f) {
					nearest_debris = debris_number;
					int i;
					for(i = 0; i < 5; ++i) {
						nearest_debris_vector[i] = debris_vector[i];
					}
					nearest_debris_distance = debris_vector[3];
				}
			}
		}
	}

	api.setPositionTarget(dst);

	if(nearest_debris >= 0) { //Si nearest_debris es un debris válido
		
		float useful_side_vel = mathVecInner(side_vel, nearest_debris_vector, 3);
		float useful_deviation_per_meter = useful_side_vel/head_speed;
		float useful_deviation = useful_deviation_per_meter*nearest_debris_vector[HEAD_DIST];
		float time_to_impact = (nearest_debris_vector[HEAD_DIST]/head_speed);
		float needed_correction = (correction - (nearest_debris_vector[SIDE_DIST] + useful_deviation));

		/*
		mathVecScalarMult(delta, nearest_debris_vector, needed_correction, 3);

		float target_pos[3];
		mathVecAdd(target_pos, delta, debris_position[nearest_debris], 3);
		api.setPositionTarget(delta);

		#ifdef DEBUG_ACTIVE
		DEBUG(("movement: Going to "));debug_print_vector(target_pos, 3);
		#endif
		*/
		
		
		float needed_speed_up = needed_correction/time_to_impact;
		mathVecScalarMult(delta, nearest_debris_vector, SPHERE_INERTIAL_MASS*needed_speed_up, 3);

		#ifdef DEBUG_ACTIVE
		DEBUG(("needed_speed_up = %f\n", needed_speed_up));
		#endif

		api.setForces(delta);
	}
	#ifdef DEBUG_ACTIVE
	else {
		movement_last_debris = -1.0f;
		nearest_debris_distance = 0.0f;
		DEBUG(("movement:Clear!\n"));
	}
	#endif

	return false;
}
