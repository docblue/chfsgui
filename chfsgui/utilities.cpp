#include "utilities.h"
#include "precompile.h"

Utilities::Utilities()
{

}

int compareIPv4(QString a, QString b)
{
    QString newa, newb;

    for(QString i: a.split(".")){
        newa += QString("%1.").arg(i,3,QLatin1Char('0'));
    }
    newa.remove(newa.length()-1,1);

    for(QString i: b.split(".")){
        newb += QString("%1.").arg(i,3,QLatin1Char('0'));
    }
    newb.remove(newb.length()-1,1);

    return newa.compare(newb);
}

QString rule2humanbeings(QString rawrule)
{
    QString rule = rawrule.toUpper();

    if( rule.contains("D") )
        return "读写+删除";
    else if( rule.contains("W") )
        return "读写";
    else if( rule.contains("R") )
        return "只读";
    else
        return "不可读写";
}


QString makeRW(bool r , bool w, bool d)
{
    if( d )         return "RWD";
    else if( w )    return "RW";
    else if( r )    return "R";
    else            return "";
}


void convertFromRulestr(QString rulestr, QMap<QString,QMap<QString,QString>>& rules, QMap<QString, QString>& accs)
{
    rules.clear();
    accs.clear();

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

void setRulesFromConverted(const QMap<QString,QMap<QString,QString>>& rules, const QMap<QString, QString>& accs)
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

    g_settings.setValue(PARAM_RULE, str);
}

void addUser(QString user, QString pwd, const QMap<QString, QString>& addedrule)
{
    QMap<QString,QMap<QString,QString>> rules;
    QMap<QString, QString> acces;
    convertFromRulestr(g_settings.value(PARAM_RULE).toString(), rules, acces);

    if( user != GUESTUSER ){
        acces[user] = pwd;
    }
    rules[user] = addedrule;

    setRulesFromConverted(rules, acces);
}

void modifyUser(QString user, QString newpwd)
{
    QMap<QString,QMap<QString,QString>> rules;
    QMap<QString, QString> acces;
    convertFromRulestr(g_settings.value(PARAM_RULE).toString(), rules, acces);

    acces[user] = newpwd;

    setRulesFromConverted(rules, acces);
}

void removeUser(QString user)
{
    QMap<QString,QMap<QString,QString>> rules;
    QMap<QString, QString> acces;
    convertFromRulestr(g_settings.value(PARAM_RULE).toString(), rules, acces);

    acces.remove(user);
    rules.remove(user);

    setRulesFromConverted(rules, acces);
}

void addRule(QString user, QString dir, QString mask)
{
    QMap<QString,QMap<QString,QString>> rules;
    QMap<QString, QString> acces;
    convertFromRulestr(g_settings.value(PARAM_RULE).toString(), rules, acces);

    rules[user][dir] = mask;

    setRulesFromConverted(rules, acces);
}

void modifyRule(QString user, QString dir, QString newmask)
{
    QMap<QString,QMap<QString,QString>> rules;
    QMap<QString, QString> acces;
    convertFromRulestr(g_settings.value(PARAM_RULE).toString(), rules, acces);

    rules[user][dir] = newmask;

    setRulesFromConverted(rules, acces);
}

void removeRule(QString user, QString dir)
{
    QMap<QString,QMap<QString,QString>> rules;
    QMap<QString, QString> acces;
    convertFromRulestr(g_settings.value(PARAM_RULE).toString(), rules, acces);

    rules[user].remove(dir);

    setRulesFromConverted(rules, acces);
}

QMap<QString,QString> getRulesFromRulestr(QString user)
{
    QMap<QString,QMap<QString,QString>> rules;
    QMap<QString, QString> acces;
    convertFromRulestr(g_settings.value(PARAM_RULE).toString(), rules, acces);
    return rules[user];
}

QMap<QString, QString> getAccsFromRulestr()
{
    QMap<QString,QMap<QString,QString>> rules;
    QMap<QString, QString> acces;
    convertFromRulestr(g_settings.value(PARAM_RULE).toString(), rules, acces);
    return acces;
}

