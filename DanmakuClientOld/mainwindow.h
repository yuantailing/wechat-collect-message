#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void pullDanmu();
    void getDanmuFinishedSlot(QNetworkReply *);

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *nam;
    QSet<int> showedDanmu;
};

#endif // MAINWINDOW_H
