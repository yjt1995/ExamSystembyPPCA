#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    w2=new MessageDialog(this);
    isLocked=false;
    isExamming=false;

    connect(ui->beginButton,SIGNAL(clicked()),this,SLOT(beginWithWarning()));
    connect(ui->endButton,SIGNAL(clicked()),this,SLOT(endWithWarning()));
    connect(ui->lockButton,SIGNAL(clicked()),this,SLOT(lockWithWarning()));
    connect(ui->unlockButton,SIGNAL(clicked()),this,SLOT(unlockWithWarning()));

    CreatTrayIcon();

    beginAction = new QAction("开始考试(&B)",this);
    connect(beginAction,SIGNAL(triggered()),this,SLOT(beginWithWarning()));

    endAction = new QAction("结束考试(&E)",this);
    connect(endAction,SIGNAL(triggered()),this,SLOT(endWithWarning()));

    lockAction = new QAction("开始锁屏(&L)",this);
    connect(lockAction,SIGNAL(triggered()),this,SLOT(lockWithWarning()));

    unlockAction = new QAction("结束锁屏(&U)",this);
    connect(unlockAction,SIGNAL(triggered()),this,SLOT(unlockWithWarning()));

    postMessageAction = new QAction("发布消息(&P)",this);
    connect(postMessageAction,SIGNAL(triggered()),this,SLOT(postMessage()));

    postSingleMessageAction = new QAction("发送单人消息(&S)",this);
    connect(postSingleMessageAction,SIGNAL(triggered()),this,SLOT(postSingleMessage()));

    messageRecordAction = new QAction("消息记录(&M)",this);
    connect(messageRecordAction,SIGNAL(triggered()),this,SLOT(messageRecord()));

    singleMessageAction = new QAction("单人消息记录(&E)",this);
    connect(singleMessageAction,SIGNAL(triggered()),this,SLOT(singleMessage()));

    linkStateAction = new QAction("总体链接状态(&L)",this);
    connect(linkStateAction,SIGNAL(triggered()),this,SLOT(showLinkState()));

    singleStateAction = new QAction("单人链接状态(&S)",this);
    connect(singleStateAction,SIGNAL(triggered()),this,SLOT(showSingleState()));

    relinkAction = new QAction("单机重连(&R)",this);
    connect(relinkAction,SIGNAL(triggered()),this,SLOT(singleRelink()));

    stopLinkAction = new QAction("断开链接(&P)",this);
    connect(stopLinkAction,SIGNAL(triggered()),this,SLOT(stopLink()));

    basicMenu = menuBar()->addMenu("基本操作(&B)");
    basicMenu->addAction(beginAction);
    basicMenu->addAction(endAction);
    basicMenu->addAction(lockAction);
    basicMenu->addAction(unlockAction);

    messageMenu = menuBar()->addMenu("消息(&M)");
    messageMenu->addAction(postMessageAction);
    messageMenu->addAction(postSingleMessageAction);
    messageMenu->addAction(messageRecordAction);
    messageMenu->addAction(singleMessageAction);

    linkMenu = menuBar()->addMenu("链接(&L)");
    linkMenu->addAction(linkStateAction);
    linkMenu->addAction(singleStateAction);
    linkMenu->addAction(relinkAction);
    linkMenu->addAction(stopLinkAction);

    aboutMenu = menuBar()->addMenu("关于(&A)");
    aboutMenu->addAction(aboutAction);

    rightMessageAction = new QAction("消息记录",this);
    rightStateAction = new QAction("连接状态",this);
    rightLinkAction = new QAction("重新连接",this);
    rightUnlinkAction = new QAction("断开连接",this);
    rightMenu = new QMenu(this);
    rightMenu->addAction(rightMessageAction);
    rightMenu->addAction(rightStateAction);
    rightMenu->addAction(rightLinkAction);
    rightMenu->addAction(rightUnlinkAction);
    connect(rightMessageAction,SIGNAL(triggered()),this,SLOT(rightMessage()));
    connect(rightStateAction,SIGNAL(triggered()),this,SLOT(rightState()));
    connect(rightLinkAction,SIGNAL(triggered()),this,SLOT(rightLink()));
    connect(rightUnlinkAction,SIGNAL(triggered()),this,SLOT(rightUnlink()));

    ui->plainTextEdit->appendPlainText("考试系统已经开启。");
    ui->plainTextEdit->appendPlainText("以下是学生机的登陆情况：");
    allMessage="考试系统已经开启。\n以下是学生机的登陆情况：\n";

    computerSum=0;
    QFile file("E:\\Qt-ceshi\\server-ceshi\\data.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream fin(&file);
        fin>>computerSum;
        //computerSum=10;
        linkSum=0;
        //std::cout<<computerSum<<std::endl;
        for (int i = 0; i < computerSum; i++)
        {
            fin>>everyIP[i];
            //everyIP[i]="192.168.201."+everyIP[i].setNum(i+90);
            //std::cout<<s<<std::endl;
            //everyIP[i] = QString::fromStdString(s);
            fin>>everyName[i];
            //everyName[i]="A"+everyName[i].setNum(i);
            //std::cout<<computerSum<<std::endl;
            //everyName[i] = QString::fromStdString(s);
            everyMessage[i]="考试系统已经开启。\n";
            isLinked[i]=false;
        }
    }
    file.close();

    ui->listView->setViewMode(QListView::IconMode);
    ui->listView->setIconSize(QSize(80,80));
    ui->listView->setGridSize(QSize(100,100));
    ui->listView->setMovement(QListView::Static);
    computerGroup = new QStandardItemModel(ui->listView);
    for (int i = 0; i < computerSum; i++)
    {
        computerItem[i] = new QStandardItem();
        computerItem[i]->setText(everyName[i]);
        computerItem[i]->setToolTip(everyName[i]);
        computerItem[i]->setFont(QFont("微软雅黑",10,1));
        computerItem[i]->setIcon(QIcon(":/images/unlinked"));
        computerGroup->appendRow(computerItem[i]);
    }
    ui->listView->setModel(computerGroup);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CreatTrayMenu()
{
    messageAction = new QAction("消 息(&M)",this);
    showAction = new QAction("还 原(&R)",this);
    aboutAction = new QAction("关 于(&A)",this);
    quitAction = new QAction("退 出(&Q)",this);

    connect(messageAction,SIGNAL(triggered()),this,SLOT(showAllMessage()));
    connect(showAction,SIGNAL(triggered()),this,SLOT(show()));
    connect(aboutAction,SIGNAL(triggered()),this,SLOT(showAbout()));
    connect(quitAction,SIGNAL(triggered()),this,SLOT(quitWithMyWarning()));

    myMenu = new QMenu((QWidget*)QApplication::desktop());

    myMenu->addAction(messageAction);
    myMenu->addAction(showAction);
    myMenu->addAction(aboutAction);
    myMenu->addSeparator();     //加入一个分离符
    myMenu->addAction(quitAction);
}

