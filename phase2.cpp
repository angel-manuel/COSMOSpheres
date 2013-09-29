void phase2_init() {
}

void phase2_loop() {
	phase2_attract();
}

void phase2_shoot() {

}

void phase2_attract() {
	float delta[DIMENSION];
	float t; //Tiempo que tardara en llegar a la altura de Y
	float a[DIMENSION];	//Aceleración que necesita el cometa
	float d[DIMENSION]; //Distancia a la que hay que estar del cometa
	int i;

	mathVecSubtract(delta, their_homebase, &our_comet_state[POS], DIMENSION); //delta = their_homebase - our_comet_pos
	t = delta[POS_Y]/our_comet_state[VEL_Y];

	a[POS_X] = (delta[POS_X] - our_comet_state[VEL_X]*t)/(2*t*t);
	a[POS_Y] = 0;
	a[POS_Z] = 0;

	for(i = 0; i < DIMENSION; ++i) {
		d[i] = signf(a[i])*MAX(sqrtf(mass/absf(a[i])), COMET_RADIUS + SPHERE_RADIUS);
	}

	DEBUG(("d[POS_X] = %f\n", d[POS_X]));

	float tvel[3];
	tvel[POS_X] = ((our_comet_state[POS_X] + d[POS_X]) - our_state[POS_X]) - our_comet_state[VEL_X];
	tvel[POS_Y] = our_comet_state[VEL_Y];
	tvel[POS_Z] = our_comet_state[VEL_Z];

	api.setVelocityTarget(tvel);
}

void phase2_predict(int dt, float initialState[6], float finalState[6]) {
	int step;
	float pos[3];
	float vel[3];
	float acel[3];

	mathVecCopy(pos, &initialState[POS], 3);
	mathVecCopy(vel, &initialState[VEL], 3);

	for(step = 0; step < dt; ++step) {
		phase2_comet_acceleration(pos, &our_state[POS], acel);
		mathVecAdd(vel, vel, acel, 3);
		mathVecAdd(pos, pos, vel, 3);
	}

	mathVecCopy(&finalState[POS], pos, 3);
	mathVecCopy(&finalState[VEL], vel, 3);
}

void phase2_comet_acceleration(float comet_pos[3], float sphere_pos[3], float acceleration[3]) {
	float delta[3];
	float d;
	float a;

	mathVecSubtract(delta, sphere_pos, comet_pos, 3);
	d = mathVecMagnitude(delta, 3);

	a = (1*mass)/(d*d);

	mathVecScalarMult(acceleration, delta, a/d, 3);
}
