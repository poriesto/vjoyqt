/*
 *  Copyright (c) 2011 Dmitry Suvorov <D.Suvorov90@gmail.com>
 */

#include <v_joy_qt_gui.h>
#include <ui_v_joy_qt_gui.h>

VJoyQtGUI::VJoyQtGUI(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

VJoyQtGUI::~VJoyQtGUI()
{
    delete ui;
}
