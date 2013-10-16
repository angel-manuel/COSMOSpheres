//Calcular la distancia del vector desde el punto inicial hasta la primera colisión con un debris.

//x0 = Punto de la Recta(posicion inicial)
//vd = Vector Director(debe ser unitario)
//p = Punto a comprobar(DEBRIS)
//retorno = vector hasta el Debris(el cuarto valor es la distancia)
void distanceToDebris(float x0[3], float vd[3], float p[3], float retorno[4]){
  float dist_XY, dist_YZ;
  //XY
  float nXY[2] = {-1.0f/vd[POS_Y], vd[POS_X]};
  dist_XY = mathVecInner(p, nXY, 2) - mathVecInner(x0, nXY, 2);
  //YZ
  float nYZ[2] = {-1.0f/vd[POS_Z], vd[POS_Y]};
  dist_YZ = mathVecInner(p+1, nYZ, 2) - mathVecInner(x0+1, nYZ, 2);

  mathVecScalarMult(retorno, nXY, dist_XY, 2);
  retorno[POS_Z] = nYZ[1] * dist_YZ;

  float tmp[3];
  mathVecSubtract(tmp, p, retorno, 3);
  mathVecSubtract(tmp, tmp, x0, 3);
  retorno[3] = mathVecMagnitude(tmp, 3);

  if(tmp[POS_X]*vd[POS_X] < 0 || tmp[POS_Y]*vd[POS_Y] < 0 || tmp[POS_Z]*vd[POS_Z] < 0) {
    retorno[3] = -retorno[3];
  }
}
