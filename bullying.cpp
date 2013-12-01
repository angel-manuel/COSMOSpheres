//Playing according to the rules is nice and all, but winning is better. And if they didn't want
//us to do it they could make the other strategies viable. Bullying is OP guys. 

//We will need this functions.
//Activate this if the net is not broken. We will mimic the other sphere and if he tries to get debris we will break the net.
void bullyingWithTheNet()
{
	//To store the distance to nearest debris.
	int nearest_debris;
	float crash[3];

	//To store the diameters of the debris danger sphere.
	float cube[6];
	float danger_radius = 0.18;

	//If the net is broken return to main.
	if(netBroken)
		return;

	//Set velocity target to mimic them without breaking the net
	api.setVelocityTarget(&their_state[3]);

	//If we are about to crash break the net and avoid the debris. Simmetry of the debris field should make them crash if they aren't fast enough.
		//Sum position and velocity. 
	mathVecAdd(crash, &our_state[0], &our_state[3], 3);

		//If the sum is in the danger radius (0.18) break the net.
	nearest_debris = nearestDebris();
	for(int i = 0; i<3; i++)
	{	
		//First define the danger zone.
		cube[2*i]   = debris_position[nearest_debris][i] + danger_radius;
		cube[2*i+1] = debris_position[nearest_debris][i] - danger_radius;
		//Look if you're in danger
		if (!(crash[i] < cube[2*i] && crash[i] > cube[2*i+1]))
			//If you aren't return to main
			return;
	}

	//Break the net!
	void breakNet();
}

//When the net breaks we will go to the item as usual, but when they try to get theirs we will push them out of bounds for a while.
//Then we get our item and just fire the laser. It's almost the same but they will have less fuel. And maybe they won't get their laser.
//PROFIT.
bool bullyingWithTheItem()
{
	float tempVec[3];
	float otherVec[3];
	int enemyItem = -1;
	//Is the enemy trying to grab any object?
	for(int i = 0; i<2; i++)
	{
		//Add their velocity and their position. If that looks like they are trying to get the item...
		mathVecAdd(otherVec, &their_state[0], &their_state[3], 3);
		mathVecSubtract(tempVec, otherVec, item_position[i], 3);
		if(mathVecMagnitude(tempVec, 3) < 0.05)
		{
			enemyItem = i; 
			break;
		}
	}

	//If not, return to main (or if we need to back to our item)
	if(enemyItem == -1 || seconds >= 75)
		return false;

	//If they are, there we go!
	movement_moveto(item_position[enemyItem], false);
	return true;
}

//And for the Grand Finale. If we didn't win yet we will try and push the other guy against his comet. 
//Why would they give us their comet state if it weren't for this?

bool bullyingInPhase2()
{
	//I'm not sure as to when we should use this... I'll be changing this according to what I see on simulations.
	//First we'll just use it when they are near their comets trying to aim.

	//To save their comet state
	float their_Comet[6];
	game.getCometState(1, their_Comet);

	//To save their state and compare it to the danger zone
	float crash[3];
	mathVecAdd(crash, &their_state[0], &their_state[3], 3);

	//Define the danger cube
	float cube[6];
	float danger_radius = 0.18;

	//Check if they are near their danger cube
	for(int i = 0; i<3; i++)
	{
		//Define the danger cube size
		cube[2*i] = their_Comet[i] + danger_radius;
		cube[2*i+1] = their_Comet[i] - danger_radius;

		//Check if they are near it
		if (!(crash[i] < cube[2*i] && crash[i] > cube[2*i+1]))
		{
			//If they aren't return to main
			return false;
		}
	}
	
	//They are near their comet, there we go!
	movement_moveto(&their_state[0], true);
	return true;
}

//Extra One! If they are near the OutOfBounds zone we should push them out just a bit don't you think?
//There we go!
bool getThemOutOfBounds()
{
	//If they are already out there is nothing to do here, we don't want to push them too much
	if(outOfBounds(&their_state[0]))
		return false;

	//Save their state
	float almostOut[3];
	float margin [3] = {0.1f, 0.1f, 0.1f};

	//Add their position and speed. Add a margin.
	mathVecAdd(almostOut, &their_state[0], &their_state[3], 3);
	mathVecAdd(almostOut, almostOut, margin, 3);

	//Check if they are almost out
	if (outOfBounds(almostOut))
	{
		movement_moveto(&their_state[0], true);
		return true;
	}

	return false;
}

//Look for the nearest debris
int nearestDebris()
{
	int nearest_debris = -1;
	float distance = 999999;
	float temp_dist [3];

	//Look for the closest debris to back to.
	for(int i = 0; i < NUMBER_OF_DEBRIS; i++)
	{
		//If it's still there
		if(!is_debris_collected[i])
		{
			//Check if it's the nearest one.
			mathVecSubtract(temp_dist, &our_state[0], debris_position[i], 3);
			if(mathVecMagnitude(temp_dist, 3)<distance)
			{
				nearest_debris = i;
				distance = mathVecMagnitude(temp_dist, 3);
			}
		}
	}
	return nearest_debris;
}

//Checks if a vector is out of bounds.
bool outOfBounds(float objective[3])
{
	//Save the game field
	float gameField[6];

   for(int i = 0; i<3; i++)
   {
   	gameField[2*i]  = (i==0 ? 0.64 : ((i==1) ? 0.80 : 0.64f));
   	gameField[2*i+1]= (i==0 ? -0.64 : ((i==1) ? -0.80 : -0.64f));

   	//Check if objective (x, y , z) is outside the game field
   	if((objective[i] > gameField[2*i]) || (objective[i] < gameField[2*i+1]))
   		return true;
   }

   return false;

}
