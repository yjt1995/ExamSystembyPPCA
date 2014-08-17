#include "messagedialog.h"
#include "ui_messagedialog.h"

MessageDialog::MessageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageDialog)
{
    ui->setupUi(this);

    connect(parent,SIGNAL(showMessage(QString &)),this,SLOT(showWithMessage(QString &)));
    //connect(tcpclient,SIGNAL(recieveMessage(Qstring me)),this,SLOT(setMessage(Qstring))));
}

MessageDialog::~MessageDialog()
{
    delete ui;
}

void MessageDialog::showWithMessage(QString &mess)
{
    ui->plainTextEdit->setPlainText(mess);
    show();
}
