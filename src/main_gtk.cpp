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

#include "helloworld_gtk.h"
#include <gtkmm/application.h>
#define  ARMA_DONT_USE_WRAPPER
#include <armadillo>
#include <iostream>
#include "race.h"
#include "car.h"
#include "track.h"

int main (int argc, char *argv[])
{
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    HelloWorld helloworld;
    
    std::cout << "Start main" << std::endl;

    OptimalCar ferrari;
    Track monaco;
    train_car(ferrari, monaco);
    race(ferrari, monaco);

    //Shows the window and returns when it is closed.
    return 0; //app->run(helloworld);
}
