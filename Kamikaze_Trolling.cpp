//Those bastards won't know what hit them.

//To avoid being pushed out by kamikazes we will fall back between the debris to make them crash

//Calculate the difference between the relative position of the spheres and
//their velocity vector.
bool Kamikaze_Detecting()
{
	//Relative position of both spheres
	float relative_position [3];
	mathVecSubtract(relative_position, &our_state[0], &their_state[0], 3);
	mathVecNormalize(relative_position, 3);

	//Velocity of enemy sphere
	float enemy_velocity[3];
	enemy_velocity[0] = their_state[3];
	enemy_velocity[1] = their_state[4];
	enemy_velocity[2] = their_state[5];
	mathVecNormalize(enemy_velocity, 3);

	//We compare both
	float result[3];
	mathVecSubtract(result, enemy_velocity, relative_position,3);

	//If less than the correction factor, we are being attacked.
	return (mathVecMagnitude(result, 3) < delta);
}

//If under attack is time to act. 
void Kamikaze_Trolling()
{
	//To store the point to which we should move in case of attack.
	float position_target [3];
	position_target[0] = our_state[0];
	position_target[1] = our_state[1];
	position_target[2] = our_state[2];
	//Check if we are under attack
	Kamikaze[0] = Kamikaze_Detecting();

	//If we are under attack this loop we move a bit to check next loop.
	if(Kamikaze[0] && !(Kamikaze[1]))
	{	
		position_target[2] -= 0.1f;
		movement_moveto(position_target, true);
	}


	//If we have been under attack two loops or we have activated troll mode.
	else if((Kamikaze[0] && Kamikaze[1]) && (!(Kamikaze[2])))
	{
		//We save the position target in movingHere. It's near debris. 
		movingHere[0] = debris_position[nearestDebris()][0];
		movingHere[1] = debris_position[nearestDebris()][1];
		movingHere[2] = debris_position[nearestDebris()][2];
		movingHere[0] += ((our_state[0] > 0) ? 0.18f : -0.18f);
		movement_moveto(movingHere, false);
		//We activate troll mode.
		Kamikaze[2] = true;
	}

	else if(Kamikaze[2])
	{
		//While troll mode is active we keep on moving to the point
		//When we arrive there we get out of troll mode. We will fall back in
		//if they keep chasing us.
		Kamikaze[2] = (!(movement_moveto(movingHere, false)));
	}

	Kamikaze[1] = Kamikaze[0];

}

//Set our position to make them crash into debris when they come for us.
int nearestDebris()
{
	int nearest_debris = -1;
	float distance = 999999;
	float temp_dist [3];

	//Look for the closest debris to back to.
	for(int i = 0; i < NUMBER_OF_DEBRIS; i++)
	{
		if(!is_debris_collected[i])
		{
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



