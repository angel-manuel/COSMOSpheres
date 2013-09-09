#define DEBUG_TRACKS 7 //Número de datos que se pueden seguir

char* tracking_names[DEBUG_TRACKS];
float* tracking_vars[DEBUG_TRACKS];

float debug[DEBUG_TRACKS];

void debug_init() {
	int i;

	for(i = 0; i < DEBUG_TRACKS; ++i) {
		tracking_names[i] = NULL;
		tracking_vars[i] = NULL;
		debug[i] = 0.0f;
	}
}

void debug_update() {
	int i;

	for(i = 0; i < DEBUG_TRACKS; ++i) {
		if(tracking_vars[i] != NULL) {
			debug[i] = *(tracking_vars[i]);
		} else {
			debug[i] = 0.0f;
		}
	}

	api.setDebug(debug);
}

bool debug_track(int track, float* var, char* name) {
	if(track >= 0 && track < DEBUG_TRACKS && var != NULL && tracking_vars[track] == NULL) {
		tracking_vars[track] = var;
		tracking_names[track] = name;
		if(name != NULL) {
			DEBUG(("Debug track %i = %s\n", track, name));
		} else {
			DEBUG(("Debug track %i ???\n", track));
		}
		return true;
	}
	return false;
}