//math
//This file supplies math function that dont belong to C or ZR

//mathVecScalarMult(c, a, b, n)
//c = a - b
//n -> Length of the vectors
void mathVecScalarMult(float *c, float *a, float b, int n) {
	int i;
	for(i = 0; i < n; ++i) {
		c[i] = a[i] * b;
	}
}
