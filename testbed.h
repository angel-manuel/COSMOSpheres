//testbed.h
//NO INCLUIR EN EL PROYECTO
//Suple las funciones de ZR para que se pueda compilar en local

#include <stdio.h>
#include <math.h>

#define DEBUG((...)) printf(__VA_ARGS__)

//MATH
void mathVecAdd(float* c, float* a, float* b, int n) {
    int i;
    for(i = 0; i < n; ++i) {
        c[i] = a[i] + b[i];
    }
}

void mathVecSubtract(float* c, float* a, float* b, int n) {
    int i;
    for(i = 0; i < n; ++i) {
        c[i] = a[i] - b[i];
    }
}

float mathVecInner(float* a, float* b, int n) {
    float ret = 0.0f;
    int i;
    for(i = 0; i < n; ++i) {
        ret += a[i] * b[i];
    }
    return ret;
}

float mathVecMagnitude(float* v, int n) {
    float ret = 0.0f;
    int i;
    for(i = 0; i < n; ++i) {
        ret += v[i]*v[i];
    }
    return sqrtf(ret);
}

float mathVecNormalize(float* v, int n) {
    float mod = mathVecMagnitude(v, n);
    int i;
    for(i = 0; i < n; ++i) {
        v[i] /= mod;
    }
}
//END MATH