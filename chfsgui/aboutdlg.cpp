#include "aboutdlg.h"
#include "ui_aboutdlg.h"
#include "precompile.h"


QString AboutDlg::getCHFSver()
{
    QProcess chfs;
    chfs.start(g_chfsfile, QStringList() << "--version");
    chfs.waitForFinished();

    return QString(chfs.readAllStandardError()).remove("\n");
}

AboutDlg::AboutDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDlg)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);

    ui->_lbTitle->setText( QString("<h2>CHFS GUI v%1.%2</h2>").arg(getCHFSver()).arg(GUIVER) );
}

AboutDlg::~AboutDlg()
{
    delete ui;
}

void AboutDlg::on_pushButton_clicked()
{
    close();
}
