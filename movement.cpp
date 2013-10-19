#ifdef DEBUG_ACTIVE
float last_debris;
#endif

void movement_init() {
	#ifdef DEBUG_ACTIVE
	last_debris = -1.0f;
	debug_track(6, &last_debris, (char*)"last_debris");
	#endif
}

//movement_moveto
//dst -> Destino
//tvel -> Velocidad objetivo
//return -> true si ya se ha llegado, false en todos los demas casos
bool movement_moveto(float dst[3]) {
	float delta[3];
	float head[3];
	const float correction = (SPHERE_RADIUS + DEBRIS_RADIUS + 0.1f);

	mathVecSubtract(delta, dst, &our_state[POS], 3);		//delta = dst - pos
	
	if(mathVecMagnitude(delta, 3) < MAX_ITEM_START_DIST && mathVecMagnitude(&our_state[VEL], 3) < MAX_ITEM_START_VEL) {
		return true;
	}

	mathVecGetNormal(head, delta, 3);						//head = delta / |delta|

	int debris_number;
	float debris_vector[5];
	int nearest_debris = -1;
	float nearest_debris_distance = 1000000.0f;
	float* nearest_debris_vector;
	for(debris_number = 0; debris_number < NUMBER_OF_DEBRIS; debris_number++) {
		if(!is_debris_collected[debris_number]) {
			distanceToDebris(&our_state[POS], head, debris_position[debris_number], debris_vector);
			if(debris_vector[4] - correction < 0.0f) {
				//Colisión
				//DEBUG(("%i:[%f, %f, %f] at %f:%f\n", debris_number, debris_position[debris_number][POS_X], debris_position[debris_number][POS_Y], debris_position[debris_number][POS_Z], debris_vector[4], debris_vector[3]));
				if(debris_vector[3] < nearest_debris_distance && debris_vector[3] > 0.0f) {
					nearest_debris = debris_number;
					nearest_debris_vector = debris_vector;
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

		#ifdef DEBUG_ACTIVE
		last_debris = (float)nearest_debris;
		#endif

		DEBUG(("debris %i = [%f, %f, %f] at %f\n", nearest_debris, debris_position[nearest_debris][POS_X], debris_position[nearest_debris][POS_Y], debris_position[nearest_debris][POS_Z], nearest_debris_distance));
		DEBUG(("next = [%f, %f, %f]\n", next[POS_X], next[POS_Y], next[POS_Z]));
		api.setPositionTarget(next);
	} else {
		#ifdef DEBUG_ACTIVE
		last_debris = -1.0f;
		#endif
		DEBUG(("Clear!\n"));
		api.setPositionTarget(dst);
	}

	return false;
}
