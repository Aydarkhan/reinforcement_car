// Interface for Car

#include <vector>
#include <cmath>
#include <cstdlib>
#include <armadillo>
#include "car.h"

using namespace arma;

Car::Car() : 
    maxVel(0.3),
    spaceDim(31),
    velDim(11),
    nActions(9),
    nNeurons(spaceDim * spaceDim + velDim * velDim),
    gSpace(init_grid(0.0, 1.0, spaceDim)),
    gVel(init_grid(-maxVelGrid, maxVelGrid, velDim)),
    maxVelGrid(1.0),
    spaceSigmaDenom(2 * pow(1.0 / (spaceDim - 1), 2)),
    velSigmaDenom(2 * pow(maxVelGrid / (velDim - 1) * 2, 2)),
    epsilon(0.01),
    eta(0.005),
    lambda(0.95),
    gamma(0.95),
    Q_last(0.0),
    epsilon_decay(1.0),
    time(0),
    weights(nActions, nNeurons, fill::zeros),
    eligibility_trace(nActions, nNeurons, fill::zeros)
{}


Car::~Car(){}

mat Car::init_grid(double start, double end, int dim){
    mat lin_vector = repmat(linspace<mat>(start, end, dim), 1, dim);
    return join_cols(vectorise(lin_vector.t()), vectorise(lin_vector));
}


void Car::reset(){
    time = 0;
    epsilon *= epsilon_decay;
    Q_last = 0.0;
    eligibility_trace.zeros();
    return;
}


int Car::choose_action(vec::fixed<2> pos, vec::fixed<2> vel, double R, bool learn){
    vec space_r;
    vec vel_r;
    vec Q;
    uword action;

    space_r = exp(-(pow(pos(0) - gSpace.col(0), 2) + pow(pos(1) - gSpace.col(1), 2)) / spaceSigmaDenom);
    vel_r = exp(-(pow(pos(0) - gVel.col(0), 2) + pow(pos(1) - gVel.col(1), 2)) / velSigmaDenom);
    Q = weights * join_cols(space_r, vel_r);
    Q.max(action);

    if(learn){
        if(rand() < epsilon)
            action = round(rand() * nActions);

        weights += eta * (R - Q_last + gamma + Q(action)) * eligibility_trace;
        
        eligibility_trace *= lambda * gamma;
        eligibility_trace.row(action) +=  join_cols(space_r, vel_r);

        Q_last = Q(action);

    }

    time++;

    return (int) action;
}



