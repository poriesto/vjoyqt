#include <QtGui/QApplication>
#include <v_joy_qt_gui.h>

int main( int argc, char *argv[] )
{
    QApplication a(argc, argv);
    VJoyQtGUI window;
    window.show();
    return a.exec();
}
