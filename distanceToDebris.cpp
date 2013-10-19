//Calcular la distancia del vector desde el punto inicial hasta la primera colisión con un debris.

//x0 = Punto de la Recta(posicion inicial)
//vd = Vector Director(debe ser unitario)
//p = Punto a comprobar(DEBRIS)
//ret = vector hasta el Debris(el cuarto valor es la distancia)
void distanceToDebris(float x0[3], float vd[3], float p[3], float ret[5]){
  float dist_XY, dist_YZ;
  //XY
  float nXY[2] = {-1.0f/vd[POS_Y], vd[POS_X]};
  mathVecNormalize(nXY, 2);
  dist_XY = mathVecInner(p, nXY, 2) - mathVecInner(x0, nXY, 2);
  //YZ
  float nYZ[2] = {-1.0f/vd[POS_Z], vd[POS_Y]};
  mathVecNormalize(nYZ, 2);
  dist_YZ = mathVecInner(p+1, nYZ, 2) - mathVecInner(x0+1, nYZ, 2);
  dist_YZ = -dist_XY*vd[POS_X]*vd[POS_Z]; 

  mathVecScalarMult(ret, nXY, dist_XY, 2);
  ret[POS_Z] = nYZ[1] * dist_YZ;

  float tmp[3];
  mathVecSubtract(tmp, p, ret, 3);
  mathVecSubtract(tmp, tmp, x0, 3);
  ret[3] = mathVecMagnitude(tmp, 3);

  if(tmp[POS_X]*vd[POS_X] < 0 || tmp[POS_Y]*vd[POS_Y] < 0 || tmp[POS_Z]*vd[POS_Z] < 0) {
    ret[3] = -ret[3];
  }
  ret[4] = mathVecMagnitude(ret, 3);
  mathVecNormalize(ret, 3);
  /*
  DEBUG(("x0 = [%f, %f, %f]\n", x0[POS_X], x0[POS_Y], x0[POS_Z]));
  DEBUG(("vd = [%f, %f, %f]\n", vd[POS_X], vd[POS_Y], vd[POS_Z]));
  DEBUG(("p = [%f, %f, %f]\n", p[POS_X], p[POS_Y], p[POS_Z]));
  DEBUG(("ret = [%f, %f, %f, %f, %f]\n", ret[0], ret[1], ret[2], ret[3], ret[4]));
  */
  //DEBUG(("p = [%f, %f, %f]\nd = %f\n", p[POS_X], p[POS_Y], p[POS_Z], ret[4]));
}