void MainWindow::CreatTrayIcon()
{
    CreatTrayMenu();

    if (!QSystemTrayIcon::isSystemTrayAvailable())      //判断系统是否支持系统托盘图标
    {
        return;
    }

    myTrayIcon = new QSystemTrayIcon(this);

    myTrayIcon->setIcon(QIcon(":/images/computerIcon"));   //设置图标图片

    myTrayIcon->setToolTip("考试系统教师端");    //托盘时，鼠标放上去的提示信息

    myTrayIcon->setContextMenu(myMenu);     //设置托盘上下文菜单

    myTrayIcon->show();

    connect(myTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
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

void MainWindow::quitWithMyWarning()
{
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this, tr("退出考试系统"),QString(tr("您真的需要退出吗?")),QMessageBox::Yes | QMessageBox::No);

    if (button == QMessageBox::Yes)
        qApp->quit();
        else hide();
}

void MainWindow::beginWithWarning()
{
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this, tr("开始考试"),QString(tr("您确定开始考试吗?")),QMessageBox::Yes | QMessageBox::No);

    if (button == QMessageBox::Yes)
        beginExam();
}

void MainWindow::endWithWarning()
{
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this, tr("结束考试"),QString(tr("您确定结束考试吗?")),QMessageBox::Yes | QMessageBox::No);

    if (button == QMessageBox::Yes)
        endExam();
}

