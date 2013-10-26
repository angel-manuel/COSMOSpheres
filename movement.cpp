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
//dst -> Destino
//tvel -> Velocidad objetivo
//return -> true si ya se ha llegado, false en todos los demas casos
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

	//delta = head_vel
	mathVecScalarMult(delta, head, mathVecInner(head, &our_state[VEL], 3), 3);
	float head_speed = mathVecMagnitude(delta, 3);
	float side_vel[3];
	mathVecSubtract(side_vel, &our_state[VEL], delta, 3);

	const float danger_radius = (SPHERE_RADIUS + DEBRIS_RADIUS) + 0.03f;
	const float correction = danger_radius + 0.02f;

	if(direct || seconds >= 90 || !game.isNetBroken()) {
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
			distanceToDebris(&our_state[POS], dst, debris_position[debris_number], debris_vector);
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

	if(nearest_debris >= 0 && nearest_debris < NUMBER_OF_DEBRIS) { //Si nearest_debris es un debris válido
		//delta = tmp
		//debris_vector = next

		mathVecScalarMult(delta, nearest_debris_vector, correction, 3);
		mathVecAdd(debris_vector, debris_position[nearest_debris], delta, 3);

		#ifdef DEBUG_ACTIVE
		movement_last_debris = (float)nearest_debris;
		movement_distance = nearest_debris_distance;
		DEBUG(("movement:debris %i at %f:%f\n", nearest_debris, debris_position[nearest_debris][POS_X], debris_position[nearest_debris][POS_Y], debris_position[nearest_debris][POS_Z], nearest_debris_distance, nearest_debris_vector[SIDE_DIST]));
		DEBUG(("movement:next = [%f, %f, %f]\n", debris_vector[POS_X], debris_vector[POS_Y], debris_vector[POS_Z]));
		#endif

		api.setPositionTarget(debris_vector);
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
