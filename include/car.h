// Interface for Car


#ifndef CAR
#define CAR

#include <vector>
#include <armadillo>

class Car
{

    const double maxVel; //  maximum velocity

    // Dimensionality
    const int spaceDim; 
    const int velDim; 

    const int nActions; 
    const int nNeurons;
    arma::mat weights; 
    arma::mat eligibility_trace;

    // Grid
    const arma::mat gSpace; // = array([(x, y) for x in linspace(0.0, 1.0, space_dim) for y in linspace(0.0, 1.0,\
    // space_dim)]).T
    const double maxVelGrid; //max_vel
    const arma::mat gVel;// = array([(x, y) for x in linspace(-max_vel_grid, max_vel_grid, vel_dim) for y in\
    // linspace(-max_vel_grid, max_vel_grid, vel_dim)]).T


    // Parameters
    const double spaceSigmaDenom;
    const double velSigmaDenom; 
    double epsilon;
    double eta;
    const double lambda;
    const double gamma;

    // Learning
    double Q_last; 
    const double epsilon_decay;
    int time;

    arma::mat init_grid(double, double, int);

public:
    Car();
    virtual ~Car();
    void reset();
    int choose_action(arma::vec::fixed<2>, arma::vec::fixed<2>, double, bool learn = true);

};

#endif // CAR
