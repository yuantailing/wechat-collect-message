#include <QTimer>
#include <QUrl>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QColorDialog>
#include <QSettings>
#include <QDebug>
#include <algorithm>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "danmu.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButtonColor->setStyleSheet(QString("background-color: rgb(0,0,255);"));

    configFilename = "config.ini";
    configLoading = configDirty = false;
    danmu = new Danmu(this);

    nam = new QNetworkAccessManager(this);
    QObject::connect(nam, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(getDanmuFinishedSlot(QNetworkReply*)));

    QTimer *timer = new QTimer(this);
    timer->setInterval(2000);
    connect(timer, SIGNAL(timeout()), this, SLOT(pullDanmu()));
    timer->start();

    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(saveConfig()));
    timer->start();

    loadConfig();
    pullDanmu();
}

MainWindow::~MainWindow()
{
    delete danmu;
    delete ui;
}

void MainWindow::pullDanmu() {
    int maxsec = ui->spinBoxTime->value();
    QUrl url(QString("http://dsa.cs.tsinghua.edu.cn/danmaku/message.php?action=get_danmu_recent&seconds=%1").arg(maxsec));
    nam->get(QNetworkRequest(url));
}

void MainWindow::getDanmuFinishedSlot(QNetworkReply *reply) {
    // Reading attributes of the reply
    // e.g. the HTTP status code
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    // no error received?
    if (reply->error() == QNetworkReply::NoError && statusCodeV.toInt() == 200)
    {
        QByteArray bytes = reply->readAll();
        QJsonParseError json_error;
        QJsonDocument parse_document = QJsonDocument::fromJson(bytes, &json_error);
        if (json_error.error == QJsonParseError::NoError) {
            QJsonObject jsonObject = parse_document.object();
            if (jsonObject["error"].toInt() == 0) {
                QJsonArray danmuSet = jsonObject["danmu_set"].toArray();
                QVector<QPair<int, QPair<QString, QString> > > v;
                for (int i = danmuSet.size() - 1; i >= 0; i--) {
                    QJsonObject danmu = danmuSet[i].toObject();
                    int messageId = danmu["message_id"].toInt();
                    QString user = danmu["from_user_name_salt_md5"].toString();
                    QString text = danmu["text"].toString();
                    v.append(qMakePair(messageId, qMakePair(user, text)));
                }
                std::sort(v.begin(), v.end());
                QVector<Danmu::MessageItem> u;
                for (int i = v.size() - 1; i >= 0; i--)
                    u.push_back(Danmu::MessageItem(v[i].second.first, v[i].second.second));
                danmu->setMessages(u);
            }
        }
    }
    reply->deleteLater();
}

void MainWindow::on_checkBoxVisible_stateChanged(int arg1)
{
    configDirty = true;
    danmu->setVisible(arg1 != 0);
}

void MainWindow::on_comboBoxAlign_currentIndexChanged(int)
{
    configDirty = true;
    danmu->update();
}

void MainWindow::on_fontComboBoxFont_currentIndexChanged(int)
{
    configDirty = true;
    danmu->update();
}

void MainWindow::on_pushButtonColor_clicked()
{
    QPalette palette = ui->pushButtonColor->palette();
    QColor color = QColorDialog::getColor(palette.button().color(), this);
    if (color.isValid()) {
        ui->pushButtonColor->setStyleSheet(QString("background-color: %1").arg(color.name()));
        configDirty = true;
        danmu->update();
    }
}

void MainWindow::on_lineEditTitle_textChanged(const QString &)
{
    configDirty = true;
    danmu->update();
}

void MainWindow::on_lineEditFilter_textChanged(const QString &)
{
    configDirty = true;
    danmu->update();
}


void MainWindow::on_spinBoxTime_valueChanged(int)
{
    configDirty = true;
    danmu->update();
}

void MainWindow::on_horizontalSliderLeft_valueChanged(int)
{
    configDirty = true;
    danmu->update();
}

