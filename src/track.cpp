#include <forward_list>
#include <functional>
#include <string>
#include <iostream>
#include <tuple>
#include <cmath>
#include <armadillo>
#include "track.h"

using namespace arma;
//using namespace std;

Track::Track() :
    accelMap(make_accelMap()),
    xMax(1.0),
    yMax(1.0),
    xMin(0.0),
    yMin(0.0),
    vMax(0.3),
    accelFactor(0.1),
    tPenty(2),
    rPenty(-1.0),
    finishLine("0.8 1.; 0.1 0.25"),
    rMax(8.0),
    pos{0.3, 0.3},
    vel{0.0, 0.0},
    last_pos(pos),
    t_penty(0),
    total_reward(0.0),
    time(0),
    finished(false),
    message("Start the engine!")
{
    std::cout << "Track constructor" << std::endl;
    std::forward_list<std::tuple<double, double, std::function<double (double)>, int>>::iterator it_c = contours.before_begin();
    it_c = contours.insert_after(it_c, std::make_tuple(0.0, 1.0, [](double x){return sin(x*datum::pi);},1));
    it_c = contours.insert_after(it_c, std::make_tuple(0.1, 0.9, [](double x){return sin(x*datum::pi) - 0.3;},-1));

}

Track::~Track() {}


mat Track::make_accelMap(){
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
    //cout << accelFactor << endl;
    return a * 0.02; // accelFactor;
}

    //car_img = im.imread('carfig.jpg');
void Track::set_walls(){
    //walls;
    std::forward_list<mat::fixed<2, 2>>::iterator it = walls.before_begin();
    it = walls.insert_after(it, mat::fixed<2,2>("0.4 0.4; 0.64 0.8"));
    it = walls.insert_after(it, mat::fixed<2,2>("0.5 0.5; 0.85 1.0"));

    return;
} 

bool Track::crossed(mat::fixed<2, 2> wall, vec::fixed<2> lastpos, vec::fixed<2> pos){
    mat wall3(wall);
    wall3.resize(3,2);
    //wall3.row(2).zeros();
    vec lastpos3(lastpos);
    lastpos3.resize(3);
    //lastpos3(2) = 0;
    vec pos3(pos);
    pos3.resize(3);
    //pos3(2) = 0;

    bool cr = dot(cross(pos3 - lastpos3, wall3.col(0) - lastpos3),
              cross(pos3 - lastpos3, wall3.col(1) - lastpos3)) < 0 &&
              dot(cross(wall3.col(1) - wall3.col(0), pos3 - wall3.col(0)),
              cross(wall3.col(1) - wall3.col(0), lastpos3 - wall3.col(0))) < 0;
    return cr;
}

bool Track::wall_crash(vec::fixed<2> pos){
    bool bound = false, wall = false, contour = false;
    double min, max;
    int side;
    std::function<double (double)> func;

    bound = pos(0) <= xMin || pos(0) >= xMax ||
                 pos(1) <= yMin || pos(1) >= yMax;

    if (!bound)
        for(auto& w : walls){
            if(crossed(w, this->pos, pos)){
                wall = true;
                break;
            }
        }

    if (!bound && !wall){
        for(auto& c : contours){
            std::tie (min, max, func, side) = c;
            if(pos(0) >= min && pos(0) <= max && (pos(1) - func(pos(0)))*side >=0){
                contour = true;
                break;
            }
        }
    }
    return bound || wall || contour;
}

std::tuple<vec::fixed<2>, vec::fixed<2>> Track::setup(int level){
    pos = vec("0.05 0.03");
    vel = vec("0.0 0.0");

    last_pos = pos;
    history.clear();
    it_h = history.insert_after(history.before_begin(), pos);
    t_penty = 0.0;
    total_reward = 0.0;
    time = 0;
    finished = false;
    
    if(level != 0)
        set_walls();

    message = "Start the engine!";

    return std::make_tuple(pos, vel);
}

std::tuple<vec, vec, double> Track::move(int action){
    double rew;
    vec::fixed<2> new_pos;

    vel += accelMap.row(action).t();

    rew = 0.0;
    message = std::string("Time: ") + std::to_string(time); 

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
        message = std::string("Finish! Time: ") + std::to_string(time) + ", Reward: " + std::to_string(total_reward);
        finished = true;
    }

    last_pos = pos;
    pos = new_pos;
    total_reward += rew;
    time++;

    history.insert_after(it_h, pos);

    return std::make_tuple(pos, vel / vMax, rew);
}// Type ???

bool Track::is_finished(){
    return finished;
}

std::string Track::status(){
    return message;
}

