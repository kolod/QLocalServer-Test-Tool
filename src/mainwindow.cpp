//    QLocalServer Test Tool
//    Copyright (C) 2021  Oleksandr Kolodkin <alexandr.kolodkin@gmail.com>
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mSocketDropHandler(this)
    , mMessageDropHandler(this)
{
    ui->setupUi(this);

    connect(ui->buttonConnect, &QPushButton::clicked, this, &MainWindow::onConnect);
    connect(ui->buttonSend, &QPushButton::clicked, this, &MainWindow::onSend);
    connect(ui->inputMessage, &QLineEdit::editingFinished, this, &MainWindow::onSend);
    connect(&mSocket, &QLocalSocket::stateChanged, this, &MainWindow::onStateChanged);
    connect(&mSocket, &QLocalSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(&mSocketDropHandler, &FileNameDropHandler::fileNameDropped, ui->inputSocket, &QLineEdit::setText);
    connect(&mMessageDropHandler, &FileNameDropHandler::fileNameDropped, ui->inputMessage, &QLineEdit::setText);

    ui->inputSocket->installEventFilter(&mSocketDropHandler);
    ui->inputMessage->installEventFilter(&mMessageDropHandler);
}

MainWindow::~MainWindow()
{
    if (mSocket.state() != QLocalSocket::UnconnectedState) mSocket.close();
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSession();
    event->accept();
}

void MainWindow::saveSession()
{
    QSettings settings;

    settings.setValue("geometry"     , saveGeometry());
    settings.setValue("state"        , saveState());
    settings.setValue("socket"       , ui->inputSocket->text());
    settings.setValue("message"      , ui->inputMessage->text());
}

void MainWindow::restoreSession()
{
    QSettings settings;

    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());

    ui->inputSocket->setText(settings.value("socket").toString());
    ui->inputMessage->setText(settings.value("message").toString());
}


void MainWindow::onConnect() {
    auto socket = ui->inputSocket->text();
    appendLog(tr("Attempting to connect to the server via socket: %1.").arg(socket));
    mSocket.connectToServer(socket, QIODevice::ReadWrite);
}

void MainWindow::onSend() {
    if (mSocket.state() != QLocalSocket::ConnectedState) {
        appendLog(tr("Error: Socket not opened!"));
        return;
    }

    auto message =ui->inputMessage->text() + "\n";
    auto count = mSocket.write(message.toUtf8());
    appendLog(tr("Message '%1' sent.").arg(message));
    appendLog(tr("  %1 bytes writed.").arg(count));
}

void MainWindow::onStateChanged(QLocalSocket::LocalSocketState socketState) {
    appendLog(tr("Socket state changed to: %1.").arg(stateToString(socketState)));
}

void MainWindow::onReadyRead() {
    auto message = QString::fromUtf8(mSocket.readAll());
    appendLog(tr("Reaceived message from server:\n%1").arg(message));
}

void MainWindow::appendLog(QString message) {
    QString msg = colorize(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz (t) : "), "LightYellow") + message;
    ui->log->append(msg);
}

//TODO: Add support for light/dark thames
QString MainWindow::colorize(QString text, QString color) {
    if (color.isEmpty()) return text;
    return QString("<font color=\"%2\">%1</font>").arg(text, color);
}

QString MainWindow::stateToString(QLocalSocket::LocalSocketState socketState) {
    switch (socketState) {
    case QLocalSocket::ConnectedState:   return tr("Connected");
    case QLocalSocket::ConnectingState:  return tr("Connecting");
    case QLocalSocket::ClosingState:     return tr("Closing");
    case QLocalSocket::UnconnectedState: return tr("Unconnected");
    default: return QString();
    }
}
