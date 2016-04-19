#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPoint>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    // virtual methhod that draw on widget
    void paintEvent(QPaintEvent *event);


private:
    Ui::Widget *ui;
    // method that calculate cordinates for point
    void moveRandomly(QPoint* pt);
    // method that chose random corner of triangle
    QPoint chooseRandomCorner();
};

#endif // WIDGET_H
