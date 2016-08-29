#include <math.h>

double vec_mag_sq(double x, double y)
{
    return x * x + y * y;
}

double vec_mag(double x, double y)
{
    return sqrt(vec_mag_sq(x, y));
}

double vec_arg(double x, double y)
{
    double arg = atan(y/x) * 180 / M_PI;

    if (x < 0)
        return 180 + arg;
    return arg;
}

void vec_norm(double *x, double *y)
{
    double mag = vec_mag(*x, *y);

    *x /= mag;
    *y /= mag;
}
