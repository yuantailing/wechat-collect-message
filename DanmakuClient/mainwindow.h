#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
class QNetworkReply;
QT_END_NAMESPACE

namespace Ui {
class MainWindow;
}

class Danmu;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend class Danmu;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void pullDanmu();
    void getDanmuFinishedSlot(QNetworkReply *);

    void on_checkBoxVisible_stateChanged(int arg1);
    void on_comboBoxAlign_currentIndexChanged(int index);
    void on_fontComboBoxFont_currentIndexChanged(int index);
    void on_pushButtonColor_clicked();
    void on_lineEditTitle_textChanged(const QString &arg1);
    void on_lineEditFilter_textChanged(const QString &arg1);
    void on_spinBoxTime_valueChanged(int arg1);
    void on_horizontalSliderLeft_valueChanged(int value);
    void on_horizontalSliderUp_valueChanged(int value);
    void on_horizontalSliderWidth_valueChanged(int value);
    void on_horizontalSliderHeight_valueChanged(int value);
    void on_horizontalSliderFontsize_valueChanged(int value);
    void on_horizontalSliderOpacity_valueChanged(int value);
    void on_horizontalSliderBackground_valueChanged(int value);

    void saveConfig();

private:
    void loadConfig();

private:
    Ui::MainWindow *ui;
    QString configFilename;
    bool configLoading;
    bool configDirty;
    Danmu *danmu;
    QNetworkAccessManager *nam;
};

#endif // MAINWINDOW_H