void MainWindow::on_horizontalSliderUp_valueChanged(int)
{
    configDirty = true;
    danmu->update();
}

void MainWindow::on_horizontalSliderWidth_valueChanged(int)
{
    configDirty = true;
    danmu->update();
}

void MainWindow::on_horizontalSliderHeight_valueChanged(int)
{
    configDirty = true;
    danmu->update();
}

void MainWindow::on_horizontalSliderFontsize_valueChanged(int)
{
    configDirty = true;
    danmu->update();
}

void MainWindow::on_horizontalSliderOpacity_valueChanged(int)
{
    configDirty = true;
    danmu->update();
}

void MainWindow::on_horizontalSliderBackground_valueChanged(int)
{
    configDirty = true;
    danmu->update();
}

void MainWindow::saveConfig() {
    if (configLoading || !configDirty)
        return;
    QSettings writer(configFilename, QSettings::IniFormat);
    writer.setValue("danmu/align", ui->comboBoxAlign->currentIndex());
    writer.setValue("danmu/font", ui->fontComboBoxFont->currentFont().family());
    writer.setValue("danmu/color", ui->pushButtonColor->palette().button().color().name());
    writer.setValue("danmu/title", ui->lineEditTitle->text());
    writer.setValue("danmu/filter", ui->lineEditFilter->text());
    writer.setValue("danmu/time", ui->spinBoxTime->value());
    writer.setValue("danmu/left", ui->horizontalSliderLeft->value());
    writer.setValue("danmu/up", ui->horizontalSliderUp->value());
    writer.setValue("danmu/width", ui->horizontalSliderWidth->value());
    writer.setValue("danmu/height", ui->horizontalSliderHeight->value());
    writer.setValue("danmu/fontsize", ui->horizontalSliderFontsize->value());
    writer.setValue("danmu/opacity", ui->horizontalSliderOpacity->value());
    writer.setValue("danmu/background", ui->horizontalSliderBackground->value());
    configDirty = false;
}

void MainWindow::loadConfig() {
    configLoading = true;
    QSettings reader(configFilename, QSettings::IniFormat);
    if (reader.contains("danmu/align"))
        ui->comboBoxAlign->setCurrentIndex(reader.value("danmu/align").toInt());
    if (reader.contains("danmu/font"))
        ui->fontComboBoxFont->setCurrentFont(QFont(reader.value("danmu/font").toString()));
    if (reader.contains("danmu/color"))
        ui->pushButtonColor->setStyleSheet(QString("background-color: %1").arg(reader.value("danmu/color").toString()));
    if (reader.contains("danmu/title"))
        ui->lineEditTitle->setText(reader.value("danmu/title").toString());
    if (reader.contains("danmu/filter"))
        ui->lineEditFilter->setText(reader.value("danmu/filter").toString());
    if (reader.contains("danmu/time"))
        ui->spinBoxTime->setValue(reader.value("danmu/time").toInt());
    if (reader.contains("danmu/left"))
        ui->horizontalSliderLeft->setValue(reader.value("danmu/left").toInt());
    if (reader.contains("danmu/up"))
        ui->horizontalSliderUp->setValue(reader.value("danmu/up").toInt());
    if (reader.contains("danmu/width"))
        ui->horizontalSliderWidth->setValue(reader.value("danmu/width").toInt());
    if (reader.contains("danmu/height"))
        ui->horizontalSliderHeight->setValue(reader.value("danmu/height").toInt());
    if (reader.contains("danmu/fontsize"))
        ui->horizontalSliderFontsize->setValue(reader.value("danmu/fontsize").toInt());
    if (reader.contains("danmu/opacity"))
        ui->horizontalSliderOpacity->setValue(reader.value("danmu/opacity").toInt());
    if (reader.contains("danmu/background"))
        ui->horizontalSliderBackground->setValue(reader.value("danmu/background").toInt());
    configLoading = false;
}
