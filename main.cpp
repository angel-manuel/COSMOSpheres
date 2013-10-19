int seconds;	//Segundos de juego pasados
int phase;		//Fase de juego actual

//update()
bool blue_sphere;			//Indica si somos la SPHERE azul

ZRState our_state;		//El estado de nuestra SPHERE

bool is_debris_collected[NUMBER_OF_DEBRIS];	//Indica si alguien ha recogido cierta escombrera

float debris_position[NUMBER_OF_DEBRIS][3];	//Contiene las posiciones de las escombreras

bool is_item_collected[NUMBER_OF_ITEMS];	//Indica si alguien ha recogido cierto item

float item_position[NUMBER_OF_ITEMS][3];	//Contiene las posiciones de los items

float our_comet_state[6];			//Estado de nuestro cometa ({pos}{vel})

int laser_shots_left;			//Número de disparos láser disponibles

float mass;		//Masa actual de nuestro satélite
//update()

void init_update() {
	int i;
	loop_update();

	blue_sphere = our_state[POS_X] > 0.0f;

	for(i = 0; i < NUMBER_OF_DEBRIS; ++i) {
		game.getDebrisLocation(i, debris_position[i]);
	}

	item_position[0][POS_X] = 0.50f;
	item_position[0][POS_Y] = 0.65f;
	item_position[0][POS_Z] = 0.0f;
	item_position[1][POS_X] = -0.50f;
	item_position[1][POS_Y] = 0.65f;
	item_position[1][POS_Z] = 0.0f;

	#ifdef DEBUG_ACTIVE
	debug_track(0, &mass, (char*)"mass");
	#endif
}

void loop_update() {
	int i;

	api.getMyZRState(our_state);

	for(i = 0; i < NUMBER_OF_DEBRIS; ++i) {
		is_debris_collected[i] = game.haveDebris(OUR, i) || game.haveDebris(THEIR, i);
	}

	for(i = 0; i < NUMBER_OF_ITEMS; ++i) {
		is_item_collected[i] = game.haveItem(OUR, i) || game.haveItem(THEIR, i);
	}

	if(phase == 2) {
		game.getCometState(OUR, our_comet_state);
	}

	laser_shots_left = game.laserShotsRemaining();

	mass = game.getMass();
}

void init() {
  seconds = 0;
  phase = 1;

  #ifdef DEBUG_ACTIVE
  debug_init();
  #endif
  init_update();

  movement_init();
  phase1_init();
  phase2_init();
}

void loop() {
	loop_update();
	#ifdef DEBUG_ACTIVE
	debug_update();
	#endif

  if(seconds < 90) {
  	phase = 1;
    phase1_loop();
  } else {
  	phase = 2;
    phase2_loop();
  }

   ++seconds;
}
