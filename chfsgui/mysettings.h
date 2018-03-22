#ifndef MYSETTINGS_H
#define MYSETTINGS_H

#include <QSettings>
#define ITEMSEP "^*^"
#define DEFAULTITEM "默认启动项"
#define PARAM_PATH "PATH"
#define PARAM_PORT "PORT"
#define PARAM_ALLOW "ALLOW"
#define PARAM_RULE "RULE"
#define GUESTUSER "_-_guestuser_-_"
#define ROOTPATH "/"
struct LauncherItem{
    QString _key;
    bool _isLastUsed;
    bool _isAutoRun;
    QStringList _value;

    static LauncherItem decode(QString key,QString value){
        LauncherItem item;

        item._key = key;
        QStringList list = value.split(ITEMSEP);
        for(int i=0; i<list.length(); i++){
            if(i==0){
                item._isLastUsed = list.at(i)=="t";
            }else if(i==1){
                item._isAutoRun = list.at(i)=="t";
            }else{     
                item._value.append(list.at(i));
            }
        }

        return item;
    }

    static  QString encode(const LauncherItem& item){
        QString res;
        res += item._isLastUsed?"t":"f";
        res += ITEMSEP;
        res += item._isAutoRun?"t":"f";
        res += ITEMSEP;

        for(int i=0; i<item._value.length(); i++){
            res += item._value.at(i);
            res += ITEMSEP;
        }

        res.remove(res.length()-QString(ITEMSEP).length(),
                   QString(ITEMSEP).length());

        return res;
    }
};

class MySettings : public QSettings
{
    Q_OBJECT
public:
    static MySettings & instance()
    {
        static MySettings _instance(APPDOMAIN, APPKEY);
        return _instance;
    }

    QList<LauncherItem> getAllItems();
    QStringList getAllKeys();
    void renameKey(QString oldname, QString newname);
    void toggleItemAutoFlag(QString key);
    void setItemLastusedFlag(QString key);
    LauncherItem getItem(QString key);

    QString getPathValue(QString key);
    QString getPortValue(QString key);
    QString getAllowValue(QString key);
    QString getRuleValue(QString key);
    void setPathValue(QString key, QString value);
    void setPortValue(QString key, QString value);
    void setAllowValue(QString key, QString value);
    void setRuleValue(QString key, QString value);

    QMap<QString, QString> getAccsFromRulestr(QString key);
    QMap<QString,QString> getRulesFromRulestr(QString key, QString user);
    void setRulesFromConverted(QString key, const QMap<QString,QMap<QString,QString>>&, const QMap<QString, QString>&);
    void addUser(QString key, QString user, QString pwd, const QMap<QString, QString>& addedrule); // or modify
    void modifyUser(QString key, QString user, QString newpwd);
    void removeUser(QString key, QString user);
    void addRule(QString key, QString user, QString dir, QString mask);
    void modifyRule(QString key, QString user, QString dir, QString newmask);
    void removeRule(QString key, QString user, QString dir);
private:
    MySettings(const QString & organization, const QString & application = QString(),QObject *parent = 0);

    void setValue(const QString & key, const QVariant & value);
    QString getUniqueItemName();
    QString getParamValue(QString key, QString which);
    void setParamValue(QString key, QString value, QString which);
    void convertFromRulestr(QString key, QMap<QString,QMap<QString,QString>>&, QMap<QString, QString>&);
signals:
    void currentItemChanged(QString key);
public slots:
    LauncherItem onCreateSaveNewItem();
    void onCurrentItemChanging(QString key);
};

#endif // MYSETTINGS_H
