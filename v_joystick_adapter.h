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

#ifndef _V_JOYSTICK_ADAPTER_H_
#define _V_JOYSTICK_ADAPTER_H_

// ------------------------------------------------------------------- INCLUDES
#include <QThread>
#include <QString>
#include <QStringList>
// ------------------------------------------------------------------- SYNOPSIS
// ----------------------------------------------------------------------------

class VJoystickAdapter : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(VJoystickAdapter)

public:
    enum HatPosition {
        JOYSTICK_HAT_CENTERED,
        JOYSTICK_HAT_UP,
        JOYSTICK_HAT_UP_RIGHT,
        JOYSTICK_HAT_RIGHT,
        JOYSTICK_HAT_DOWN_RIGHT,
        JOYSTICK_HAT_DOWN,
        JOYSTICK_HAT_DOWN_LEFT,
        JOYSTICK_HAT_LEFT,
        JOYSTICK_HAT_UP_LEFT
    };

public:
    explicit VJoystickAdapter(QObject *parent = 0);
    virtual ~VJoystickAdapter();

    bool connect(int id);
    void disconnect();
    bool isConnected() const;

    int getJoystickId() const;
    QString getJoystickName() const;
    int getJoystickNumAxes() const;
    int getJoystickNumHats() const;
    int getJoystickNumBalls() const;
    int getJoystickNumButtons() const;

    static int getNumAvaliableJoystick();
    static QStringList getAvaliableJoystickName();

signals:
    void sigButtonChanged(int id, bool state);
    void sigAxisChanged(int id, int state);
    void sigHatCanged(int id, int state);
    void sigBallChanged(int id, int stateX, int stateY);

private:
    static HatPosition convertHatPosition(int position);

private:
    class VJoystickThread;

    struct d;
    d* m_d;
};

class VJoystickAdapter::VJoystickThread : public QThread
{
    Q_OBJECT

public:
    VJoystickThread(VJoystickAdapter* adapter) : m_adapter(adapter) {}

protected:
    virtual void run();

private:
    VJoystickAdapter* m_adapter;
};


#endif // _V_JOYSTICK_ADAPTER_H_
