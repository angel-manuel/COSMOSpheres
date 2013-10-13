//Calcular la distancia del vector desde el punto inicial hasta la primera colisión con un debris.

float vector1[2];
float vector2[2];
float distance[3];

//x0 = Punto de la Recta(posicion inicial)
//vd = Vector Director
//po = Punto a comprobar(DEBRIS)
//retorno = vector hasta el Debris(el cuarto valor es la distancia)
void distanceToDebris(float x0[3], float vd[3], float po[3], float retorno[4]){
  
  float vector1[2] = {-(1/vd[POS_Y]), (po[POS_X])};
  float vector2[2] = {-(1/vd[POS_Z]), (po[POS_Y])};
  
  /*
  distance = sqrt(vector1^2 + vector2^2);
  retorno[4] = mathVecMagnitude(distance, 3);
  */
  //Supongo que esto seria necesario luego :)
  
  retorno[POS_X] = distance[POX_X];
  retorno[POS_Y] = distance[POS_Y];
  retorno[POS_Z] = distance[POS_Z];
}
