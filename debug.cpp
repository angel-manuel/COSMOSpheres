#define DEBUG_TRACKS 7 //Número de datos que se pueden seguir

char* debug_tracking_names[DEBUG_TRACKS];
float* debug_tracking_vars[DEBUG_TRACKS];

float debug[DEBUG_TRACKS];

void debug_init() {
	int i;

	for(i = 0; i < DEBUG_TRACKS; ++i) {
		debug_tracking_names[i] = NULL;
		debug_tracking_vars[i] = NULL;
		debug[i] = 0.0f;
	}
}

void debug_update() {
	int i;

	for(i = 0; i < DEBUG_TRACKS; ++i) {
		if(debug_tracking_vars[i] != NULL) {
			debug[i] = *(debug_tracking_vars[i]);
		} else {
			debug[i] = 0.0f;
		}
	}

	api.setDebug(debug);
}

//debug_track(track, var, name)
//Monitoriza una variable global
//track -> Pista a usar (0-6)
//var -> Puntero a la variable global
//name -> Nombre de la variable
//return -> true si se ha registrado el seguimiento correctamente y false en otro caso
bool debug_track(int track, float* var, char* name) {
	if(track >= 0 && track < DEBUG_TRACKS && var != NULL && debug_tracking_vars[track] == NULL) {
		debug_tracking_vars[track] = var;
		debug_tracking_names[track] = name;
		if(name != NULL) {
			DEBUG(("Debug track %i = %s\n", track, name));
		} else {
			DEBUG(("Debug track %i ???\n", track));
		}
		return true;
	}
	return false;
}
