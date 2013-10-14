//movement_moveto
//dst -> Destino
//tvel -> Velocidad objetivo
//return -> true si ya se ha llegado, false en todos los demas casos
bool movement_moveto(float dst[3]) {
	float delta[3];
	float head[3];

	mathVecSubtract(delta, dst, &our_state[POS], 3);		//delta = dst - pos
	
	if(mathVecMagnitude(delta, 3) < MAX_ITEM_START_DIST && mathVecMagnitude(&our_state[VEL]) < MAX_ITEM_START_VEL) {
		return true;
	}

	mathVecGetNormal(head, delta, 3);						//head = delta / |delta|

	int debris_number;
	float debris_distance[4];
	float debris_d;
	int nearest_debris = -1;
	float nearest_debris_distance = 1000000.0f;
	float* nearest_debris_vector;
	for(debris_number = 0; debris_number < NUMBER_OF_DEBRIS; debris_number++) {
		if(!is_debris_collected[debris_number]) {
			distanceToDebris(&our_state[POS], head, debris_position[debris_number], debris_distance);
			debris_d = mathVecMagnitude(debris_distance, 3) - (SPHERE_RADIUS + DEBRIS_RADIUS + 0.01f);
			if(debris_d < 0) {
				//Colisión
				if(debris_distance[3] < nearest_debris_distance && debris_distance[3] > 0.0f) {
					nearest_debris = debris_number;
					nearest_debris_vector = debris_distance;
					nearest_debris_distance = debris_distance[3];
				}
			}
		}
	}

	if(nearest_debris >= 0 && nearest_debris < NUMBER_OF_DEBRIS) { //Si nearest_debris es un debris válido
		float max_penetration_point[3];
		float tmp[3];
		float next[3];

		mathVecScalarMult(tmp, head, nearest_debris_distance, 3);
		mathVecAdd(max_penetration_point, &our_state[POS], tmp, 3);
		mathVecScalarMult(tmp, nearest_debris_vector, -nearest_debris_distance, 3); //Correcion a aplicar
		mathVecAdd(next, max_penetration_point, tmp, 3);

		api.setPositionTarget(next);
	} else {
		api.setPositionTarget(dst);
	}

	return false;
}

//movement_timeto
//dst -> Destino
//tvel -> Velocidad objetivo
//return -> Tiempo estimado de llegada
float movement_timeto(float dst[3], float tvel[3]) {
	float delta[3];
	float delta_vel[3];

	mathVecSubtract(delta, dst, &our_state[POS], 3);
	mathVecSubtract(delta_vel, tvel, &our_state[VEL], 3);

	return 0.0f;
}
