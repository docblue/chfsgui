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
#include <QSettings>

#define REG_RUN "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define APPKEY "chfsgui"
#define APPDOMAIN "iscute.cn"
#define AUTORUNFORBIDDEN "forbidden"

#define PARAM_PATH "PATH"
#define PARAM_PORT "PORT"
#define PARAM_ALLOW "ALLOW"
#define PARAM_RULE "RULE"
#define PARAM_AUTO_LAUNCHE "AUTO"
#define GUESTUSER "_-_guestuser_-_"
#define ROOTPATH "/"

extern QString g_chfsfile;
extern QProcess g_chfsProcess;
extern QSettings g_settings;

#endif // PRECOMPLIE_H

