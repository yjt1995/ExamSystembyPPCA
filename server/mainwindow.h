#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDir>
#include <QTextStream>
#include <QIODevice>
#include <QMainWindow>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QListView>
#include <QInputDialog>
#include "messagedialog.h"
#include "teaclientthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void CreatTrayMenu();
    void CreatTrayIcon();
    void beginExam();
    void endExam();
    void lockExam();
    void unlockExam();


    QSystemTrayIcon *myTrayIcon;

    QMenu *myMenu;

    QAction *showAction;
    QAction *quitAction;
    QAction *messageAction;
    QAction *aboutAction;

    MessageDialog *w2;

    QAction *beginAction;
    QAction *endAction;
    QAction *lockAction;
    QAction *unlockAction;
    QAction *postMessageAction;
    QAction *postSingleMessageAction;
    QAction *messageRecordAction;
    QAction *singleMessageAction;
    QAction *linkStateAction;
    QAction *singleStateAction;
    QAction *relinkAction;
    QAction *stopLinkAction;

    QMenu *basicMenu;
    QMenu *messageMenu;
    QMenu *linkMenu;
    QMenu *aboutMenu;

    QString allMessage;
    QString everyMessage[200];

    QStandardItemModel *computerGroup;
    QStandardItem *computerItem[200];

    QMenu *rightMenu;
    QAction *rightLinkAction;
    QAction *rightMessageAction;
    QAction *rightStateAction;
    QAction *rightUnlinkAction;

public slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

private slots:
    void quitWithMyWarning();
    void beginWithWarning();
    void endWithWarning();
    void lockWithWarning();
    void unlockWithWarning();
    void postMessage();
    void postSingleMessage();
    void messageRecord();
    void singleMessage();
    void showLinkState();
    void showSingleState();
    void showAllMessage();
    void singleRelink();
    void stopLink();
    void showAbout();
    void rightMessage();
    void rightState();
    void rightLink();
    void rightUnlink();
    void error(int num);
    void succ(int num,QString command);
    void on_listView_customContextMenuRequested(const QPoint &pos);
    void teaClient(QString ipAdr, QString command, QString arg, int num);

    void on_listView_doubleClicked(const QModelIndex &index);

signals:
    void showMessage(QString &mess);
    void translateMessage(int com,QString mess);

protected:
    void closeEvent(QCloseEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    Ui::MainWindow *ui;
    QString everyIP[200];
    QString everyName[200];
    QString mess;
    bool isLocked;
    bool isExamming;
    bool isLinked[200];
    int linkSum;
    int computerSum;
    int currentComputer;
    int mytimeID;
};

#endif // MAINWINDOW_H
