#include <QDesktopWidget>
#include <QFont>
#include <QPainter>
#include <QDebug>
#include "danmu.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"


Danmu::Danmu(MainWindow *parent) : QWidget(nullptr), parent(parent)
{
    setGeometry(0, 0, 640, 400);

    setWindowFlags(Qt::FramelessWindowHint|Qt::Tool|Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setFocusPolicy(Qt::NoFocus);
    setWindowOpacity(1.0);
    show();
}

void Danmu::setMessages(QVector<MessageItem> const &messages) {
    this->messages = messages;
    update();
}

void Danmu::paintEvent(QPaintEvent *) {
    setWindowOpacity((qreal)parent->ui->horizontalSliderOpacity->value() / (qreal)parent->ui->horizontalSliderOpacity->maximum());
    QRect rectScreen(QApplication::desktop()->screenGeometry());
    int w = (rectScreen.width() - 100) * parent->ui->horizontalSliderWidth->value() / parent->ui->horizontalSliderWidth->maximum() + 100;
    int h = (rectScreen.height() - 50) * parent->ui->horizontalSliderHeight->value() / parent->ui->horizontalSliderHeight->maximum() + 50;
    int x = (rectScreen.width() - w) * parent->ui->horizontalSliderLeft->value() / parent->ui->horizontalSliderLeft->maximum() + rectScreen.x();
    int y = (rectScreen.height() - h) * parent->ui->horizontalSliderUp->value() / parent->ui->horizontalSliderUp->maximum() + rectScreen.y();
    setGeometry(x, y, w, h);

    QPainter painter(this);

    painter.save();
    painter.setOpacity((qreal)parent->ui->horizontalSliderBackground->value() / (qreal)parent->ui->horizontalSliderBackground->maximum());
    painter.setPen(Qt::white);
    painter.setBrush(Qt::white);
    painter.drawRect(QRect(QPoint(0, 0), size()));
    painter.restore();

    int fontsize = parent->ui->horizontalSliderFontsize->value();
    QFont fontName("黑体", fontsize, 100, false);
    QFontMetrics metricsName(fontName);
    QFont fontText("黑体", fontsize, 0, false);
    QFontMetrics metricsText(fontText);

    int y_top = 0;
    int y_gap = 4;
    QString title = parent->ui->lineEditTitle->text();
    if (!title.isEmpty()) {
        int fz = fontsize;
        while (1) {
            QFont fontTitle("黑体", fz, 100, false);
            QFontMetrics metricsTitle(fontTitle);
            QSize szTitle(metricsTitle.size(Qt::TextSingleLine, title));
            if (fz > 12 && szTitle.width() > width()) {
                fz--;
                continue;
            }
            painter.setPen(Qt::black);
            painter.setFont(fontTitle);
            painter.drawText(QRect(0, y_top, width() - 1, szTitle.height() - 1), title);
            y_top += szTitle.height() + y_gap;
            break;
        }
    }

    for (MessageItem const &msg: messages) {
        QString filter(parent->ui->lineEditFilter->text().trimmed());
        QString mid = msg.text.trimmed().mid(0, filter.length());
        if (mid.length() < filter.length())
            continue;
        for (int i = 0; i < filter.size(); i++) {
            if (filter[i] == '?' || filter.mid(i, 1) == "？")
                mid[i] = filter[i];
        }
        if (mid.toLower() != filter.toLower())
            continue;
        QString filteredText = msg.text.trimmed().mid(filter.length()).trimmed();
        if (filteredText.isEmpty())
            continue;

        QString textUser = msg.user.mid(14, 4) + ": ";
        int seed = msg.user.mid(0, 7).toInt(nullptr, 16);
        QSize sz(metricsName.size(Qt::TextSingleLine, textUser));
        int lineHeight = sz.height();
        painter.setPen(QColor::fromHsv(seed % 360, seed / 360 % 64 + 192, seed / 360 / 64 % 64 + 192));
        painter.setFont(fontName);
        painter.drawText(QRect(0, y_top, sz.width() - 1, sz.height() - 1), textUser);
        QString textText = metricsText.elidedText(filteredText, Qt::ElideRight, width() - sz.width(), Qt::TextSingleLine);
        QSize szText(metricsText.size(Qt::TextSingleLine, textText));
        lineHeight = qMax(lineHeight, szText.height());
        painter.setPen(Qt::black);
        painter.setFont(fontText);
        painter.drawText(QRect(sz.width(), y_top, width() - sz.width(), szText.height() - 1), textText);
        y_top += lineHeight + y_gap;
        if (y_top > height())
            break;
    }
    painter.end();
}
