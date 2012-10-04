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
// ------------------------------------------------------------------- SYNOPSIS
#include <v_joystick_adapter.h>

#include <v_joystick_about_dialog.h>

#include <v_joystick_main_window.h>
#include <ui_v_joystick_main_window.h>
// ----------------------------------------------------------------------------

VJoystickMainWindow::VJoystickMainWindow(QWidget *parent) :
    QMainWindow(parent), m_ui(new Ui::VJoystickMainWindow), m_adapter(0)
{
    m_ui->setupUi(this);

    connect(m_ui->pushButtonScan, SIGNAL(clicked()), this, SLOT(scanDevices()));
    connect(m_ui->pushButtonConnect, SIGNAL(clicked()), this, SLOT(connectToDevices()));
    connect(m_ui->pushButtonDisconnect, SIGNAL(clicked()), this, SLOT(disconnectFromDevices()));

    connect(m_ui->actionScan, SIGNAL(triggered()), this, SLOT(scanDevices()));
    connect(m_ui->actionConnect, SIGNAL(triggered()), this, SLOT(connectToDevices()));
    connect(m_ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(disconnectFromDevices()));

    connect(m_ui->actionExit,  SIGNAL(triggered()), this, SLOT(appExit()));
    connect(m_ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));

    m_ui->comboBoxSelectJoy->setDisabled(true);
    setJoystickInfoVisible(false);
    setDeviceControlVisible(false);

    m_ui->ballWidget->hide();

    m_ui->pushButtonScan->setFocus();
}

VJoystickMainWindow::~VJoystickMainWindow()
{
    delete m_ui;
    delete m_adapter;
}

void VJoystickMainWindow::connectToDevices()
{
    if(m_adapter) {
        if(m_adapter->isConnected()) {
            m_adapter->disconnect();
        }
        m_adapter->connect(m_ui->comboBoxSelectJoy->currentIndex());
        setJoystickInfoVisible(true);
        setJoystickInfo();

        connect(m_adapter, SIGNAL(sigButtonChanged(int,bool)), this, SLOT(slotButtonChanged(int,bool)));
        connect(m_adapter, SIGNAL(sigAxisChanged(int,int)), this, SLOT(slotAxisChanged(int,int)));
        connect(m_adapter, SIGNAL(sigHatCanged(int,int)), this, SLOT(slotHatCanged(int,int)));
        connect(m_adapter, SIGNAL(sigBallChanged(int,int,int)), this, SLOT(slotBallChanged(int,int,int)));

        m_visibleButtons.fill(false, m_adapter->getJoystickNumButtons());
        advancedConfigurationVisible();
    }
}

void VJoystickMainWindow::disconnectFromDevices()
{
    if(m_adapter) {
        m_adapter->disconnect();
        setJoystickInfoVisible(false);
        clearJoystickInfo();

        disconnect(m_adapter, SIGNAL(sigButtonChanged(int,bool)), this, SLOT(slotButtonChanged(int,bool)));
        disconnect(m_adapter, SIGNAL(sigAxisChanged(int,int)), this, SLOT(slotAxisChanged(int,int)));
        disconnect(m_adapter, SIGNAL(sigHatCanged(int,int)), this, SLOT(slotHatCanged(int,int)));
        disconnect(m_adapter, SIGNAL(sigBallChanged(int,int,int)), this, SLOT(slotBallChanged(int,int,int)));
    }
}

void VJoystickMainWindow::scanDevices()
{
    delete m_adapter;
    m_adapter = new VJoystickAdapter(this);
    if(m_adapter) {
        m_ui->comboBoxSelectJoy->clear();
        m_ui->comboBoxSelectJoy->addItems(m_adapter->getAvaliableJoystickName());

        if(m_ui->comboBoxSelectJoy->count()) {
            m_ui->comboBoxSelectJoy->setEnabled(true);
            setDeviceControlVisible(true);
        }
        else {
            m_ui->comboBoxSelectJoy->setDisabled(true);
            setDeviceControlVisible(false);
            setJoystickInfoVisible(false);
        }
    }
}

void VJoystickMainWindow::slotButtonChanged(int id, bool state)
{
    QString text("");
    if(state) {
        m_visibleButtons[id] = true;
    }
    else {
        m_visibleButtons[id] = false;
    }
    for(int i = 0; i < m_adapter->getJoystickNumButtons(); ++i) {
        if(m_visibleButtons[i]) {
            if(i < 10) {
                text += QString("0%1   ").arg(i);
            }
            else {
                text += QString("%1   ").arg(i);
            }
        }
    }
    m_ui->labelButton->setText(text);
}

void VJoystickMainWindow::slotAxisChanged(int id, int state)
{
    switch(id) {
    case 0:
        m_ui->labelXAxis->setText(QString("%1").arg(state));
        break;

    case 1:
        m_ui->labelYAxis->setText(QString("%1").arg(-state));
        break;

    case 2:
        m_ui->labelXRotate->setText(QString("%1").arg(state));
        break;

    case 3:
        m_ui->labelYRotate->setText(QString("%1").arg(-state));
        break;
    }
}

