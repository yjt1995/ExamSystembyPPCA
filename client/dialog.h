#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMenu>
#include <QTextStream>
#include <QIODevice>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include "windows.h"
#include "messagedialog.h"
#include "screenwindow.h"
#include "spifirewall.h"
#include "stuserver.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

private:
    QStringList slist;
    QStringList slist2;

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    void CreatTrayMenu();
    void CreatTrayIcon();

    QSystemTrayIcon *myTrayIcon;

    QMenu *myMenu;

    QAction *submitAction;
    QAction *quitAction;
    QAction *messageAction;
    QAction *aboutAction;

    MessageDialog *w2;
    screenWindow *w3;
    spiFirewall *fire;
    StuServer *stu;

public slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

private slots:
    void quitWithMyWarning();
    void selectFiles();
    void submitFiles();
    void showAbout();
    void beginExam();
    void endExam();
    void lockScreen();
    void unlockScreen();
    void gotIP(QString comIP);
    void recieveMessage(QString mess);

signals:
    void submitFile(QStringList fileName);
    void showTranslateMessage();
    void appendMessage(QString mess);

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::Dialog *ui;
    bool isLocked;
    bool isExamming;
};

#endif // DIALOG_H
