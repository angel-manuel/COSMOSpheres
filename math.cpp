//Producto de un vector a, por el escalar b. Resultado vector c.
void mathVecScalarMult(float *c, float *a, float b, int n) {
	int i;
	for(i = 0; i < n; ++i) {
		c[i] = a[i] * b;
	}
}
//Para dismiuir el número de funciones, creo que esta es redundante.
void mathVecScalarDiv(float *c, float *a, float b, int n) {
	int i;
	for(i = 0; i < n; ++i) {
		c[i] = a[i] / b;
	}
}
//Para normalizar el vector v. Propongo un cambio tonto para solo usar la función mathVecScalarMult. Recuerdardtambién
//la función mathVecNormalize(float *v, n) que ya esta definida.
void mathVecGetNormal(float *norm, float *v, int n) {
	mathVecScalarDiv(norm, v, mathVecMagnitude(v, n), n);
	//mod= 1/mathVecMagnitude(v, n);
	//mathVecScalarMult(norm, v, mod, n);  
}
//Copiar vecotres. Entrada src, salida dst.
void mathVecCopy(float *dst, float *src, int n) {
	int i;
	for(i =0; i < n; ++i) {
		dst[i] = src[i];
	}
}
//Valor absoluto de un número.
float absf(float val) {
	return (val > 0) ? val : -val;
}
//Función signo.
float signf(float val) {
	if(val > 0.0f) {
		return 1.0f;
	} else if(val < 0.0f) {
		return -1.0f;
	} else {
		return 0.0f;
	}
}
