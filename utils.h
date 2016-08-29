#ifndef UTILS_H
#define UTILS_H
double vec_mag_sq(double x, double y);
double vec_mag(double x, double y);
double vec_arg(double x, double y);
void vec_norm(double *x, double *y);
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))
#endif
