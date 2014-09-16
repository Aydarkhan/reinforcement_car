// Interface for track


#ifndef TRACK
#define TRACK

#include <forward_list>
#include <vector>
#include <functional>
#include <string>
#include <tuple>
#include <armadillo>

class Track
{
    const double accelFactor; // acceleration per time step
    const arma::mat::fixed<9, 2> accelMap; // maps the action number to the correspondent acceleration vector

    // limits of the track
    const double xMax, yMax, xMin, yMin, vMax;

    arma::vec::fixed<2> pos; 
    arma::vec::fixed<2> vel; 
    arma::vec::fixed<2> last_pos; 
    std::vector<arma::vec::fixed<2>> history;
    std::forward_list<arma::vec::fixed<2>>::iterator it_h;

    const int tPenty; // time penalty for hitting a wall. the car stays still for this number of time steps.
    const double rPenty; // reward penalty for hitting a wall.
    const double rMax;
    int t_penty;
    double total_reward; 
    int time;  // time steps

    const arma::mat::fixed<2, 2> finishLine; 
    bool finished; 
    std::string message;

    std::forward_list<arma::mat::fixed<2, 2>> walls;
    std::forward_list<std::tuple<double, double, std::function<double (double)>, int>> contours;

    //car_img = im.imread('carfig.jpg');
    arma::mat make_accelMap();
    void set_walls();          // create the walls
    bool crossed(arma::mat::fixed<2, 2>, arma::vec::fixed<2>, arma::vec::fixed<2>);
    bool wall_crash(arma::vec::fixed<2>);

public:
    Track();
    virtual ~Track();
    std::tuple<arma::vec::fixed<2>, arma::vec::fixed<2>> setup(int);
    std::tuple<arma::vec, arma::vec, double> move(int); // Type ???
    bool is_finished();
    std::string status();
    std::vector<arma::vec::fixed<2>> get_history();

};

#endif // TRACK
