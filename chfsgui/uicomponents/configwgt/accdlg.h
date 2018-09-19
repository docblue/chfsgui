#ifndef ACCDLG_H
#define ACCDLG_H

class QLineEdit;
class QCheckBox;
class QPushButton;
class RuleTableWidget;
class QMenu;
class QAction;


struct AccDlgUIComponents{
    QLineEdit* editUser;
    QLineEdit* editPwd;
    QPushButton* btnOK;
    QPushButton* _btnCancel;
    QCheckBox* cbRead;
    QCheckBox* cbWrite;
    QCheckBox* cbDelete;
    QPushButton* btnNew;
    QPushButton* btnRemove;
    RuleTableWidget* maskTable;
    QMenu* addMenu;
};

class AccDlg : public QDialog
{
    Q_OBJECT
public:

    explicit AccDlg(QString acc = "" /*用来标识是添加还是修改*/, QWidget *parent = 0);

private:
    AccDlgUIComponents ui;
    QAction* _chooseAction;
    QAction* _typeAction;

    QString _currentKey;
    QString _accModify;

    void createContents();
    void createMenu();
    QString createDirName();
    inline bool isAddMode(){return _accModify.isEmpty();}
signals:

public slots:
    void onBtnOK();
};

#endif // ACCDLG_H
