#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>

class Dashboard : public QWidget
{
    Q_OBJECT

    double value;

public:
    explicit Dashboard(QWidget *parent = 0);

private:
    void drawWidget(QPainter &qp);
    void drawScale(QPainter &qp, QPen pen, double ratio_o, double ratio_i,
                   double interval, double b_arc=-60.0, double e_arc=240.0 );
    void drawScaleDigit(QPainter &qp, int *data,
                        double interval=20, double b_arc=-60.0, double e_arc=240.0);
    void drawPointer(QPainter &qp);
protected:
    void paintEvent(QPaintEvent *e);

public slots:
    void setValue(double value);
};

#endif // DASHBOARD_H
