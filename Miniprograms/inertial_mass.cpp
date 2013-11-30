//inertial_mass = 4.4f
#define IMPULSE_MAGNITUDE 0.02f

bool impulse;
bool print;

void init() {
	impulse = true;
	print = true;
}

void loop() {
	if(impulse) {
		float imp[3] = {0.0f, 0.0f, IMPULSE_MAGNITUDE};
		api.setForces(imp);

		impulse = false;
	} else if(print) {
		ZRState our_state;
		float speed;
		float inertial_mass;

		api.getMyZRState(our_state);
		speed = mathVecMagnitude(&our_state[3], 3);

		//i = delta_p
		//i = delta_v*mass
		//mass = i/delta_v
		inertial_mass = IMPULSE_MAGNITUDE/speed;

		DEBUG(("inertial_mass is %f\n", inertial_mass));

		print = false;
	}
}