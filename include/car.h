// Interface for Car


#ifndef CAR
#define CAR

#include <armadillo>

class Car
{
protected:

    const double maxVel; //  maximum velocity

    // Dimensionality
    const int spaceDim; 
    const int velDim; 

    const int nActions; 
    const int nNeurons;

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
    Car(int, int);
    virtual ~Car();
    virtual void reset();
    virtual int choose_action(arma::vec::fixed<2>, arma::vec::fixed<2>, double, bool learn = true) =0;

};

class SimpleCar : public Car{
    arma::mat weights; 
    arma::mat eligibility_trace;
public:
    SimpleCar();
    virtual ~SimpleCar();
    virtual int choose_action(arma::vec::fixed<2>, arma::vec::fixed<2>, double, bool learn = true);
    virtual void reset();
};

class OptimalCar : public Car{
    arma::cube weights; 
    arma::cube eligibility_trace;
public:
    OptimalCar();
    virtual ~OptimalCar();
    virtual int choose_action(arma::vec::fixed<2>, arma::vec::fixed<2>, double, bool learn = true);
    virtual void reset();
};

#endif // CAR
