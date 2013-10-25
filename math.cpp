//Producto de un vector a, por el escalar b. Resultado vector c.
void mathVecScalarMult(float *c, float *a, float b, int n) {
	int i;
	for(i = 0; i < n; ++i) {
		c[i] = a[i] * b;
	}
}
