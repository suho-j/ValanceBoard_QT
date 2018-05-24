#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

class PointPolarStream;
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
    void on_replayButton_clicked();

    void on_resetButton_clicked();

    void on_spotButton_clicked();

    void on_analyzeButton_clicked();

    void on_histogramButton_clicked();

    void on_linkButton_clicked();

    void sensorData(QString data);

    void connected();

    void disconnected(bool onceConnected);

    void on_autoButton_clicked();

    void on_manualButton_clicked();

    void autoStop();

private:
    enum AppState
    {
        NotConnected,
        ConnectedNotRecording,
        Recording,
    };
    AppState appState = NotConnected;

    int dataCount = 0;

    float initial1 = 0;
    float initial2 = 0;
    float initial3 = 0;
    float initial4 = 0;

    bool init = false;

    QTimer autoTimer;

    QString originalSensorData;

    bool fileExists(QString path);

    void startRecord();

    void endRecord();

    void processData(float a1, float a2, float a3, float a4);

    void resetBoardPlot();

    void toggleLinkButton(bool default_mode);

    void toggleSpotButton(bool default_mode);

    void toggleReplayButton(bool default_mode);

    void toggleManualButton(bool default_mode);

    Ui::MainWindow *ui;

    PointPolarStream* mainStream;
    PointPolarStream* convexHullStream;
};

#endif // MAINWINDOW_H
