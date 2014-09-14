#ifndef RACE
#define RACE

#include <armadillo>
#include "car.h"


void train_car(Car &);
arma::vec::fixed<2> race(Car &);

#endif // RACE