void MainWindow::lockWithWarning()
{
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this, tr("开始锁屏"),QString(tr("您确定开始锁屏吗?")),QMessageBox::Yes | QMessageBox::No);

    if (button == QMessageBox::Yes)
        lockExam();
}

void MainWindow::unlockWithWarning()
{
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this, tr("结束锁屏"),QString(tr("您确定结束锁屏吗?")),QMessageBox::Yes | QMessageBox::No);

    if (button == QMessageBox::Yes)
        unlockExam();
}

void MainWindow::beginExam()
{
    isExamming=true;
    for (int i = 0; i < computerSum; i++)
        if(isLinked[i])
        {
            teaClient(everyIP[i],"testStart","",i);

        }
}

void MainWindow::endExam()
{
    for (int i = 0; i < computerSum; i++)
        if(isLinked[i])
        {
            teaClient(everyIP[i],"testFinished","",i);
        }
    isExamming=false;
    bool isOK;
    QString message = QInputDialog::getText(this,"收回文件路径","请输入收回文件路径：",QLineEdit::Normal,"file dir",&isOK);
    if (!isOK)
    {
        message="D:\\testWorkSpace\\";
    }
    for (int i = 0; i < computerSum; i++)
        if(isLinked[i])
        {

            teaClient(everyIP[i],"sendFile",message+everyName[i]+"\\",i);
        }
}

void MainWindow::lockExam()
{

    for (int i = 0; i < computerSum; i++)
    {
        teaClient(everyIP[i],"lockScreen","",i);

    }
    isLocked=true;
    mytimeID = startTimer(30000);

}
void MainWindow::unlockExam()
{
    isLocked=false;
    for (int i = 0; i < computerSum; i++)
        if(isLinked[i])
        {
            teaClient(everyIP[i],"unlockScreen","",i);

        }
}

void MainWindow::postMessage()
{
    bool isOK;
    QString message = QInputDialog::getText(this,"发布消息","请输入要发布的消息：",QLineEdit::Normal,"your message",&isOK);
    if (isOK)
    {
        emit translateMessage(-1,message);
    }
}

void MainWindow::postSingleMessage()
{
    bool isOK;
    QString name = QInputDialog::getText(this,"发送单人消息","请输入要发送消息的机器编号：",QLineEdit::Normal,"computer name",&isOK);
    if (isOK)
    {
        QString message = QInputDialog::getText(this,"发送单人消息","请输入要发送的消息：",QLineEdit::Normal,"your message",&isOK);
        if (isOK)
        {
            for (int i = 0; i < computerSum; i++)
                if (everyName[i]==name)
                {
                    emit translateMessage(i,message);
                    break;
                }
        }
    }
}

void MainWindow::messageRecord()
{
    emit showMessage(allMessage);
}

void MainWindow::singleMessage()
{
    bool isOK;
    QString name = QInputDialog::getText(this,"单人消息记录","请输入要查看消息记录的机器编号：",QLineEdit::Normal,"computer name",&isOK);
    if (isOK)
    {
        for (int i = 0; i < computerSum; i++)
            if (everyName[i]==name)
            {
                emit showMessage(everyMessage[i]);
                break;
            }
    }
}

void MainWindow::showLinkState()
{
    QString s,s2;
    mess = "登记机器数："+mess.setNum(computerSum)+", 已连接机器数："+s.setNum(linkSum)+", 未连接机器数："+s2.setNum(computerSum-linkSum)+"\n";
    mess += "未连接的机器有：\n";
    for (int i = 0; i < computerSum; i++)
        if (!isLinked[i])
            mess += everyName[i] + " ";
    emit showMessage(mess);
}

