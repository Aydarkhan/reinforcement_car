// Interface for Car

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <armadillo>
#include "car.h"

using namespace arma;

Car::Car(int spacedim, int veldim) : 
    maxVel(0.3),
    nActions(9),
    spaceDim(spacedim),
    velDim(veldim),
    nNeurons(spaceDim * spaceDim + velDim * velDim),
    gSpace(init_grid(0.0, 1.0, spaceDim)),
    maxVelGrid(0.3),
    gVel(init_grid(-maxVelGrid, maxVelGrid, velDim)),
    spaceSigmaDenom(2 * pow(1.0 / (spaceDim - 1), 2)),
    velSigmaDenom(2 * pow(maxVelGrid / (velDim - 1) * 1.5 / 2, 2)),
    epsilon(0.2),
    eta(0.05),
    lambda(0.95),
    gamma(0.99),
    Q_last(0.0),
    epsilon_decay(1.0),
    time(0)
{
    std::cout << "Car constructor" << std::endl;
}


Car::~Car(){}

mat Car::init_grid(double start, double end, int dim){
    mat lin_vector = repmat(linspace<mat>(start, end, dim), 1, dim);
    mat res = join_rows(vectorise(lin_vector.t()), vectorise(lin_vector));
    return res;
}


void Car::reset(){
    time = 0;
    epsilon *= epsilon_decay;
    Q_last = 0.0;
    return;
}

SimpleCar::SimpleCar(): Car(31, 11),
    weights(nActions, nNeurons, fill::zeros),
    eligibility_trace(nActions, nNeurons, fill::zeros)
{}

OptimalCar::OptimalCar(): Car(11, 5),
    weights(spaceDim * spaceDim, velDim * velDim, nActions,  fill::zeros),
    eligibility_trace(spaceDim * spaceDim, velDim * velDim, nActions, fill::zeros)
{}

SimpleCar::~SimpleCar(){}
OptimalCar::~OptimalCar(){}

int SimpleCar::choose_action(vec::fixed<2> pos, vec::fixed<2> vel, double R, bool learn){
    vec space_r;
    vec vel_r;
    vec Q;
    uword action;

    space_r = exp(-(pow(pos(0) - gSpace.col(0), 2) + pow(pos(1) - gSpace.col(1), 2)) / spaceSigmaDenom);
    vel_r = exp(-(pow(pos(0) - gVel.col(0), 2) + pow(pos(1) - gVel.col(1), 2)) / velSigmaDenom);
    Q = weights * join_cols(space_r, vel_r);
    Q.max(action);

    if(learn){
        if(rand() % 100 < epsilon * 100){
            action = round(rand() % nActions);
        }

        weights += eta * (R - Q_last + gamma * Q(action)) * eligibility_trace;
        
        eligibility_trace *= lambda * gamma;
        eligibility_trace.row(action) +=  join_cols(space_r, vel_r).t();

        Q_last = Q(action);

    }

    time++;

    return (int) action;
}

void SimpleCar::reset(){
    Car::reset();
    eligibility_trace.zeros();
    return;
}

int OptimalCar::choose_action(vec::fixed<2> pos, vec::fixed<2> vel, double R, bool learn){
    vec space_r(spaceDim * spaceDim);
    vec vel_r(velDim * velDim);
    vec Q(nActions);
    mat r;
    uword action;

    space_r = exp(-(pow(pos(0) - gSpace.col(0), 2) + pow(pos(1) - gSpace.col(1), 2)) / spaceSigmaDenom);
    vel_r = exp(-(pow(pos(0) - gVel.col(0), 2) + pow(pos(1) - gVel.col(1), 2)) / velSigmaDenom);

    r = space_r * vel_r.t();
    for(int i = 0; i < nActions; i++){
        Q(i) = accu(weights.slice(i) % r);
    }
    Q.max(action);

    if(learn){

        if(R < 0.0 )
            R = -5.0;

        if(rand() % 100 < epsilon * 100){
            action = round(rand() % nActions);
        }

        weights += eta * (R - Q_last + gamma * Q(action)) * eligibility_trace;
        
        eligibility_trace *= lambda; // * gamma;
        eligibility_trace.slice(action) +=  r;

        Q_last = Q(action);

    }

    time++;


    return (int) action;
}

void OptimalCar::reset(){
    Car::reset();
    eligibility_trace.zeros();
    return;
}
