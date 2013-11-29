int seconds;	//Passed time since the begining

//update()
bool blue_sphere;		//It is true if we are the blue sphere and false if we are the red one

ZRState our_state;		//Our ZRState

bool is_debris_collected[NUMBER_OF_DEBRIS];	//is_debris_collected[i] is true if someone has taken 'i' debris

float debris_position[NUMBER_OF_DEBRIS][3];	//It holds the position of the debris

bool is_item_collected[NUMBER_OF_ITEMS];	//is_debris_collected[i] is true if someone has taken 'i' item

float item_position[NUMBER_OF_ITEMS][3];	//It holds the position of the items

float our_comet_state[6];			//State of our comet

int laser_shots_left;			//Number of laser shots left
//update()

//init_update()
//Loads into global variables some data that doesn't change while the game is happening
void init_update() {
	int i;
	loop_update();

	//We check if we are the blue sphere
	blue_sphere = our_state[POS_X] > 0.0f;

	//We collect the debris locations
	for(i = 0; i < NUMBER_OF_DEBRIS; ++i) {
		game.getDebrisLocation(i, debris_position[i]);
		#ifdef DEBUG_ACTIVE
		DEBUG(("Debris %i at [%f, %f, %f]\n", i, debris_position[i][POS_X], debris_position[i][POS_Y], debris_position[i][POS_Z]));
		#endif
	}

	//We store the item positions
	//These positions are constant
	item_position[0][POS_X] = 0.50f;
	item_position[0][POS_Y] = 0.65f;
	item_position[0][POS_Z] = 0.0f;
	item_position[1][POS_X] = -0.50f;
	item_position[1][POS_Y] = 0.65f;
	item_position[1][POS_Z] = 0.0f;
}

void loop_update() {
	int i;

	//We get our state
	api.getMyZRState(our_state);

	//We check is some articular debris is collected
	for(i = 0; i < NUMBER_OF_DEBRIS; ++i) {
		is_debris_collected[i] = game.haveDebris(OUR, i) || game.haveDebris(THEIR, i);
	}

	//We check is some articular item is collected
	for(i = 0; i < NUMBER_OF_ITEMS; ++i) {
		is_item_collected[i] = game.haveItem(OUR, i) || game.haveItem(THEIR, i);
	}

	if(seconds >= 90) {
		//We get the comet state
		game.getCometState(OUR, our_comet_state);
	}

	//We ask for the number of laser shots left
	laser_shots_left = game.laserShotsRemaining();
}

//init
//Initializes variables in main and in other files
void init() {
  seconds = 0;

  #ifdef DEBUG_ACTIVE
  //Only if we are debugging, we initialize the varialbes in the debug file
  debug_init();
  #endif
  init_update();

  //We initialize variables in other files
  #ifdef DEBUG_ACTIVE
  movement_init();
  #endif
  phase1_init();
  phase2_init();
}

//loop
//Executed each seconds
void loop() {
	//We update vriables that changes on each step
	loop_update();
	#ifdef DEBUG_ACTIVE
	debug_update();
	#endif

	#ifdef BREAK_NET
	if(game.isNetBroken()) {
		game.breakNet();
	}
	#endif

	//Depending on the time we call phase1 or phase2
	if(seconds < 90) {
	phase1_loop();
	} else {
	phase2_loop();
	}

	//Self-explanatory
	++seconds;
}