void MainWindow::showSingleState()
{
    bool isOK;
    QString name = QInputDialog::getText(this,"单人连接状态","请输入要查看连接状态的机器编号：",QLineEdit::Normal,"computer name",&isOK);
    if (isOK)
    {
        for (int i = 0; i < computerSum; i++)
            if (everyName[i]==name)
            {
                if (isLinked[i])
                    QMessageBox::about(this, "单人连接状态", name+"号机的连接状态为：已连接上");
                    else QMessageBox::about(this, "单人连接状态", name+"号机的连接状态为：断开");
                break;
            }
    }
}

void MainWindow::singleRelink()
{
    bool isOK;
    QString name = QInputDialog::getText(this,"单机重连","请输入要重新连接的机器编号：",QLineEdit::Normal,"computer name",&isOK);
    if (isOK)
    {
        for (int i = 0; i < computerSum; i++)
            if (everyName[i]==name)
            {
                if (isLinked[i])
                    QMessageBox::about(this, "单人重连", name+"号机的连接状态为：已连接上");
                    else
                    {
                        teaClient(everyIP[i],"checkLink","",i);
                        /*
                        if (isLinked[i])
                            QMessageBox::about(this, "单人重连", "经过重连，"+name+"号机的连接状态为：已连接上");
                            else QMessageBox::about(this, "单人重连", "经过重连，"+name+"号机的连接状态仍为：断开");
                        */

                    }
                break;
            }
    }
}

void MainWindow::stopLink()
{
    bool isOK;
    QString name = QInputDialog::getText(this,"断开链接","请输入要断开链接的机器编号：",QLineEdit::Normal,"computer name",&isOK);
    if (isOK)
    {
        for (int i = 0; i < computerSum; i++)
            if (everyName[i]==name)
            {
                if (!isLinked[i])
                    QMessageBox::about(this, "断开链接", name+"号机的连接状态为：断开");
                    else
                    {
                        isLinked[i]=false;
                        if (isLinked[i])
                            QMessageBox::about(this, "断开链接", name+"号机断开未成功");
                            else QMessageBox::about(this, "断开链接", name+"号机的连接已被断开");
                    }
                break;
            }
    }
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, "关于考试系统", "这是一个由ACM班姚京韬同学和郑宜霖同学制作的考试系统。");
}

void MainWindow::showAllMessage()
{
    emit showMessage(allMessage);
}

void MainWindow::rightLink()
{
    if (isLinked[currentComputer])
        QMessageBox::about(this, "单人重连", everyName[currentComputer]+"号机的连接状态为：已连接上");
        else
        {
            teaClient(everyIP[currentComputer],"checkLink","",currentComputer);
            /*
            if (isLinked[currentComputer])
                QMessageBox::about(this, "单人重连", "经过重连，"+everyName[currentComputer]+"号机的连接状态为：已连接上");
                else QMessageBox::about(this, "单人重连", "经过重连，"+everyName[currentComputer]+"号机的连接状态仍为：断开");
            */
        }
}

void MainWindow::rightState()
{
    if (isLinked[currentComputer])
        QMessageBox::about(this, "单人连接状态", everyName[currentComputer]+"号机的连接状态为：已连接上");
        else QMessageBox::about(this, "单人连接状态", everyName[currentComputer]+"号机的连接状态为：断开");
}

void MainWindow::rightUnlink()
{

    if (!isLinked[currentComputer])
        QMessageBox::about(this, "断开链接", everyName[currentComputer]+"号机的连接状态为：断开");
        else
        {
            isLinked[currentComputer]=false;
            if (isLinked[currentComputer])
                QMessageBox::about(this, "断开链接", everyName[currentComputer]+"号机断开未成功");
                else QMessageBox::about(this, "断开链接", everyName[currentComputer]+"号机的连接已被断开");
        }
}

void MainWindow::rightMessage()
{
    emit showMessage(everyMessage[currentComputer]);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (myTrayIcon->isVisible())
    {
        hide();     //最小化
        event->ignore();
    }
    else event->accept();  //接受退出信号，程序退出
}

