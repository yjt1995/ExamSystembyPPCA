#include "screenwindow.h"
#include "ui_screenwindow.h"

screenWindow::screenWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::screenWindow)
{
    ui->setupUi(this);
    img = new QImage();
    img->load(":/images/blackscreen");
    this->setCursor(Qt::BlankCursor);
}

screenWindow::~screenWindow()
{
    delete ui;
}

void screenWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    nImg = img->scaled(width(),height());
    painter.drawImage(0,0,nImg);
}

void screenWindow::changePicture()
{
    img->load(":images/blackscreen2");
}
