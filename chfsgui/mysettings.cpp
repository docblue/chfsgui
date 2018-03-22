#include "mysettings.h"
#include <QApplication>
#include <utility>

MySettings::MySettings(const QString & organization, const QString & application,QObject *parent) :
    QSettings(organization, application, parent)
{

}


QList<LauncherItem> MySettings::getAllItems()
{
    QList<LauncherItem> res;

    QStringList keys = allKeys();
    if( keys.isEmpty() ){
        LauncherItem item;
        item._key = DEFAULTITEM;
        item._isLastUsed = true;
        item._isAutoRun = false;
        item._value = QStringList{"","80","",""};
        res << item;
        setValue(item._key,LauncherItem::encode(item));
    }else{
        for(auto i: keys){
            res << LauncherItem::decode(i, this->value(i).toString());
        }
    }

    return res;
}


QStringList MySettings::getAllKeys()
{
    QStringList keys = allKeys();
    if( keys.isEmpty() ){
        keys << DEFAULTITEM;
    }

    return keys;
}

LauncherItem MySettings::onCreateSaveNewItem()
{
    LauncherItem item;
    item._key = getUniqueItemName();
    item._isLastUsed = false;
    item._isAutoRun = false;
    item._value = QStringList{QApplication::applicationDirPath(),"80","",""};
    setValue(item._key,LauncherItem::encode(item));
    return item;
}

QString MySettings::getUniqueItemName()
{
    QString newName(tr("新建启动项"));
    QStringList keys = allKeys();

    if( keys.contains(newName) ){
        for(int i=0;i<10000; i++){
            QString tmp = newName + (i>0?QString::number(i):"");
            if(keys.contains(tmp) == false){
                newName = tmp;
                break;
            }
        }
    }

    return newName;
}

void MySettings::renameKey(QString oldname, QString newname)
{
    auto v = this->value(oldname);
    this->remove(oldname);
    this->setValue(newname,v);
}

void MySettings::setValue(const QString & key, const QVariant & value)
{
    QSettings::setValue(key,value);
    sync();
}

void MySettings::toggleItemAutoFlag(QString key)
{
    LauncherItem item = LauncherItem::decode(key, value(key).toString());
    bool old = item._isAutoRun;

    const QList<LauncherItem>& items = getAllItems();
    for(auto i: items){
        if( i._key == key ){
            i._isAutoRun = !old;
        }else if( old == false ){
            i._isAutoRun = false;
        }
        setValue(i._key, LauncherItem::encode(i));
    }
}

void MySettings::setItemLastusedFlag(QString key)
{
    const QList<LauncherItem>& items = getAllItems();
    for(auto i: items){
        i._isLastUsed = (i._key == key ? true : false);
        setValue(i._key, LauncherItem::encode(i));
    }
}

LauncherItem MySettings::getItem(QString key)
{
    return LauncherItem::decode(key, this->value(key).toString());
}

void MySettings::onCurrentItemChanging(QString key)
{
    emit currentItemChanged(key);
}

QString MySettings::getPathValue(QString key)
{
    QString res = getParamValue(key, PARAM_PATH);
    if( res.isEmpty() ){
        res = QApplication::applicationDirPath();
    }
    return res;
}

QString MySettings::getPortValue(QString key)
{
    return getParamValue(key, PARAM_PORT);
}

QString MySettings::getAllowValue(QString key)
{
    return getParamValue(key, PARAM_ALLOW);
}

QString MySettings::getRuleValue(QString key)
{
    return getParamValue(key, PARAM_RULE);
}

void MySettings::setPathValue(QString key, QString value)
{
    setParamValue(key,value,PARAM_PATH);
}

void MySettings::setPortValue(QString key, QString value)
{
    setParamValue(key,value,PARAM_PORT);
}

void MySettings::setAllowValue(QString key, QString value)
{
    setParamValue(key,value,PARAM_ALLOW);
}

void MySettings::setRuleValue(QString key, QString value)
{
    setParamValue(key,value,PARAM_RULE);
}

QString MySettings::getParamValue(QString key, QString which)
{
    LauncherItem item = this->getItem(key);
    int size = item._value.length();
    if( which == PARAM_PATH && size>0)
        return item._value.at(0);
    else if( which == PARAM_PORT  && size>1)
        return item._value.at(1);
    else if( which == PARAM_ALLOW  && size>2)
        return item._value.at(2);
    else if( size>3 )
        return item._value.at(3);

    return "";
}

void MySettings::setParamValue(QString key, QString value, QString which)
{
    LauncherItem item = this->getItem(key);
    int size = item._value.length();
    if( which == PARAM_PATH && size>=1 )
        item._value.replace(0,value);
    else if( which == PARAM_PORT && size>=2 )
        item._value.replace(1,value);
    else if( which == PARAM_ALLOW && size>=3 )
        item._value.replace(2,value);
    else if( size>=4 )
        item._value.replace(3,value);

    this->setValue(key, LauncherItem::encode(item));
}



