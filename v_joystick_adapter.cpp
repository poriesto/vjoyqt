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

#include <SDL/SDL.h>

#include "v_joystick_adapter.h"

VJoystickAdapter::VJoystickAdapter(QObject *parent) :
    QObject(parent),
    m_joystick(0)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    SDL_JoystickEventState(SDL_ENABLE);
    m_joystickThread = new VJoystickThread(this);
}

VJoystickAdapter::~VJoystickAdapter()
{
    if(isConnected())
        close();
    delete m_joystickThread;
    SDL_JoystickEventState(SDL_DISABLE);
    SDL_Quit();
}

bool VJoystickAdapter::open(int id)
{
    if(SDL_JoystickOpened(id))
        return false;
    m_joystick = SDL_JoystickOpen(id);
    if(SDL_JoystickOpened(id))
    {
        m_joystickThread->start();
    }
    else
        m_joystick = 0;
    return m_joystick;
}

void VJoystickAdapter::close()
{
    if(m_joystick)
    {
        SDL_Event closeEvent;

        closeEvent.type = SDL_QUIT;
        SDL_PushEvent(&closeEvent);

        m_joystickThread->wait();
        SDL_JoystickClose(m_joystick);
        m_joystick = 0;
    }
}

void VJoystickAdapter::VJoystickThread::run()
{
    SDL_Event joyEvent;
    bool running = true;

    while(running)
    {
        SDL_WaitEvent(&joyEvent);

        if(joyEvent.type == SDL_QUIT)
            running = false;
        else if(joyEvent.jbutton.which == m_adapter->getJoystickId())
        {
            switch(joyEvent.type)
            {
            case SDL_JOYAXISMOTION:
                emit m_adapter->sigAxisChanged(joyEvent.jaxis.axis,joyEvent.jaxis.value);
                break;

            case SDL_JOYHATMOTION:
                emit m_adapter->sigHatCanged(joyEvent.jhat.hat, joyEvent.jhat.value);
                break;

            case SDL_JOYBALLMOTION:
                emit m_adapter->sigBallChanged(joyEvent.jball.ball, joyEvent.jball.xrel, joyEvent.jball.yrel);
                break;

            case SDL_JOYBUTTONDOWN:
            case SDL_JOYBUTTONUP:
                emit m_adapter->sigButtonChanged(joyEvent.jbutton.button, joyEvent.jbutton.state);
                break;
            }
        }
    }
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

    for(int i = 0; i < joyNum; ++i)
        joyNames.push_front(QString(SDL_JoystickName(i)));

    return joyNames;
}
