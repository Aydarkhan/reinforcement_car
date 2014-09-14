#include <forward_list>
#include <string>
#include <tuple>
#include <armadillo>
#include <track.h>

using namespace arma;
//using namespace std;

Track::Track() :
    accelMap(make_accelMap()),
    xMax(1.0),
    yMax(1.0),
    xMin(0.0),
    yMin(0.0),
    vMax(0.3),
    accelFactor(0.02),
    tPenty(2),
    rPenty(-1.0),
    finishLine("0.8 0.1; 1. 0.25"),
    rMax(8.0),
    pos{0.3, 0.3},
    vel{0.0, 0.0},
    last_pos(pos),
    history{pos},
    t_penty(0),
    total_reward(0.0),
    time(0),
    finished(false),
    message("Start the engine!")
{}

Track::~Track() {}


mat make_accelMap(){
    mat::fixed<9, 2> a;
    a << 0. << 0. << endr
      << 0. << 1. << endr
      << 1/sqrt(2) << 1/sqrt(2) << endr
      << 1. << 0. << endr
      << 1/sqrt(2) << -1/sqrt(2) << endr
      << 0. << -1. << endr
      << -1/sqrt(2) << -1/sqrt(2) << endr
      << -1. << 0. << endr
      << -1/sqrt(2) << 1/sqrt(2) << endr;
    return a;
}

    //car_img = im.imread('carfig.jpg');
void Track::set_walls(){
    //walls;
    std::forward_list<mat::fixed<2, 2>>::iterator it = walls.before_begin();
    it = walls.insert_after(it, mat("0.4 0.4; 0.64 0.8"));
    it = walls.insert_after(it, mat("0.5 0.5; 0.85 1.0"));
    return;
} 

bool Track::crossed(mat::fixed<2, 2> wall, vec::fixed<2> lastpos, vec::fixed<2> pos){
    bool cr = dot(cross(pos - lastpos, wall.col(0) - lastpos),
              cross(pos - lastpos, wall.col(1) - lastpos)) < 0 &&
              dot(cross(wall.col(1) - wall.col(0), pos - wall.col(0)),
              cross(wall.col(1) - wall.col(0), lastpos - wall.col(0))) < 0;
    return cr;
}

bool Track::wall_crash(vec::fixed<2> pos){
    bool bound = false, wall = false, contour = false;

    bound = pos(0) <= xMin || pos(0) >= xMax ||
                 pos(1) <= yMin || pos(1) >= yMax;

    if (!bound){
        for(auto& w : walls)
            wall = wall | crossed(w, this->pos, pos);
    }
    if (!wall){
    }
    return bound & wall & contour;
}

mat::fixed<2, 2> Track::setup(int level){
    pos = {0.05, 0.03};
    vel = {0.0, 0.0};

    last_pos = pos;
    history.insert_after(history.end(), pos);
    t_penty = 0.0;
    total_reward = 0.0;
    time = 0;
    finished = false;
    
    if(level != 0)
        set_walls();

    return join_cols(pos, vel);
}

std::tuple<vec, vec, double> Track::move(int action){
    double rew;
    vec::fixed<2> new_pos;

    vel += accelMap.row(action);

    rew = 0.0;
    message = "Time: "; // TODO add time

    if(t_penty > 0){
        vel.zeros();
        t_penty--;
    }

    // impose maximum velocities
    for(auto& v : vel){
        v = (v > vMax) ? vMax : v;
        v = (v < -vMax) ? -vMax : v;
    }
    
    new_pos = pos + vel;
    if(wall_crash(new_pos)){
        t_penty = tPenty;
        rew = rPenty;

        new_pos = pos;
        vel.zeros();

        message = "Crashed!";
    }

    if(crossed(finishLine, pos, new_pos)){
        rew = rMax;
        message = "Finish!";
        finished = true;
    }

    last_pos = pos;
    pos = new_pos;
    total_reward += rew;
    time++;

    history.insert_after(history.end(), pos);

    return std::make_tuple(pos, vel / vMax, rew);
}// Type ???

bool Track::is_finished(){
    return finished;
}

std::string status(){
    return message;
}

