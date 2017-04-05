#include <QPainter>
#include <QDebug>
#include "dashboard.h"

Dashboard::Dashboard(QWidget *parent) : QWidget(parent),value(0)
{

}

void Dashboard::setValue(double value){
    this->value = value;
}

void Dashboard::paintEvent(QPaintEvent *e){
    Q_UNUSED(e)
    QPainter qp;
    qp.begin(this);
    this->drawWidget(qp);
    qp.end();
}

void Dashboard::drawWidget(QPainter &qp){
    // Set pen
    auto b_scale_pen = QPen();
    auto scale_pen = QPen();
    b_scale_pen.setWidth(2);
    scale_pen.setWidth(1);

    // Set font
    qp.setFont(QFont("Serif", 14, QFont::Light));

    // Antialiasing
    qp.setRenderHint(QPainter::Antialiasing);

    drawScale(qp, b_scale_pen,.9, .8, 20);     // Draw bold scale
    drawScale(qp, scale_pen,.9, .84, 2);        // Draw normal scale
    int data[16];
    for(int i=0; i<16;i++) data[i]=i*20;
    drawScaleDigit(qp,data);
    drawPointer(qp);
}

void Dashboard::drawScale(QPainter &qp, QPen pen, double ratio_o,
                          double ratio_i, double interval, double b_arc, double e_arc)
{
    qp.save();
    qp.setPen(pen);

    // Get window size
    auto size = this->size();
    auto w = size.width();
    auto h = size.height();
    auto radius = (w<h?w:h) / 2;

    // Init transfrom
    QTransform trans;
    trans.translate(w/2, h/2);
    trans.rotate(b_arc);
    qp.setTransform(trans);

    // Draw scale in a for-loop
    for(double arc = b_arc; arc <= e_arc; arc+=interval){
        qp.drawLine(QPointF(-radius*ratio_o, 0), QPointF(-radius *ratio_i, 0));
        trans.rotate(interval);
        qp.setTransform(trans);
    }
    qp.restore();
}

void Dashboard::drawScaleDigit(QPainter &qp, int *data,
                               double interval, double b_arc, double e_arc)
{
    qp.save();
    auto metrics = qp.fontMetrics();

    // Get window size
    auto size = this->size();
    auto w = size.width();
    auto h = size.height();
    auto radius = (w<h?w:h) / 2;

    // Init transfrom
    QTransform trans;
    trans.translate(w/2,h/2);
    trans.rotate(b_arc-90);

    qp.setTransform(trans);
    int idx = 0;
    //int n = static_cast<int>((e_arc - b_arc)/interval) + 1;
    int fw;
    for(double arc = b_arc; arc <= e_arc; arc+=interval){
        fw = metrics.width(QString::number(data[idx]));
        qp.drawText(QPointF(-fw / 2, -radius * .92),QString::number(data[idx]));
        trans.rotate(interval);
        qp.setTransform(trans);
        idx++;
    }
    qp.restore();
}

void Dashboard::drawPointer(QPainter &qp)
{
    qp.save();

    // Get window size
    auto size = this->size();
    auto w = size.width();
    auto h = size.height();
    auto radius = (w<h?w:h) / 2;

    QTransform trans;
    trans.translate(w/2,h/2);
    qp.setTransform(trans);
    qp.save();
    trans.rotate(-60 + value);
    qp.setTransform(trans);
    qp.setBrush(QColor(75, 110, 175));
    qp.setPen(Qt::NoPen);
    QPoint points[3] = {QPoint(0, 10), QPoint(0, -10), QPoint(-radius*.8, 0)};
    qp.drawPolygon(points, 3);
    qp.drawEllipse(-30,-30,60,60);
    qp.setBrush(QColor(233,201,202));
    qp.drawEllipse(-25,-25,50,50);
    qp.restore();
    qp.setFont(QFont("Serif", 24, QFont::Bold));
    auto rect = QRect(-50, -50, 100, 100);
    qp.drawText(rect, Qt::AlignCenter, QString::number(value));
    trans.reset();
    trans.translate(w/2, h/2+radius *.5);
    qp.setTransform(trans);
    qp.drawText(rect, Qt::AlignCenter, "km/h");
    qp.restore();

}
