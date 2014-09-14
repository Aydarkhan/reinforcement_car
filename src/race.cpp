#include <tuple>
#include <cstdlib>
#include <armadillo>
#include <car.h>
#include <track.h>

using namespace arma;

Car train_car(){
    Track monaco;
    Car ferrari;
    mat::fixed<2, 2> vector;
    vec::fixed<2> pos, vel;
    int action;
    double R;

    int n_trials = 1000, n_time_steps = 1000;

    for(int tr = 0; tr < n_trials; tr++){
        vector = monaco.setup(1);
        pos = vector.col(0);
        vel = vector.col(1);
        ferrari.reset();

        action = ferrari.choose_action(pos, vel, 0.0);

        for(int ts = 0; ts < n_time_steps; ts++){
            cout << track.status();
            std::tie (pos, vel, R) = monaco.move(action);
            action = ferrari.choose_action(pos, vel, R);

            if(monaco.is_finished())
                break;
            
        }
        
    }
    return ferrari;
}

vec::fixed<2> race(Car car){
    return vec();
}
