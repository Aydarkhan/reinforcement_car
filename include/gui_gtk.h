#include <gtkmm.h>
#include <string>
#include "track.h"
#include "car.h"

class TrackArea : public Gtk::DrawingArea
{
    Track * monacoP;
    std::vector<arma::vec::fixed<2>> trajectory;

    public:
        TrackArea();
        TrackArea(Track *);
        virtual ~TrackArea();
        void draw_last();
        void draw_race();

    protected:
        //Override default signal handler:
        Glib::Threads::Mutex mutexM;
        Glib::Dispatcher m_Dispatcher;
        Glib::Threads::Thread* drawingThread;
        virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
        void do_draw_race();

};



class CarButton : public Gtk::Button
{
    Track * monacoP;
    Car * ferrariP;

    public:
        CarButton();
        CarButton(std::string, Track *, Car *);
        virtual ~CarButton();
        void on_button_clicked_train();
        void on_button_clicked_race();

};
