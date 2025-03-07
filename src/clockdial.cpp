#include "clockdial.h"
#include <QPainter>
#include <QPen>
#include <QFontMetrics>
#include <QGraphicsSceneMouseEvent>
#include <cmath>
#include <QtMath>
#include <QtDebug>

ClockDial::ClockDial()
    : lineAngle(0), dragging(false), resizing(false), circleRadius(100)
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);

    d = new DialData();
}

QRectF ClockDial::boundingRect() const
{
    // Return the bounding rectangle for the item
    return QRectF(0, 0, 400, 400); // Adjust size as needed
}

void ClockDial::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    // Draw the dial
    drawDial(*painter);
    drawTicks(*painter);
    drawNumbers(*painter);
    drawLineItem(*painter);
    drawCircle(*painter);
}

float ClockDial::getDialRadius()
{
    return d->radius;
}

float ClockDial::setDialRadius(float rad)
{
    emit radiusUpdate();
    d->radius = rad;
}

float ClockDial::getAngle()
{
    return d->angle;
}

float ClockDial::setAngle(float angle)
{
    emit angleUpdated();
    d->angle = angle;
}

QPointF ClockDial::getMousePosition()
{
    return d->mousePos;
}

QPointF ClockDial::setMousePosition(QPointF pos)
{
    emit MousePosUpdated();
    d->mousePos = pos;
}

void ClockDial::drawDial(QPainter &painter)
{
    painter.setPen(QPen(Qt::black, 2));
    painter.drawEllipse(QRectF(10, 10, 380, 380)); // Adjust for the bounding rectangle
}

void ClockDial::drawTicks(QPainter &painter)
{
    int radius = 190; // Radius adjusted for the bounding rectangle
    QPointF center(200, 200); // Center of the bounding rectangle

    painter.setPen(QPen(Qt::black, 1));

    // Draw large ticks at every 10 degrees
    for (int angle = 0; angle < 360; angle += 10) {
        double rad = qDegreesToRadians(static_cast<double>(angle));
        int x1 = center.x() + radius * cos(rad);
        int y1 = center.y() - radius * sin(rad);
        int x2 = center.x() + (radius - 20) * cos(rad);
        int y2 = center.y() - (radius - 20) * sin(rad);
        painter.drawLine(x1, y1, x2, y2);
    }

    // Draw small ticks at every degree
    painter.setPen(QPen(Qt::black, 0.5));
    for (int angle = 0; angle < 360; angle += 1) {
        if (angle % 10 != 0) { // Skip large ticks
            double rad = qDegreesToRadians(static_cast<double>(angle));
            int x1 = center.x() + radius * cos(rad);
            int y1 = center.y() - radius * sin(rad);
            int x2 = center.x() + (radius - 10) * cos(rad);
            int y2 = center.y() - (radius - 10) * sin(rad);
            painter.drawLine(x1, y1, x2, y2);
        }
    }
}

void ClockDial::drawNumbers(QPainter &painter)
{
    int radius = 180; // Radius adjusted for the bounding rectangle
    int textOffset = 30; // Offset to place the text outside the circumference
    QPointF center(200, 200); // Center of the bounding rectangle
    QFont font("Arial", 8);
    painter.setFont(font);
    painter.setPen(Qt::black);

    // Calculate the number's bounding rect to adjust its alignment
    QFontMetrics fm(font);

    // Draw numbers every 5 degrees in a clockwise direction
    for (int angle = 0; angle < 360; angle += 10) {
        double rad = qDegreesToRadians(static_cast<double>(-angle+90)); // Negative to rotate clockwise

        // Calculate the position for the text
        int x = center.x() + (radius + textOffset) * cos(rad);
        int y = center.y() - (radius + textOffset) * sin(rad);

        // Prepare the text
        QString numberText = QString::number(angle);

        // Calculate the text bounding rectangle
        QRect textRect = fm.boundingRect(numberText);

        // Adjust the position to center the text
        int textX = x - textRect.width() / 2;
        int textY = y + textRect.height() / 2;

        // Draw the text
        painter.drawText(textX, textY, numberText);
    }
}

void ClockDial::drawLineItem(QPainter &painter)
{
    QPointF center(200, 200); // Center of the bounding rectangle
    int length = 150; // Length of the needle

    painter.setPen(QPen(Qt::red, 2)); // Set color and width for the needle
    painter.drawLine(center, QPointF(center.x() + length * cos(qDegreesToRadians(lineAngle)), center.y() - length * sin(qDegreesToRadians(lineAngle))));
}

void ClockDial::drawCircle(QPainter &painter)
{
    QPointF center(200, 200); // Center of the bounding rectangle
    painter.setPen(QPen(Qt::blue, 2)); // Set color and width for the circle
    painter.drawEllipse(center, circleRadius, circleRadius);
}

void ClockDial::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Check if clicking near the circle's circumference to start resizing
    QPointF center(200, 200); // Center of the bounding rectangle
    double distance = QLineF(center, event->scenePos()).length();
    if (distance >= circleRadius - 10 && distance <= circleRadius + 10) {
        resizing = true;
    } else {
        dragging = true;
        lastMousePos = event->scenePos();
        setMousePosition(lastMousePos);
    }
}

void ClockDial::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (dragging) {
        QPointF center(200, 200); // Center of the bounding rectangle
        QPointF newPos = event->scenePos();
        double angle = atan2(center.y() - newPos.y(), newPos.x() - center.x()) * 180 / M_PI;
        if (angle < 0) {
            angle += 360;
        }
        lineAngle = angle;
        setAngle(lineAngle);
        update(); // Request a repaint
        lastMousePos = newPos;
    } else if (resizing) {
        QPointF center(200, 200); // Center of the bounding rectangle
        double newRadius = QLineF(center, event->scenePos()).length();
        circleRadius = qMax(newRadius, 10.0); // Set a minimum radius
        setDialRadius(circleRadius);
        update(); // Request a repaint
    }
}

void ClockDial::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    dragging = false;
    resizing = false;
}
