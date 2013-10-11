//movement_moveto
//dst -> Destino
//tvel -> Velocidad objetivo
//return -> true si ya se ha llegado, false en todos los demas casos
bool movement_moveto(float dst[3], float tvel[3]) {
	float delta[3];
	float d;
	float head[3];

	float head_vel[3];
	float side_vel[3];
	float head_tvel[3];
	float side_tvel[3];

	mathVecSubtract(delta, dst, &our_state[POS], 3);		//delta = dst - pos
	d = mathVecMagnitude(delta, 3);							//d = |delta|

	mathVecGetNormal(head, delta, 3);						//head = delta / |delta|

	mathVecMult(head_vel, head, vel, 3);					//head_vel = head * vel
	mathVecSubtract(side_vel, vel, head_vel, 3);			//side_vel = vel - head_vel

	mathVecMult(head_tvel, head, tvel, 3);					//head_tvel = head * tvel
	mathVecSubtract(side_tvel, tvel, head_tvel, 3);			//side_tvel = tvel - head_tvel
}

//movement_timeto
//dst -> Destino
//tvel -> Velocidad objetivo
//return -> Tiempo estimado de llegada
float movement_timeto(float dst[3], float tvel[3]) {
	float delta[3];
	float delta_vel[3];

	mathVecSubtract(delta, dst, &our_state[POS], 3);
	mathVecSubtract(delta_vel, tvel, &our_state[VEL], 3);

	return 0.0f;
}
