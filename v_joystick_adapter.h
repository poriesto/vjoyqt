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

#ifndef __V_JOYSTICK_ADAPTER_H
#define __V_JOYSTICK_ADAPTER_H

#include <QThread>
#include <QString>
#include <QStringList>
#include <SDL/SDL_joystick.h>

class VJoystickAdapter : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(VJoystickAdapter)

public:
    enum HatPosition {
        JOYSTICK_HAT_CENTERED = SDL_HAT_CENTERED,
        JOYSTICK_HAT_UP = SDL_HAT_UP,
        JOYSTICK_HAT_UP_RIGHT = SDL_HAT_RIGHTUP,
        JOYSTICK_HAT_RIGHT = SDL_HAT_RIGHT,
        JOYSTICK_HAT_RIGHT_DOWN = SDL_HAT_RIGHTDOWN,
        JOYSTICK_HAT_DOWN = SDL_HAT_DOWN,
        JOYSTICK_HAT_DOWN_LEFT = SDL_HAT_LEFTDOWN,
        JOYSTICK_HAT_LEFT = SDL_HAT_LEFT,
        JOYSTICK_HAT_LEFT_UP = SDL_HAT_LEFTUP
    };

public:
    explicit VJoystickAdapter(QObject *parent = 0);
    ~VJoystickAdapter();

    bool open(int id);
    void close();
    bool isConnected() const {
        return m_joystick ? SDL_JoystickOpened(getJoystickId()) : false;
    }

    inline int getJoystickId() const {
        return SDL_JoystickIndex(m_joystick);
    }
    inline QString getJoystickName() const {
        return QString(SDL_JoystickName(getJoystickId()));
    }
    inline int getJoystickNumAxes() const {
        return SDL_JoystickNumAxes(m_joystick);
    }
    inline int getJoystickNumHats() const {
        return SDL_JoystickNumHats(m_joystick);
    }
    inline int getJoystickNumBalls() const {
        return SDL_JoystickNumBalls(m_joystick);
    }
    inline int getJoystickNumButtons() const {
        return SDL_JoystickNumButtons(m_joystick);
    }

    static int getNumAvaliableJoystick();
    static QStringList getAvaliableJoystickName();

signals:
    void sigButtonChanged(int id, bool state);
    void sigAxisChanged(int id, int state);
    void sigHatCanged(int id, int state);
    void sigBallChanged(int id, int stateX, int stateY);

private:
    class               VJoystickThread;

    SDL_Joystick*       m_joystick;
    VJoystickThread*    m_joystickThread;
};

class VJoystickAdapter::VJoystickThread : public QThread
{
    Q_OBJECT

public:
    inline VJoystickThread(VJoystickAdapter* adapter) : m_adapter(adapter) { }

protected:
    virtual void run();

private:
    VJoystickAdapter *m_adapter;
};

#endif // __V_JOYSTICK_ADAPTER_H
