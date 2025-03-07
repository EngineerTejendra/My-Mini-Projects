#ifndef CLOCKDIAL_H
#define CLOCKDIAL_H

#include <QGraphicsItem>
#include <QPainter>
#include <QPen>
#include <QFont>
#include <QObject>

struct DialData{
    float radius;
    float angle;
    QPointF mousePos;
};

class ClockDial :public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    ClockDial();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    float getDialRadius();
    float setDialRadius(float rad);

    float getAngle();
    float setAngle(float angle);

    QPointF getMousePosition();
    QPointF setMousePosition(QPointF pos);

signals:
    void radiusUpdate();
    void angleUpdated();
    void MousePosUpdated();


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    void drawDial(QPainter &painter);
    void drawNumbers(QPainter &painter);
    void drawTicks(QPainter &painter);
    void drawLineItem(QPainter &painter);
    void drawCircle(QPainter &painter);

    double lineAngle; // Angle of the needle
    bool dragging; // Indicates if the needle is being dragged
    bool resizing; // Indicates if the circle is being resized
    QPointF lastMousePos; // Last mouse position for dragging
    double circleRadius; // Radius of the adjustable circle

    DialData *d;
};

#endif // CLOCKDIAL_H
