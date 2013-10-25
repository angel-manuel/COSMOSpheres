#define HEAD_DIST 3
#define SIDE_DIST 4

#ifdef DEBUG_ACTIVE
float movement_last_debris;
float movement_distance;
#endif

void movement_init() {
	#ifdef DEBUG_ACTIVE
	movement_last_debris = -1.0f;
	movement_distance = 0.0f;
	debug_track(5, &movement_distance, (char*)"movement_debris_distance");
	debug_track(6, &movement_last_debris, (char*)"movement_last_debris");
	#endif
}

//movement_moveto
//dst -> Destino
//tvel -> Velocidad objetivo
//return -> true si ya se ha llegado, false en todos los demas casos
bool movement_moveto(float dst[3]) {
	float delta[3];
	float head[3];

	mathVecSubtract(delta, dst, &our_state[POS], 3);		//delta = dst - pos
	mathVecGetNormal(head, delta, 3);						//head = delta / |delta|

	float head_vel[3];
	mathVecScalarMult(head_vel, head, mathVecInner(head, &our_state[VEL], 3), 3);
	float head_speed = mathVecMagnitude(head_vel, 3);
	float side_vel[3];
	mathVecSubtract(side_vel, &our_state[VEL], head_vel, 3);

	const float danger_radius = (SPHERE_RADIUS + DEBRIS_RADIUS) + 0.03f;
	const float correction = danger_radius + 0.02f;
	
	if(mathVecMagnitude(delta, 3) < MAX_ITEM_START_DIST && mathVecMagnitude(&our_state[VEL], 3) < MAX_ITEM_START_VEL) {
		return true;
	}

	if(seconds >= 90 || !game.isNetBroken()) {
		api.setPositionTarget(dst);
		return false;
	}

	int debris_number;
	float debris_vector[5];
	int nearest_debris = -1;
	float nearest_debris_distance = 1000000.0f;
	float nearest_debris_vector[5];
	float desviation;
	for(debris_number = 0; debris_number < NUMBER_OF_DEBRIS; debris_number++) {
		if(!is_debris_collected[debris_number]) {
			distanceToDebris(&our_state[POS], head, debris_position[debris_number], debris_vector);
			desviation = (mathVecInner(side_vel, debris_vector, 3)/head_speed)*debris_vector[3];
			if((debris_vector[SIDE_DIST] - (danger_radius + desviation)) < 0.0f) {
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

	if(nearest_debris >= 0 && nearest_debris < NUMBER_OF_DEBRIS && nearest_debris_distance < mathVecMagnitude(delta, 3)) { //Si nearest_debris es un debris válido
		float tmp[3];
		float next[3];

		mathVecScalarMult(tmp, nearest_debris_vector, correction, 3);
		mathVecAdd(next, debris_position[nearest_debris], tmp, 3);

		if(ABS(next[POS_X]) > 0.64f || ABS(next[POS_Y]) > 0.8f || ABS(next[POS_Z]) > 0.64f) {
			mathVecScalarMult(tmp, nearest_debris_vector, -correction, 3);
			mathVecAdd(next, debris_position[nearest_debris], tmp, 3);
		}

		#ifdef DEBUG_ACTIVE
		movement_last_debris = (float)nearest_debris;
		movement_distance = nearest_debris_distance;
		DEBUG(("movement:debris %i at %f:%f\n", nearest_debris, debris_position[nearest_debris][POS_X], debris_position[nearest_debris][POS_Y], debris_position[nearest_debris][POS_Z], nearest_debris_distance, nearest_debris_vector[SIDE_DIST]));
		DEBUG(("movement:next = [%f, %f, %f]\n", next[POS_X], next[POS_Y], next[POS_Z]));
		#endif

		api.setPositionTarget(next);
	} else {
		#ifdef DEBUG_ACTIVE
		movement_last_debris = -1.0f;
		nearest_debris_distance = 0.0f;
		DEBUG(("movement:Clear!\n"));
		#endif
		api.setPositionTarget(dst);
	}

	return false;
}
