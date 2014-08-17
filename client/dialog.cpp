#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    w2=new MessageDialog(this);
    w3=new screenWindow(this);
    fire=new spiFirewall(this);
    isLocked=false;
    isExamming=false;
    connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(hide()));
    connect(ui->toolButton,SIGNAL(clicked()),this,SLOT(selectFiles()));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(submitFiles()));
    CreatTrayIcon();
    stu = new StuServer(this);
    stu->listen(QHostAddress::Any,14423);
    connect(stu,SIGNAL(ipgot(QString)),this,SLOT(gotIP(QString)));
    connect(stu,SIGNAL(testStart()),this,SLOT(beginExam()));
    connect(stu,SIGNAL(testFinished()),this,SLOT(endExam()));
    connect(stu,SIGNAL(lockScreen()),this,SLOT(lockScreen()));
    connect(stu,SIGNAL(unlockScreen()),this,SLOT(unlockScreen()));
    connect(stu,SIGNAL(sendMessage(QString)),this,SLOT(recieveMessage(QString)));
    connect(this,SIGNAL(submitFile(QStringList)),stu,SLOT(changeFiles(QStringList)));
    //
    //Sleep(10000);
    //w3->hide();
    //w3->setWindowFlags(Qt::SubWindow);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::CreatTrayMenu()
{
    messageAction = new QAction("消 息(&M)",this);
    submitAction = new QAction("提 交(&S)",this);
    aboutAction = new QAction("关 于(&A)",this);
    quitAction = new QAction("退 出(&Q)",this);

    connect(messageAction,SIGNAL(triggered()),this,SIGNAL(showTranslateMessage()));
    connect(submitAction,SIGNAL(triggered()),this,SLOT(show()));
    connect(aboutAction,SIGNAL(triggered()),this,SLOT(showAbout()));
    connect(quitAction,SIGNAL(triggered()),this,SLOT(quitWithMyWarning()));

    myMenu = new QMenu((QWidget*)QApplication::desktop());

    myMenu->addAction(messageAction);
    myMenu->addAction(submitAction);
    myMenu->addAction(aboutAction);
    myMenu->addSeparator();     //加入一个分离符
    myMenu->addAction(quitAction);
}

void Dialog::CreatTrayIcon()
{
    CreatTrayMenu();

    if (!QSystemTrayIcon::isSystemTrayAvailable())      //判断系统是否支持系统托盘图标
    {
        return;
    }

    myTrayIcon = new QSystemTrayIcon(this);

    myTrayIcon->setIcon(QIcon(":/images/computerIcon"));   //设置图标图片

    myTrayIcon->setToolTip("考试系统客户端");    //托盘时，鼠标放上去的提示信息

    myTrayIcon->setContextMenu(myMenu);     //设置托盘上下文菜单

    myTrayIcon->show();

    connect(myTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void Dialog::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Trigger:
        break;
    case QSystemTrayIcon::DoubleClick:
        show();
        break;

    default:
        break;
    }
}

void Dialog::selectFiles()
{
    QFileDialog *fd=new QFileDialog(this);
    fd->setFileMode(QFileDialog::ExistingFiles);
    if(fd->exec()==QFileDialog::Accepted)
    {
        slist=fd->selectedFiles();
        QString filesString="";
        for (QStringList::Iterator it=slist.begin();it!=slist.end();it++)
            filesString=filesString+"\""+(*it)+"\"";
        ui->lineEdit->setText(filesString);
    }
}

void Dialog::submitFiles()
{
    for (QStringList::Iterator it=slist.begin();it!=slist.end();it++)
    {
        bool tt=false;
        for (QStringList::Iterator it2=slist2.begin();it2!=slist2.end();it2++)
            if ((*it)==(*it2))
            {
                tt=true;
                break;
            }
        if (!tt)
        {
            slist2.append(*it);
        }
    }
    emit submitFile(slist2);
    ui->lineEdit->setText("");
}

void Dialog::quitWithMyWarning()
{
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this, tr("退出考试系统"),QString(tr("您真的需要退出吗?")),QMessageBox::Yes | QMessageBox::No);

    if (button == QMessageBox::Yes)
        qApp->quit();
        else hide();
}

void Dialog::showAbout()
{
    QMessageBox::about(this, "关于考试系统", "这是一个由ACM班姚京韬同学和郑宜霖同学制作的考试系统。");
}

void Dialog::closeEvent(QCloseEvent *event)
{
    if (myTrayIcon->isVisible())
    {
        ui->lineEdit->setText("");
        hide();     //最小化
        event->ignore();
    }
    else event->accept();  //接受退出信号，程序退出
}

void Dialog::beginExam()
{
    w3->hide();
    isExamming=true;
    emit appendMessage("本机考试已开始。");
}

void Dialog::endExam()
{
    isExamming=false;
    w3->changePicture();
    w3->showFullScreen();
    emit appendMessage("本机考试已结束。");
}

void Dialog::lockScreen()
{
    w3->setWindowFlags(Qt::Window);
    w3->showFullScreen();
    fire->initFirewall();
    isLocked=true;
    emit appendMessage("本机已锁屏。");
}

void Dialog::unlockScreen()
{
    isLocked=false;
    fire->deleteFirewall();
    w3->hide();
    emit appendMessage("本机已结束锁屏。");
}

void Dialog::gotIP(QString comIP)
{
    QFile file("mainIP.log");
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream fout(&file);
        fout<<comIP;
    }
    qDebug()<<file.errorString();
    file.close();
}

void Dialog::recieveMessage(QString mess)
{
    emit appendMessage("接受到教师机消息："+mess);
}
