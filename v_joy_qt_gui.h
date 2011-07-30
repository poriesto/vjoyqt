/*
 * The MIT License
 *
 * Copyright (c) 2011 Evgeny Proydakov lord.tiran@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef __V_VIDEO_SERVER_
#define __V_VIDEO_SERVER_

#include <QMainWindow>
#include <QKeyEvent>
#include <QVector>

namespace Ui
{
class MainWindow;
};

class VJoystickAdapter;

class VJoyQtGUI : public QMainWindow
{
    Q_OBJECT

public:
    VJoyQtGUI(QWidget *parent = 0 );
    ~VJoyQtGUI();

private slots:
    void connectToDevices();
    void disconnectFromDevices();

    void scanDevices();

    void slotButtonChanged(int id, bool state);
    void slotAxisChanged(int id, int state);
    void slotHatCanged(int id, int state);
    void slotBallChanged(int id, int stateX, int stateY);

private:
    void keyPressEvent(QKeyEvent *event);

    void setJoystickInfoVisible(bool visible);
    void setDeviceControlVisible(bool visible);

    void setJoystickInfo();
    void clearJoystickInfo();

private:
    Ui::MainWindow *ui;
    VJoystickAdapter *m_adapter;
    QVector<bool> m_visibleButtons;
};

#endif // __V_VIDEO_SERVER_
