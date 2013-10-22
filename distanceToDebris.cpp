//Calcular la distancia del vector desde el punto inicial hasta la primera colisión con un debris.

//x0 = Punto de la Recta(posicion inicial)
//vd = Vector Director(debe ser unitario)
//p = Punto a comprobar(DEBRIS)
//ret = vector hasta el Debris(el cuarto valor es la distancia)
void distanceToDebris(float x1[3], float x2[3], float p[3], float ret[5]){
  float x1_p[3];
  float x2_x1[3];

  mathVecSubtract(x1_p, x1, p, 3);
  mathVecSubtract(x2_x1, x2, x1, 3);

  float d = mathVecMagnitude(x2_x1, 3);
  float cxs[3];
  float one[3] = {1.0f, 1.0f, 1.0f};
  mathVecCross(cxs, x1_p, x2_x1);
  float t = mathVecInner(cxs, one, 3)/(d*d);

  ret[POS_X] = (x1[POS_X]-p[POS_X])+(x2[POS_X]-x1[POS_X])*t;
  ret[POS_Y] = (x1[POS_Y]-p[POS_Y])+(x2[POS_Y]-x1[POS_Y])*t;
  ret[POS_Z] = (x1[POS_Z]-p[POS_Z])+(x2[POS_Z]-x1[POS_Z])*t;

  ret[3] = t;
  ret[4] = mathVecNormalize(ret, 3);

  /*
  #ifdef DEBUG_ACTIVE
  if(seconds == 0) {
  DEBUG(("x1 = "));debug_print_vector(x1, 3);
  DEBUG(("x2 = "));debug_print_vector(x2, 3);
  DEBUG(("p = "));debug_print_vector(p, 3);
  DEBUG(("ret = "));debug_print_vector(ret, 5);
  }
  #endif
  */
}