void MainWindow::on_listView_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex ind = ui->listView->indexAt(pos);
    QVariant var = ind.data(Qt::ToolTipRole);
    if (var.isValid())
    {
        QString name = var.toString();
        for (int i = 0; i < computerSum; i++)
            if (everyName[i]==name)
            {
                currentComputer=i;
                rightMenu->exec(QCursor::pos());
            }
    }
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    QVariant var = index.data(Qt::ToolTipRole);
    if (var.isValid())
    {
        QString name = var.toString();
        for (int i = 0; i < computerSum; i++)
            if (everyName[i]==name)
            {
                currentComputer=i;
                rightState();
            }
    }
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId()==mytimeID)
    {
        for (int i = 0; i < computerSum; i++)
            if(isLinked[i])
            {
                teaClient(everyIP[i],"checkLink","",i);
            }
    }
}

void MainWindow::teaClient(QString ipAdr, QString command, QString arg, int num)
{
    TeaClientThread *tct=new TeaClientThread(ipAdr,command,arg,num,this);
    connect(tct,SIGNAL(error(int)),this,SLOT(error(int)));
    connect(tct,SIGNAL(succeed(int,QString)),this,SLOT(succ(int,QString)));
    connect(tct,SIGNAL(finished()),tct,SLOT(deleteLater()));
    //connect(tct,SLOT(finished(),tct,SLOT(finish(i))))
    tct->run();
}

void MainWindow::error(int num)
{
    if (isLinked[num])
    {
        isLinked[num] = false;
        computerItem[num]->setIcon(QIcon(":/images/unlinked"));
        linkSum--;
    }
    if (isLocked && !isExamming)
    {
        everyMessage[num] += everyName[num]+"号机收文件失败\n";
        allMessage += everyName[num]+"号机收文件失败\n";
        ui->plainTextEdit->appendPlainText(everyName[num]+"号机收文件失败");
    }
    else
    {
        everyMessage[num] += everyName[num]+"号机连接失败\n";
        allMessage += everyName[num]+"号机连接失败\n";
        ui->plainTextEdit->appendPlainText(everyName[num]+"号机连接失败");
    }
}

void MainWindow::succ(int num, QString command)
{
    if (!isLinked[num])
    {
        linkSum++;
        isLinked[num] = true;
        computerItem[num]->setIcon(QIcon(":/images/linked"));
    }
    if (command=="sendFile")
    {
        everyMessage[num] += everyName[num]+"号机收文件成功\n";
        allMessage += everyName[num]+"号机收文件成功\n";
        ui->plainTextEdit->appendPlainText(everyName[num]+"号机收文件成功");
        return;
    }
    if (command=="lockScreen")
    {
        everyMessage[num] += everyName[num]+"号机锁屏成功\n";
        allMessage += everyName[num]+"号机锁屏成功\n";
        ui->plainTextEdit->appendPlainText(everyName[num]+"号机锁屏成功");
        return;
    }
    if (command=="unlockScreen")
    {
        everyMessage[num] += everyName[num]+"号机屏幕解锁成功\n";
        allMessage += everyName[num]+"号机屏幕解锁成功\n";
        ui->plainTextEdit->appendPlainText(everyName[num]+"号机屏幕解锁成功");
        return;
    }
    if (command=="testStart")
    {
        everyMessage[num] += everyName[num]+"号机成功开始考试\n";
        allMessage += everyName[num]+"号机成功开始考试\n";
        ui->plainTextEdit->appendPlainText(everyName[num]+"号机成功开始考试");
        return;
    }
    if (command=="testFinished")
    {
        everyMessage[num] += everyName[num]+"号机成功结束考试\n";
        allMessage += everyName[num]+"号机成功结束考试\n";
        ui->plainTextEdit->appendPlainText(everyName[num]+"号机成功结束考试");
        return;
    }
    if (command=="checkLink") return;
    everyMessage[num] += "向"+everyName[num]+"号机成功发送消息："+command+"\n";
    allMessage += "向"+everyName[num]+"号机成功发送消息："+command+"\n";
    ui->plainTextEdit->appendPlainText("向"+everyName[num]+"号机成功发送消息："+command);

}
