#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QWidget>

class TitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TitleWidget(QWidget *parent = 0);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *);

};

#endif // TITLEWIDGET_H
