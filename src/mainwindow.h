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

#pragma once

#include <QLocalSocket>
#include <QMainWindow>
#include <QCloseEvent>
#include <QSettings>
#include <QObject>
#include <QDateTime>
#include <QColor>

#include "filenamedrophandler.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void saveSession();
    void restoreSession();

private slots:
    void closeEvent(QCloseEvent *event);
    void onConnect();
    void onSend();
    void onStateChanged(QLocalSocket::LocalSocketState socketState);
    void onReadyRead();

private:
    Ui::MainWindow *ui;
    QLocalSocket mSocket;
    FileNameDropHandler mSocketDropHandler;
    FileNameDropHandler mMessageDropHandler;

    void appendLog(QString message);
    QString colorize(QString text, QString = "");
    QString stateToString(QLocalSocket::LocalSocketState socketState);
};
