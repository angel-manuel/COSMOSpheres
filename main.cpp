int seconds;	//Segundos de juego pasados
int phase;		//Fase de juego actual

//update()
bool blue_sphere;			//Indica si somos la SPHERE azul

ZRState our_state;		//El estado de nuestra SPHERE
ZRState their_state;	//El estado de su SPHERE

bool is_net_broken;		//Indica si la red está rota

bool is_debris_collected[10];	//Indica si alguien ha recogido cierta escombrera
bool is_debris_ours[10];			//Indica si nosotros hemos recogido cierta escombrera
bool is_debris_theirs[10];		//Indica si ellos han recogido cierta escombrera

float debris_position[NUMBER_OF_DEBRIS*3];	//Contiene las posiciones de las escombreras

bool is_item_collected[NUMBER_OF_ITEMS];	//Indica si alguien ha recogido cierto item
bool is_item_ours[NUMBER_OF_ITEMS];				//Indica si nosotros hemos recogido cierto item
bool is_item_theirs[NUMBER_OF_ITEMS];			//Indica si ellos han recogido cierto item

float item_position[NUMBER_OF_ITEMS*3];	//Contiene las posiciones de los items

float our_comet_state[6];			//Estado de nuestro cometa ({pos}{vel})
float their_comet_state[6];		//Estado de su cometa ({pos}{vel})

int laser_shots_left;			//Número de disparos láser disponibles
//TODO: Estimar disparos restantes del otro satélite

float our_homebase[3];		//Posición de nuestro hogar
float their_homebase[3];	//Posición de su hogar

float mass;		//Masa actual de nuestro satélite
//TODO: Estimar masa del otro satélite

float fuel;		//Combustible restante de nuestro satélite
//TODO: Estimar combustible restante del otro satelite

float our_score;		//Nuestra puntuación actual (para debug)
float their_score;	//Su puntuación actual (para debug)

bool collision;		//Indica si estamos chocando con el otro SPHERES
bool bounce;			//Indica si estamos chocando con el cometa o una escombrera

unsigned char last_message;	//Contiene el último mensaje recibido de la otra SPHERE
//update()

void init_update() {
	int i;
	loop_update();

	blue_sphere = our_state[POS_X] > 0.0f;

	our_homebase[POS_X] = (blue_sphere) ? 0.64f : -0.64f;
	our_homebase[POS_Y] = -0.8f;
	our_homebase[POS_Z] = (blue_sphere) ? 0.64f : -0.64f;

	their_homebase[POS_X] = (blue_sphere) ? -0.64f : 0.64f;
	their_homebase[POS_Y] = -0.8f;
	their_homebase[POS_Z] = (blue_sphere) ? -0.64f : 0.64f;

	for(i = 0; i < NUMBER_OF_DEBRIS; ++i) {
		game.getDebrisLocation(i, &(debris_position[i*3]));
	}

	item_position[0] = 0.50f;
	item_position[1] = 0.65f;
	item_position[2] = 0.0f;
	item_position[3] = -0.50f;
	item_position[4] = 0.65f;
	item_position[5] = 0.0f;

	our_comet_state[POS_X] = their_comet_state[POS_X] = 0.0f;
	our_comet_state[POS_Y] = their_comet_state[POS_Y] = 0.8f;
	our_comet_state[POS_Z] = their_comet_state[POS_Z] = 0.0f;

	our_comet_state[VEL_X] = (blue_sphere) ? 0.0568181818f : -0.0568181818f;
	their_comet_state[VEL_X] = (blue_sphere) ? -0.0568181818f : 0.0568181818f;
	our_comet_state[VEL_Y] = their_comet_state[VEL_Y] = -0.1420454545f;
	our_comet_state[VEL_Z] = (blue_sphere) ? 0.0568181818f : -0.0568181818f;
	their_comet_state[VEL_Z] = (blue_sphere) ? +0.0568181818f : 0.0568181818f;

	debug_track(0, &fuel, (char*)"fuel");
	debug_track(1, &mass, (char*)"mass");
}

void loop_update() {
	int i;

	api.getMyZRState(our_state);
	api.getOtherZRState(their_state);

	is_net_broken = game.isNetBroken();

	for(i = 0; i < NUMBER_OF_DEBRIS; ++i) {
		is_debris_ours[i] = game.haveDebris(OUR, i);
		is_debris_theirs[i] = game.haveDebris(THEIR, i);
		is_debris_collected[i] = is_debris_ours[i] || is_debris_theirs[i];
	}

	for(i = 0; i < NUMBER_OF_ITEMS; ++i) {
		is_item_ours[i] = game.haveItem(OUR, i);
		is_item_theirs[i] = game.haveItem(THEIR, i);
		is_item_collected[i] = is_item_ours[i] || is_item_theirs[i];
	}

	if(phase == 2) {
		game.getCometState(OUR, our_comet_state);
		game.getCometState(THEIR, their_comet_state);
	}

	laser_shots_left = game.laserShotsRemaining();

	mass = game.getMass();

	fuel = game.getFuelRemaining();

	our_score = game.getScore();
	their_score = game.getOtherScore();

	collision = game.wasCollisionActive();
	bounce = game.wasBounceActive();

	last_message = game.receiveMessage();
}

void init() {
  seconds = 0;
  phase = 1;
  debug_init();
	init_update();

  phase1_init();
  phase2_init();
}

void loop() {
	loop_update();
	debug_update();

  if(seconds < 90) {
  	phase = 1;
    phase1_loop();
  } else {
  	phase = 2;
    phase2_loop();
  }

   ++seconds;
}
