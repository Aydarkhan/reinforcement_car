#include <iostream>
#include <tuple>
#include <armadillo>
#include "car.h"
#include "track.h"
#include "race.h"
//#include "gnuplot-iostream.h"


using namespace arma;

void train_car(Car & ferrari, Track & monaco){
    vec::fixed<2> pos, vel;
    int action;
    double R;

    int n_trials = 100, n_time_steps = 1000;

    std::cout << "Car training..." << std::endl;

    for(int tr = 0; tr < n_trials; tr++){
        std::tie (pos, vel) = monaco.setup(1);
        ferrari.reset();

        action = ferrari.choose_action(pos, vel, 0.0);
        std::cout << monaco.status() << endl;

        for(int ts = 0; ts < n_time_steps; ts++){
            std::tie (pos, vel, R) = monaco.move(action);
            action = ferrari.choose_action(pos, vel, R);

            if(monaco.is_finished()){
                std::cout << monaco.status() << std::endl;
                break;
            }
            
        }
        if(!monaco.is_finished())
            std::cout << "Not finished" << std::endl;
    }
    return;
}

void race(Car & ferrari, Track & monaco){
    vec::fixed<2> pos, vel;
    int action;
    double R;

    int n_time_steps = 1000;

    std::cout << "*****  GO ****** " << std::endl;

    std::tie (pos, vel) = monaco.setup(1);
    ferrari.reset();

    action = ferrari.choose_action(pos, vel, 0.0);
    std::cout << monaco.status() << endl;

    for(int ts = 0; ts < n_time_steps; ts++){
        std::tie (pos, vel, R) = monaco.move(action);
        action = ferrari.choose_action(pos, vel, R, false);

        if(monaco.is_finished()){
            std::cout << monaco.status() << endl;
            break;
        }

    }
    if(!monaco.is_finished())
        std::cout << "Not finished" << endl;


    cout << "The race:" << endl;
    
    //mat A = mat(monaco.get_history());

    //Gnuplot gp;
    //gp << "set xrange [-2:2]\nset yrange [-2:2]\n";
    //gp << "plot '-' w p title 'Car', sin(x*pi), sin(x*pi) - 0.3";
    //gp.send1d(monaco.get_history());

    for(auto& pos : monaco.get_history()){
       cout << pos.t();
       //std::vector<std::vector<double>> a = {{0, 0},{pos(0), pos(1)}};
       //gp.send1d(a);
    }

    return;
}
