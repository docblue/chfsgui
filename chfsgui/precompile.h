#ifndef PRECOMPLIE_H
#define PRECOMPLIE_H
#include <QObject>
#include <QPushButton>
#include <QBoxLayout>
#include <QLabel>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>
#include <QProcess>
#include <QLineEdit>
#include <QMessageBox>
#include <QApplication>
#include <QCheckBox>

#define REG_RUN "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define APPKEY "chfsgui"
#define APPDOMAIN "iscute.me"
#define AUTORUNFORBIDDEN "forbidden"

extern QString g_chfsfile;
extern QProcess g_chfsProcess;

#endif // PRECOMPLIE_H

