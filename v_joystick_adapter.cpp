/*
 * Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
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

// ------------------------------------------------------------------- INCLUDES
#include <SDL/SDL.h>
#include <SDL/SDL_joystick.h>
// ------------------------------------------------------------------- SYNOPSIS
#include "v_joystick_adapter.h"
// ----------------------------------------------------------------------------

struct VJoystickAdapter::d
{
    SDL_Joystick* m_sdlJoystick;
    VJoystickAdapter::VJoystickThread* m_joystickThread;
};

// ----------------------------------------------------------------------------

VJoystickAdapter::VJoystickAdapter(QObject *parent) :
    QObject(parent),
    m_d(NULL)
{
    m_d = new d;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    SDL_JoystickEventState(SDL_ENABLE);
}

VJoystickAdapter::~VJoystickAdapter()
{
    if(isConnected()) {
        disconnect();
    }

    SDL_JoystickEventState(SDL_DISABLE);
    SDL_Quit();

    delete m_d;
    m_d = NULL;
}

bool VJoystickAdapter::connect(int id)
{
    if(SDL_JoystickOpened(id)) {
        return false;
    }
    m_d->m_sdlJoystick = SDL_JoystickOpen(id);

    if(SDL_JoystickOpened(id)) {
        m_d->m_joystickThread = new VJoystickThread(this);
        m_d->m_joystickThread->start();
    }
    else {
        m_d->m_sdlJoystick = NULL;
    }
    return m_d->m_sdlJoystick;
}

void VJoystickAdapter::disconnect()
{
    if(m_d->m_sdlJoystick)
    {
        SDL_Event closeEvent;

        closeEvent.type = SDL_QUIT;
        SDL_PushEvent(&closeEvent);
        m_d->m_joystickThread->wait();

        delete m_d->m_joystickThread;
        m_d->m_joystickThread = NULL;

        SDL_JoystickClose(m_d->m_sdlJoystick);
        m_d->m_sdlJoystick = 0;
    }
}

bool VJoystickAdapter::isConnected() const
{
    return m_d->m_sdlJoystick ? SDL_JoystickOpened(getJoystickId()) : false;
}

int VJoystickAdapter::getJoystickId() const
{
    return SDL_JoystickIndex(m_d->m_sdlJoystick);
}
QString VJoystickAdapter::getJoystickName() const
{
    return QString(SDL_JoystickName(getJoystickId()));
}

int VJoystickAdapter::getJoystickNumAxes() const
{
    return SDL_JoystickNumAxes(m_d->m_sdlJoystick);
}

int VJoystickAdapter::getJoystickNumHats() const
{
    return SDL_JoystickNumHats(m_d->m_sdlJoystick);
}

int VJoystickAdapter::getJoystickNumBalls() const
{
    return SDL_JoystickNumBalls(m_d->m_sdlJoystick);
}

int VJoystickAdapter::getJoystickNumButtons() const
{
    return SDL_JoystickNumButtons(m_d->m_sdlJoystick);
}

int VJoystickAdapter::getNumAvaliableJoystick()
{
    SDL_JoystickUpdate();
    return SDL_NumJoysticks();
}

QStringList VJoystickAdapter::getAvaliableJoystickName()
{
    QStringList joyNames;

    int joyNum = getNumAvaliableJoystick();

    for(int i = 0; i < joyNum; ++i) {
        joyNames.push_front(QString(SDL_JoystickName(i)));
    }

    return joyNames;
}

void VJoystickAdapter::VJoystickThread::run()
{
    SDL_Event joyEvent;
    bool running = true;

    int joystickId = m_adapter->getJoystickId();

    while(running) {
        SDL_WaitEvent(&joyEvent);

        if(joyEvent.type == SDL_QUIT) {
            running = false;
        }
        else if(joyEvent.jbutton.which == joystickId)
        {
            switch(joyEvent.type) {
            case SDL_JOYAXISMOTION:
                emit m_adapter->sigAxisChanged(joyEvent.jaxis.axis,joyEvent.jaxis.value);
                break;

            case SDL_JOYHATMOTION:
                emit m_adapter->sigHatCanged(joyEvent.jhat.hat, VJoystickAdapter::convertHatPosition(joyEvent.jhat.value));
                break;

            case SDL_JOYBALLMOTION:
                emit m_adapter->sigBallChanged(joyEvent.jball.ball, joyEvent.jball.xrel, joyEvent.jball.yrel);
                break;

            case SDL_JOYBUTTONDOWN:
            case SDL_JOYBUTTONUP:
                emit m_adapter->sigButtonChanged(joyEvent.jbutton.button, joyEvent.jbutton.state);
                break;

            default:
                break;
            }
        }
    }
}

VJoystickAdapter::HatPosition VJoystickAdapter::convertHatPosition(int position)
{
    switch(position) {
    case SDL_HAT_CENTERED:
        return JOYSTICK_HAT_CENTERED;
        break;

    case SDL_HAT_UP:
        return JOYSTICK_HAT_UP;
        break;

    case SDL_HAT_RIGHTUP:
        return JOYSTICK_HAT_UP_RIGHT;
        break;

    case SDL_HAT_RIGHT:
        return JOYSTICK_HAT_RIGHT;
        break;

    case SDL_HAT_RIGHTDOWN:
        return JOYSTICK_HAT_DOWN_RIGHT;
        break;

    case SDL_HAT_DOWN:
        return JOYSTICK_HAT_DOWN;
        break;
    case SDL_HAT_LEFTDOWN:
        return JOYSTICK_HAT_DOWN_LEFT;
        break;

    case SDL_HAT_LEFT:
        return JOYSTICK_HAT_LEFT;
        break;

    case SDL_HAT_LEFTUP:
        return JOYSTICK_HAT_UP_LEFT;
        break;

    default:
        return JOYSTICK_HAT_CENTERED;
        break;
    }
    return JOYSTICK_HAT_CENTERED;
}
