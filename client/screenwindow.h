#ifndef SCREENWINDOW_H
#define SCREENWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QImage>
#include <QPainter>

namespace Ui {
class screenWindow;
}

class screenWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit screenWindow(QWidget *parent = 0);
    ~screenWindow();

    QImage *img;
    QImage nImg;

    void changePicture();

protected:
    void paintEvent(QPaintEvent *);

private:
    Ui::screenWindow *ui;
};

#endif // SCREENWINDOW_H
