#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "danmu.h"
#include <QTimer>
#include <QUrl>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDesktopWidget>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTimer *timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(pullDanmu()));
    timer->start();

    nam = new QNetworkAccessManager(this);
    QObject::connect(nam, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(getDanmuFinishedSlot(QNetworkReply*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pullDanmu() {
    int maxn = 32;
    QUrl url(QString("http://dsa.cs.tsinghua.edu.cn/danmaku/message.php?action=get_danmu_last&n=%1").arg(maxn));
    nam->get(QNetworkRequest(url));
}

void MainWindow::getDanmuFinishedSlot(QNetworkReply *reply) {
    // Reading attributes of the reply
    // e.g. the HTTP status code
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    // no error received?
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        QJsonParseError json_error;
        QJsonDocument parse_document = QJsonDocument::fromJson(bytes, &json_error);
        if (json_error.error == QJsonParseError::NoError) {
            QJsonObject jsonObject = parse_document.object();
            if (jsonObject["error"].toInt() == 0) {
                QJsonArray danmuSet = jsonObject["danmu_set"].toArray();
                for (int i = danmuSet.size() - 1; i >= 0; i--) {
                    QJsonObject danmu = danmuSet[i].toObject();
                    int messageId = danmu["message_id"].toInt();
                    QString text = danmu["text"].toString();
                    if (showedDanmu.find(messageId) == showedDanmu.end()) {
                        qDebug() << danmu;
                        showedDanmu.insert(messageId);
                        Danmu *danmu = new Danmu(this, text, "Red", 1, QApplication::desktop()->screenGeometry());
                    }
                }
            }
        }

    }
    // Some http error received
    else
    {
        // handle errors here
    }

    // We receive ownership of the reply object
    // and therefore need to handle deletion.
    reply->deleteLater();
}