void MySettings::convertFromRulestr(QString key, QMap<QString,QMap<QString,QString>>& rules, QMap<QString, QString>& accs)
{
    rules.clear();
    accs.clear();

    QString rulestr = getRuleValue(key);
    rules[GUESTUSER] = QMap<QString,QString>{std::make_pair(ROOTPATH,"RW")};

    QStringList rs = rulestr.split("|");
    for(int i=0; i<rs.length(); i++){
        QStringList pairs = rs.at(i).split(":");

        if( pairs.length() >=3 && (pairs.length()-3)%2==0){
            QMap<QString,QString> r;

            r[ROOTPATH] = pairs[2];
            QString user(pairs[0]),pwd(pairs[1]);
            for(int j=3;j<pairs.length(); j+=2){
                r[pairs.at(j)] = pairs.at(j+1);
            }

            if( user.isEmpty() && pwd.isEmpty() ){
                rules[GUESTUSER] = r;
            }else if( !user.isEmpty() && !pwd.isEmpty()){
                rules[user] = r;
                accs[user] = pwd;
            }
        }
    }
}

void MySettings::setRulesFromConverted(QString key, const QMap<QString,QMap<QString,QString>>& rules, const QMap<QString, QString>& accs)
{
    QString str;

    auto i = rules.constBegin();
    while (i != rules.constEnd()) {
        QString user = i.key();
        auto arules = i.value();

        if( user ==  GUESTUSER ){
            str += QString("::%3").arg(arules[ROOTPATH]);
        }else{
            str += QString("%1:%2:%3").arg(user).arg(accs[user]).arg(arules[ROOTPATH]);
        }

        auto j = arules.constBegin();
        while (j != arules.constEnd()) {
            QString dir = j.key();
            QString mask = j.value();

            if( dir !=  ROOTPATH ){
                str += QString(":%1:%2").arg(dir).arg(mask);
            }
            ++j;
        }

        str += "|";

        ++i;
    }

    if(str.endsWith("|")){
        str.remove(str.length()-1,1);
    }

    setRuleValue(key, str);
}

void MySettings::addUser(QString key, QString user, QString pwd, const QMap<QString, QString>& addedrule)
{
    QMap<QString,QMap<QString,QString>> rules;
    QMap<QString, QString> acces;
    convertFromRulestr(key, rules, acces);

    if( user != GUESTUSER ){
        acces[user] = pwd;
    }
    rules[user] = addedrule;

    setRulesFromConverted(key, rules, acces);
}

void MySettings::modifyUser(QString key, QString user, QString newpwd)
{
    QMap<QString,QMap<QString,QString>> rules;
    QMap<QString, QString> acces;
    convertFromRulestr(key, rules, acces);

    acces[user] = newpwd;

    setRulesFromConverted(key, rules, acces);
}

void MySettings::removeUser(QString key, QString user)
{
    QMap<QString,QMap<QString,QString>> rules;
    QMap<QString, QString> acces;
    convertFromRulestr(key, rules, acces);

    acces.remove(user);
    rules.remove(user);

    setRulesFromConverted(key, rules, acces);
}

void MySettings::addRule(QString key, QString user, QString dir, QString mask)
{
    QMap<QString,QMap<QString,QString>> rules;
    QMap<QString, QString> acces;
    convertFromRulestr(key, rules, acces);

    rules[user][dir] = mask;

    setRulesFromConverted(key, rules, acces);
}

void MySettings::modifyRule(QString key, QString user, QString dir, QString newmask)
{
    QMap<QString,QMap<QString,QString>> rules;
    QMap<QString, QString> acces;
    convertFromRulestr(key, rules, acces);

    rules[user][dir] = newmask;

    setRulesFromConverted(key, rules, acces);
}

void MySettings::removeRule(QString key, QString user, QString dir)
{
    QMap<QString,QMap<QString,QString>> rules;
    QMap<QString, QString> acces;
    convertFromRulestr(key, rules, acces);

    rules[user].remove(dir);

    setRulesFromConverted(key, rules, acces);
}

QMap<QString,QString> MySettings::getRulesFromRulestr(QString key, QString user)
{
    QMap<QString,QMap<QString,QString>> rules;
    QMap<QString, QString> acces;
    convertFromRulestr(key, rules, acces);
    return rules[user];
}

QMap<QString, QString> MySettings::getAccsFromRulestr(QString key)
{
    QMap<QString,QMap<QString,QString>> rules;
    QMap<QString, QString> acces;
    convertFromRulestr(key, rules, acces);
    return acces;
}
