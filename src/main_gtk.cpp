//$Id: main.cc 836 2007-05-09 03:02:38Z jjongsma $ -*- c++ -*-

/* gtkmm example Copyright (C) 2002 gtkmm development team
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <gtkmm.h>
//#include "helloworld_gtk.h"
//#include <gtkmm/application.h>
//#include <gtkmm/button.h>
//#include <gtkmm/window.h>
#define  ARMA_DONT_USE_WRAPPER
#include <armadillo>
#include <iostream>
#include "race.h"
#include "car.h"
#include "track.h"
#include "gui_gtk.h"

int main (int argc, char *argv[])
{

    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

//    HelloWorld helloworld;
    

    
    std::cout << "Start main" << std::endl;

    OptimalCar ferrari;
    SimpleCar lada;
    Track monaco;
    //train_car(ferrari, monaco);
    //race(ferrari, monaco);

    Gtk::Window win;
    win.set_title("Race");
    win.set_border_width(10);

    Gtk::Box box(Gtk::ORIENTATION_VERTICAL, 20);
    box.set_homogeneous(false);

    Gtk::AspectFrame areaF;
    TrackArea area(&monaco, &ferrari);
    //Gtk::Allocation areaA( 0, 0, 800, 800 );   
    //area.size_allocate(areaA);


    Gtk::Box buttonsBcars;
    Gtk::ButtonBox buttonsB;
    buttonsB.set_layout(Gtk::BUTTONBOX_CENTER);

    Gtk::Button ladaB("Lada");
    ladaB.signal_clicked().connect(sigc::bind<Car *>(sigc::mem_fun(area, &TrackArea::changeCar), &lada));

    Gtk::Button startB("Train");
    startB.signal_clicked().connect(sigc::mem_fun(area, &TrackArea::train));

    Gtk::Button drawB("Race");
    drawB.signal_clicked().connect(sigc::mem_fun(area, &TrackArea::race));

    Gtk::Button ferrariB("Ferrari");
    ferrariB.signal_clicked().connect(sigc::bind<Car *>(sigc::mem_fun(area, &TrackArea::changeCar), &ferrari));

    buttonsBcars.pack_start(ferrariB);
    buttonsBcars.pack_start(buttonsB);
    buttonsBcars.pack_start(ladaB);
    buttonsB.pack_start(startB);
    buttonsB.pack_start(drawB);

    areaF.add(area);
    box.pack_start(areaF);
    box.pack_start(buttonsBcars, false, false);
    win.add(box);

    win.show_all_children();

    //Shows the window and returns when it is closed.
    return app->run(win);
}
