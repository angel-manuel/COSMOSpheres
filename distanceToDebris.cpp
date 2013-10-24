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
  float t = -mathVecInner(x1_p, x2_x1, 3)/(d*d);

  float tmp[3];
  mathVecScalarMult(tmp, x2_x1, t, 3);
  mathVecAdd(ret, x1_p, tmp, 3);

  ret[3] = ABS(t*d);
  ret[4] = mathVecNormalize(ret, 3);
}
