//Playing according to the rules is nice and all, but winning is better. And if they didn't want
//us to do it they could make the other strategies viable. Bullying is OP guys. 

//We will need this variables...
ZRState their_state;
bool netBroken;

//... and this functions.
//Activate this if the net is not broken. We will mimic the other sphere and if he tries to get debris we will break the net.
void bullyingWithTheNet()
{
	//To store the distance to nearest debris.
	int nearest_debris;
	float crash[3];

	//To store the diameters of the debris danger sphere.
	float cube[6];

	//If the net is broken return to main.
	if(netBroken)
		return;

	//Set velocity target to mimic them without breaking the net
	game.setVelocityTarget(&their_state[3]);

	//If we are about to crash break the net and avoid the debris. Simmetry of the debris field should make them crash if they aren't fast enough.
	//Sum position and velocity. 
	mathVecAdd(crash, &our_state[0], &our_state[3], 3);

	//If the sum is in the danger radius (0.18) break the net.
	nearest_debris = nearestDebris();
	for(int i = 0; i<3; i++)
	{	
		//First define the danger zone.
		cube[2*i]   = debris_position[nearest_debris][i] + 0.18;
		cube[2*i+1] = debris_position[nearest_debris][i] - 0.18;
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
void bullyingWithTheItem()
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
	if(enemy_item == -1 || seconds >= 75)
		return;

	//If they are, there we go!
	movement_moveto(item_position[i], false);
}

//And for the Grand Finale. If we didn't win yet we will try and push the other guy against his comet. 
//Why would they give us their comet state if it weren't for this?

void bullyingInPhase2()
{
	//I'm not sure as to when we should use this... I'll be changing this according to what I see on simulations.
	//First we'll just use it when they are near their comets trying to aim.

	//To save their comet state
	float their_Comet[6];
	game.getCometState(1, their_Comet);
	//Check if they are near their comet
	



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


