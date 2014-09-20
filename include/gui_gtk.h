#include <gtkmm.h>
#include <string>
#include "track.h"
#include "car.h"

class TrackArea : public Gtk::DrawingArea
{
    Track * monacoP;
    Car * ferrariP;
    std::vector<arma::vec::fixed<2>> trajectory;

    public:
        TrackArea();
        TrackArea(Track *, Car *);
        virtual ~TrackArea();
        void changeCar(Car *);
        void train();
        void race();

    protected:
        //Override default signal handler:
        Glib::Threads::Mutex mutexM;
        Glib::Dispatcher m_Dispatcher;
        Glib::Threads::Thread* drawingThread;
        virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
        void draw_race();
        void draw_train();
};

