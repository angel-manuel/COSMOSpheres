//distanceToDebris
//The following function calculates the point-line distance and it is used by the movement code

//x1 = Point of the line
//x2 = Another point of the line
//p = Point to trst against the line
//ret = MInimun vector between point and line, distance between the start of that vector and x and module of the vector
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

  ret[3] = t*d;
  ret[4] = mathVecNormalize(ret, 3);
}
