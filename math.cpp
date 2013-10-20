//Producto de un vector a, por el escalar b. Resultado vector c.
void mathVecScalarMult(float *c, float *a, float b, int n) {
	int i;
	for(i = 0; i < n; ++i) {
		c[i] = a[i] * b;
	}
}
//Para normalizar el vector v. Propongo un cambio tonto para solo usar la función mathVecScalarMult. Recuerdardtambién
//la función mathVecNormalize(float *v, n) que ya esta definida.
void mathVecGetNormal(float *norm, float *v, int n) {
	mathVecScalarMult(norm, v, 1.0f/mathVecMagnitude(v, n), n);
}
