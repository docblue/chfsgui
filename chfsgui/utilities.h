#ifndef UTILITIES_H
#define UTILITIES_H


class Utilities
{
public:
    Utilities();
};

int compareIPv4(QString a, QString b);
QString rule2humanbeings(QString rawrule);
QString makeRW(bool r , bool w, bool d);
void convertFromRulestr(QString rulestr, QMap<QString,QMap<QString,QString>>& rules, QMap<QString, QString>& accs);
void removeUser(QString user);
QMap<QString, QString> getAccsFromRulestr();
QMap<QString,QString> getRulesFromRulestr(QString user);
void addUser(QString user, QString pwd, const QMap<QString, QString>& addedrule);

#endif // UTILITIES_H
