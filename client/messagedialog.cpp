#include "messagedialog.h"
#include "ui_messagedialog.h"

MessageDialog::MessageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageDialog)
{
    ui->setupUi(this);
    ui->plainTextEdit->appendPlainText("考试系统已经开启。");
    ui->plainTextEdit->appendPlainText("以下是教师端发来的信息：");
    connect(parent,SIGNAL(showTranslateMessage()),this,SLOT(show()));
    connect(parent,SIGNAL(appendMessage(QString)),this,SLOT(changeMessage(QString)));
}

MessageDialog::~MessageDialog()
{
    delete ui;
}

void MessageDialog::changeMessage(QString mess)
{
    ui->plainTextEdit->appendPlainText(mess);
}