void VJoystickMainWindow::slotHatCanged(int id, int state)
{
    switch(id) {
    case 0:
        m_ui->labelHat1->setText(getHatsPosition(state));
        break;

    case 1:
        m_ui->labelHat2->setText(getHatsPosition(state));
        break;
    }
}

void VJoystickMainWindow::slotBallChanged(int id, int stateX, int stateY)
{
    Q_UNUSED(id);
    Q_UNUSED(stateX);
    Q_UNUSED(stateY);
}

void VJoystickMainWindow::setJoystickInfoVisible(bool visible)
{
    if(visible) {
        m_ui->groupBoxState->setEnabled(true);
        m_ui->groupBoxInfo->setEnabled(true);
    }
    else {
        m_ui->groupBoxState->setDisabled(true);
        m_ui->groupBoxInfo->setDisabled(true);
    }
}

void VJoystickMainWindow::setDeviceControlVisible(bool visible)
{
    if(visible) {
        m_ui->pushButtonConnect->setEnabled(true);
        m_ui->pushButtonDisconnect->setEnabled(true);
    }
    else {
        m_ui->pushButtonConnect->setDisabled(true);
        m_ui->pushButtonDisconnect->setDisabled(true);
        clearJoystickInfo();
    }
}

void VJoystickMainWindow::setJoystickInfo()
{
    if(m_adapter->isConnected()) {
        m_ui->labelName->setText(m_adapter->getJoystickName());
        m_ui->labelNumButtons->setText(QString("%1").arg(m_adapter->getJoystickNumButtons()));
        m_ui->labelNumAxes->setText(QString("%1").arg(m_adapter->getJoystickNumAxes()));
        m_ui->labelNumHats->setText(QString("%1").arg(m_adapter->getJoystickNumHats()));
        m_ui->labelNumBalls->setText(QString("%1").arg(m_adapter->getJoystickNumBalls()));

        QString noop("0");

        m_ui->labelXAxis->setText(noop);
        m_ui->labelYAxis->setText(noop);
        m_ui->labelXRotate->setText(noop);
        m_ui->labelYRotate->setText(noop);
        m_ui->labelZAxis->setText(noop);
        m_ui->labelHat1->setText(noop);
        m_ui->labelHat2->setText(noop);
        m_ui->labelBall1X->setText(noop);
        m_ui->labelBall1Y->setText(noop);
        m_ui->labelBall2X->setText(noop);
        m_ui->labelBall2Y->setText(noop);
        m_ui->labelButton->setText("");
    }
}

void VJoystickMainWindow::clearJoystickInfo()
{
    QString noop(" ");

    m_ui->labelName->setText(noop);
    m_ui->labelNumButtons->setText(noop);
    m_ui->labelNumAxes->setText(noop);
    m_ui->labelNumHats->setText(noop);
    m_ui->labelNumBalls->setText(noop);

    m_ui->labelXAxis->setText(noop);
    m_ui->labelYAxis->setText(noop);
    m_ui->labelXRotate->setText(noop);
    m_ui->labelYRotate->setText(noop);
    m_ui->labelZAxis->setText(noop);
    m_ui->labelHat1->setText(noop);
    m_ui->labelHat2->setText(noop);
    m_ui->labelBall1X->setText(noop);
    m_ui->labelBall1Y->setText(noop);
    m_ui->labelBall2X->setText(noop);
    m_ui->labelBall2Y->setText(noop);
    m_ui->labelButton->setText(noop);
}

void VJoystickMainWindow::advancedConfigurationVisible()
{
    if(m_adapter->getJoystickNumAxes() < 5) {
        m_ui->labelZAxis->setDisabled(true);
        m_ui->labelZAxisText->setDisabled(true);
    }
    if(m_adapter->getJoystickNumHats() < 2) {
        m_ui->labelHat2->setDisabled(true);
        m_ui->labelHat2Text->setDisabled(true);
        if(m_adapter->getJoystickNumHats() < 1) {
            m_ui->labelHat1->setDisabled(true);
            m_ui->labelHat1Text->setDisabled(true);
        }
    }
}

QString VJoystickMainWindow::getHatsPosition(int pos)
{
    QString text("");

    switch(pos) {
    case VJoystickAdapter::JOYSTICK_HAT_CENTERED:
        text += "centre";
        break;

    case VJoystickAdapter::JOYSTICK_HAT_UP:
        text += "up";
        break;

    case VJoystickAdapter::JOYSTICK_HAT_UP_RIGHT:
        text += "up right";
        break;

    case VJoystickAdapter::JOYSTICK_HAT_RIGHT:
        text += "right";
        break;

    case VJoystickAdapter::JOYSTICK_HAT_DOWN_RIGHT:
        text += "right down";
        break;

    case VJoystickAdapter::JOYSTICK_HAT_DOWN:
        text += "down";
        break;

    case VJoystickAdapter::JOYSTICK_HAT_DOWN_LEFT:
        text += "down left";
        break;

    case VJoystickAdapter::JOYSTICK_HAT_LEFT:
        text += "left";
        break;

    case VJoystickAdapter::JOYSTICK_HAT_UP_LEFT:
        text += "left up";
        break;
    }
    return text;
}

void VJoystickMainWindow::appExit()
{
    close();
}

void VJoystickMainWindow::about()
{
    VJoystickAboutDialog dialog(this);
    dialog.exec();
}
