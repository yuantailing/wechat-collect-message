#ifndef DANMU_H
#define DANMU_H

#include <QWidget>
#include <QVector>

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

class MainWindow;

class Danmu : public QWidget
{
    Q_OBJECT
public:
    explicit Danmu(MainWindow *parent);

    struct MessageItem {
        MessageItem(): user(""), text("") { }
        MessageItem(QString const &user, QString const &text): user(user), text(text) { }
        QString user;
        QString text;
    };
    void setMessages(QVector<MessageItem> const &messages);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *);

private:
    MainWindow *parent;
    QVector<MessageItem> messages;
};

#endif // DANMU_H
