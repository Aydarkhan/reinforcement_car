#include <glibmm/main.h>
#include <cairomm/context.h>
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include "track.h"
#include "gui_gtk.h"
#include "race.h"


TrackArea::TrackArea()
    //: m_Dispatcher()
    : drawingThread(0)
{
    override_background_color(Gdk::RGBA("#FFFFFF"));
}
TrackArea::TrackArea(Track * monacoP) : TrackArea() {
    this->monacoP = monacoP;
}
TrackArea::~TrackArea() {}

void TrackArea::draw_last(){
    Glib::Threads::Mutex::Lock lock(mutexM);
    trajectory = monacoP->get_history();
    queue_draw();
    return;
}

void TrackArea::draw_race(){
    //do_draw_race();

    Glib::Threads::Mutex::Lock lock(mutexM);

    if (drawingThread)
    {
        std::cout << "Can't start a drawing thread while another one is running." << std::endl;
    }
    else
    {
        // Start a new worker thread.
        drawingThread = Glib::Threads::Thread::create(
                sigc::mem_fun(*this, &TrackArea::do_draw_race));
    }
    return;
}

void TrackArea::do_draw_race(){
    trajectory.clear();
    for(auto& pos: monacoP->get_history()){
        trajectory.push_back(pos);
        queue_draw();
        Glib::usleep(30000);
    }

    //drawingThread->join();
    drawingThread = 0;
    return;
}

//Override default signal handler:
bool TrackArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr){

    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    // coordinates for the center of the window
    int xc, yc;
    xc = width / 2;
    yc = height / 2;

    cr->set_line_width(log10(width));
    cr->set_source_rgb(0.1, 0.5, 0.5);

    double min, max;
    int side;
    std::function<double (double)> func;
    for(auto& c : monacoP->get_contours()){
        std::tie (min, max, func, side) = c;
        cr->move_to(min*width, func(min)*height);
        for(auto& x : arma::linspace<arma::vec>(min, max, 100))
            cr->line_to(x*width, func(x)*height);
        cr->stroke();
    }

    for(auto& w : monacoP->get_walls()){
        cr->move_to(w(0,0)*width, w(1,0)*height);
        cr->line_to(w(0,1)*width, w(1,1)*height);
        cr->stroke();
    }

    arma::mat::fixed<2, 2> w = monacoP->get_finishLine();
    cr->move_to(w(0,0)*width, w(1,0)*height);
    cr->line_to(w(0,1)*width, w(1,1)*height);
    cr->stroke();


    cr->set_line_width(log10(width)*2);
    cr->set_source_rgb(0.9, 0.7, 0.7);
    if(!trajectory.empty()){
        cr->move_to(trajectory[0](0)*width, trajectory[0](1)*height);
        for(auto& pos : trajectory)
            cr->line_to(pos(0)*width, pos(1)*height);
        cr->stroke();
    }

    return true;
}

CarButton::CarButton() {}
CarButton::CarButton(std::string label, Track * monacoP, Car * ferrariP) : 
    Gtk::Button(label),
    monacoP(monacoP), 
    ferrariP(ferrariP)
{}
CarButton::~CarButton() {}

void CarButton::on_button_clicked_train(){
    train_car(*ferrariP, *monacoP);
    return;
}
void CarButton::on_button_clicked_race(){
    race(*ferrariP, *monacoP);
    return;
}

