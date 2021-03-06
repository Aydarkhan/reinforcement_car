// Interface for Car


#ifndef CAR
#define CAR

#include <string>
#include <armadillo>

class Car
{
protected:

    const std::string name;
    // Dimensionality
    const int spaceDim; 
    const int velDim; 

    const int nActions; 
    const int nNeurons;

    // Grid
    const arma::mat gSpace; 
    const arma::mat gVel;

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

    //Racing
    arma::vec velPrev;
    bool stuck;

    arma::mat init_grid(double, double, int);


public:
    Car(int, int, std::string);
    virtual ~Car();
    virtual void reset();
    virtual int choose_action(arma::vec::fixed<2>, arma::vec::fixed<2>, double, bool learn = true) =0;
    std::string get_name();
    

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
