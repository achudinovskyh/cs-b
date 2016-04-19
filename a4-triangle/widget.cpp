#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

// Method that draw a triangle
void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    // creating painter
    QPainter painter(this);
    // creating point to draw
    QPoint* pt = new QPoint;
    // creating pen and set if black
    QPen blackPen(Qt::black);
    painter.setPen(blackPen);

    int dotsCount  = 0;
    while(dotsCount < 1000000){
        // move point to random position
        moveRandomly(pt);
        // get x and y from point
        int x = pt->x();
        int y = pt->y();
        // and draw point with this cordinates
        painter.drawPoint(x,y);
        // increments counter
        dotsCount++;
    }

}

void Widget::moveRandomly(QPoint* pt){
    // chose random triangle corner
    QPoint temp = chooseRandomCorner();
    // add curren point cordinates to new cordinates of random corner and divide it by 2
    double newX = (pt->x() + temp.x()) / 2.0;
    double newY = (pt->y() + temp.y()) / 2.0;
    // setting new cordinates to point
    pt->setX(newX);
    pt->setY(newY);
}


QPoint Widget::chooseRandomCorner(){
    // cose the random corner
    int cornerIndex = qrand() % 3 + 1;
    if (cornerIndex == 1) {
        return QPoint(320, 0);
    } else if (cornerIndex == 2) {
        return QPoint(640, 480);
    } else {
        return  QPoint(0,480);
    }

}
