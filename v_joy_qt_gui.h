#ifndef __V_VIDEO_SERVER_
#define __V_VIDEO_SERVER_

#include <QMainWindow>

namespace Ui
{
class MainWindow;
}

class VJoyQtGUI : public QMainWindow
{
    Q_OBJECT
public:
    VJoyQtGUI(QWidget *parent = 0 );
    ~VJoyQtGUI();

private:
    Ui::MainWindow *ui;
};

#endif // __V_VIDEO_SERVER_
