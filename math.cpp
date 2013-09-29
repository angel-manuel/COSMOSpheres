void mathVecScalarMult(float *c, float *a, float b, int n) {
	int i;
	for(i = 0; i < n; ++i) {
		c[i] = a[i] * b;
	}
}

void mathVecScalarDiv(float *c, float *a, float b, int n) {
	int i;
	for(i = 0; i < n; ++i) {
		c[i] = a[i] / b;
	}
}

void mathVecGetNormal(float *norm, float *v, int n) {
	mathVecScalarDiv(norm, v, mathVecMagnitude(v, n), n);
}

float absf(float val) {
	return (val > 0) ? val : -val;
}
