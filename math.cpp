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

void mathVecCopy(float *dst, float *src, int n) {
	int i;
	for(i =0; i < n; ++i) {
		dst[i] = src[i];
	}
}

float absf(float val) {
	return (val > 0) ? val : -val;
}

float signf(float val) {
	if(val > 0.0f) {
		return 1.0f;
	} else if(val < 0.0f) {
		return -1.0f;
	} else {
		return 0.0f;
	}
}
