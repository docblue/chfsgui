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
    bool hasR = rule.contains("R");
    bool hasW = rule.contains("W");

    if(!hasR && !hasW){
        return "不可读写";
    }else if(hasW){
        return "读写";
    }else{
        return "只读";
    }
}
