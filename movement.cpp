//movement_moveto
//dst -> Destino
//tvel -> Velocidad objetivo
//return -> true si ya se ha llegado, false en todos los demas casos
bool movement_moveto(float dst[3]) {
	float delta[3];
	float d;
	float v;
	float r;
	float zero[3] = {0.0f};

	mathVecSubtract(delta, dst, &our_state[POS], 3);
	d = mathVecMagnitude(delta, 3);
	v = mathVecMagnitude(&our_state[VEL], 3);
	r = mathVecMagnitude(&our_state[RATE], 3);

	if(d < MAX_ITEM_START_DIST) {
		if(v < MAX_ITEM_START_VEL && r < MAX_ITEM_START_RATE) {
			return true;
		} else {
			api.setPositionTarget(dst);
			api.setVelocityTarget(zero); //TODO: Mejorar para tvel != 0
			api.setAttRateTarget(zero);
			return false;
		}
	} else {
		//TODO: Sustituir setPositionTarget
		//iss5: Maniobrar entre las nubes
		api.setPositionTarget(dst);
		api.setVelocityTarget(zero);
		api.setAttRateTarget(zero);
		return false;
	}
}
