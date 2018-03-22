#ifndef ACCDLG_H
#define ACCDLG_H

class QLineEdit;
class QCheckBox;
class QPushButton;
class RuleTableWidget;
class QMenu;
class QAction;

class AccDlg : public QDialog
{
    Q_OBJECT
public:

    explicit AccDlg(QString key, QString acc = "" /*用来标识是添加还是修改*/, QWidget *parent = 0);

private:
    QLineEdit* _editUser;
    QLineEdit* _editPwd;
    QPushButton* _btnOK;
    QPushButton* _btnCancel;
    QCheckBox* _cbRead;
    QCheckBox* _cbWrite;
    QPushButton* _btnNew;
    QPushButton* _btnRemove;
    RuleTableWidget* _maskTable;
    QMenu* _addMenu;
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
