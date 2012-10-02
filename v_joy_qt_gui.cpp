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

// ------------------------------------------------------------------- INCLUDES
#include <QKeyEvent>
// ------------------------------------------------------------------- SYNOPSIS
#include <v_joy_qt_gui.h>
#include <ui_v_joy_qt_gui.h>
#include <v_joystick_adapter.h>
// ----------------------------------------------------------------------------

VJoyQtGUI::VJoyQtGUI(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), m_adapter(0)
{
    ui->setupUi(this);

    connect(ui->scanDevicesButton, SIGNAL(clicked()), this, SLOT(scanDevices()));
    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(connectToDevices()));
    connect(ui->disconnectButton, SIGNAL(clicked()), this, SLOT(disconnectFromDevices()));

    ui->selectJoyBox->setDisabled(true);
    setJoystickInfoVisible(false);
    setDeviceControlVisible(false);

    ui->ballWidget->hide();

    ui->scanDevicesButton->setFocus();
}

VJoyQtGUI::~VJoyQtGUI()
{
    delete ui;
    delete m_adapter;
}

void VJoyQtGUI::connectToDevices()
{
    if(m_adapter) {
        if(m_adapter->isConnected()) {
            m_adapter->disconnect();
        }
        m_adapter->connect(ui->selectJoyBox->currentIndex());
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

void VJoyQtGUI::disconnectFromDevices()
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

void VJoyQtGUI::scanDevices()
{
    delete m_adapter;
    m_adapter = new VJoystickAdapter(this);
    if(m_adapter) {
        ui->selectJoyBox->clear();
        ui->selectJoyBox->addItems(m_adapter->getAvaliableJoystickName());

        if(ui->selectJoyBox->count()) {
            ui->selectJoyBox->setEnabled(true);
            setDeviceControlVisible(true);
        }
        else {
            ui->selectJoyBox->setDisabled(true);
            setDeviceControlVisible(false);
            setJoystickInfoVisible(false);
        }
    }
}

void VJoyQtGUI::slotButtonChanged(int id, bool state)
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
    ui->buttonLabel->setText(text);
}

void VJoyQtGUI::slotAxisChanged(int id, int state)
{
    switch(id) {
    case 0:
        ui->xAxisLabel->setText(QString("%1").arg(state));
        break;

    case 1:
        ui->yAxisLabel->setText(QString("%1").arg(-state));
        break;

    case 2:
        ui->xRotateLabel->setText(QString("%1").arg(state));
        break;

    case 3:
        ui->yRotateLabel->setText(QString("%1").arg(-state));
        break;
    }
}

void VJoyQtGUI::slotHatCanged(int id, int state)
{
    switch(id) {
    case 0:
        ui->hat1Label->setText(getHatsPosition(state));
        break;

    case 1:
        ui->hat2Label->setText(getHatsPosition(state));
        break;
    }
}

void VJoyQtGUI::slotBallChanged(int id, int stateX, int stateY)
{
    Q_UNUSED(id);
    Q_UNUSED(stateX);
    Q_UNUSED(stateY);
}

void VJoyQtGUI::keyPressEvent(QKeyEvent *event)
{
    event->accept();
    switch(event->key()) {
    case Qt::Key_S:
        scanDevices();
        break;

    case Qt::Key_C:
        connectToDevices();
        break;

    case Qt::Key_D:
        disconnectFromDevices();
        break;

    case Qt::Key_Escape:
        close();
        break;

    default:
        event->ignore();
        break;
    }
    QMainWindow::keyPressEvent(event);
}

void VJoyQtGUI::setJoystickInfoVisible(bool visible)
{
    if(visible) {
        ui->joyStateBox->setEnabled(true);
        ui->joyInfoBox->setEnabled(true);
    }
    else {
        ui->joyStateBox->setDisabled(true);
        ui->joyInfoBox->setDisabled(true);
    }
}

void VJoyQtGUI::setDeviceControlVisible(bool visible)
{
    if(visible) {
        ui->connectButton->setEnabled(true);
        ui->disconnectButton->setEnabled(true);
    }
    else {
        ui->connectButton->setDisabled(true);
        ui->disconnectButton->setDisabled(true);
        clearJoystickInfo();
    }
}

void VJoyQtGUI::setJoystickInfo()
{
    if(m_adapter->isConnected()) {
        ui->nameLabel->setText(m_adapter->getJoystickName());
        ui->numButtonsLabel->setText(QString("%1").arg(m_adapter->getJoystickNumButtons()));
        ui->numAxesLabel->setText(QString("%1").arg(m_adapter->getJoystickNumAxes()));
        ui->numHatsLabel->setText(QString("%1").arg(m_adapter->getJoystickNumHats()));
        ui->numBallsLabel->setText(QString("%1").arg(m_adapter->getJoystickNumBalls()));

        QString noop("0");

        ui->xAxisLabel->setText(noop);
        ui->yAxisLabel->setText(noop);
        ui->xRotateLabel->setText(noop);
        ui->yRotateLabel->setText(noop);
        ui->zAxisLabel->setText(noop);
        ui->hat1Label->setText(noop);
        ui->hat2Label->setText(noop);
        ui->ball1XLabel->setText(noop);
        ui->ball1YLabel->setText(noop);
        ui->ball2XLabel->setText(noop);
        ui->ball2YLabel->setText(noop);
        ui->buttonLabel->setText("");
    }
}

void VJoyQtGUI::clearJoystickInfo()
{
    QString noop(" ");

    ui->nameLabel->setText(noop);
    ui->numButtonsLabel->setText(noop);
    ui->numAxesLabel->setText(noop);
    ui->numHatsLabel->setText(noop);
    ui->numBallsLabel->setText(noop);

    ui->xAxisLabel->setText(noop);
    ui->yAxisLabel->setText(noop);
    ui->xRotateLabel->setText(noop);
    ui->yRotateLabel->setText(noop);
    ui->zAxisLabel->setText(noop);
    ui->hat1Label->setText(noop);
    ui->hat2Label->setText(noop);
    ui->ball1XLabel->setText(noop);
    ui->ball1YLabel->setText(noop);
    ui->ball2XLabel->setText(noop);
    ui->ball2YLabel->setText(noop);
    ui->buttonLabel->setText(noop);
}

void VJoyQtGUI::advancedConfigurationVisible()
{
    if(m_adapter->getJoystickNumAxes() < 5) {
        ui->zAxisLabel->setDisabled(true);
        ui->zAxisTextLabel->setDisabled(true);
    }
    if(m_adapter->getJoystickNumHats() < 2) {
        ui->hat2Label->setDisabled(true);
        ui->hat2TextLabel->setDisabled(true);
        if(m_adapter->getJoystickNumHats() < 1) {
            ui->hat1Label->setDisabled(true);
            ui->hat1TextLabel->setDisabled(true);
        }
    }
}

QString VJoyQtGUI::getHatsPosition(int pos)
{
    QString text("");

    switch(pos) {
    case VJoystickAdapter::JOYSTICK_HAT_CENTERED:
        text += "centered";
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
