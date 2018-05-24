const double boardplot_max_radius = 5.5;
const int histogram_divides = 55;
const int auto_measure_time_ms = 10000;

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QKeyEvent>
#include <QTextCodec>
#include <QTime>

#include <qwt_text_label.h>
#include <cstdio>

#include "qwtpointcartesiantopolar.h"
#include "convexhull.h"
#include "histogramdialog.h"

#include "bluetooththread.h"
#include "histogramcreator.h"

void delay(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(size());
    this->setStyleSheet("QMainWindow { background-color: rgb(43, 30, 30); } QLabel { color: white }");

    ui->top->setStyleSheet("color: white");
    ui->bottom->setStyleSheet("color: white");
    ui->left->setStyleSheet("color: white");
    ui->right->setStyleSheet("color: white");

    BoardPlot* p = ui->mainPlot;
    p->addStream(); // 기본으로 stream 1개가 있음 ( mainStream ), convexHull을 위한 stream을 추가 ( convexHullStream )

    mainStream = p->streams[0];
    convexHullStream = p->streams[1];

    mainStream->setLineColor(Qt::yellow);
    mainStream->setPointColor(Qt::green);
    convexHullStream->setLineColor(Qt::red);

    ui->top->setTitle("TOP");
    ui->top->titleLabel()->setFont(QFont(fontInfo().family(), 8));
    ui->top->streams[0]->setLineColor(Qt::red);
    ui->bottom->setTitle("BOTTOM");
    ui->bottom->titleLabel()->setFont(QFont(fontInfo().family(), 8));
    ui->bottom->streams[0]->setLineColor(Qt::yellow);
    ui->left->setTitle("LEFT");
    ui->left->titleLabel()->setFont(QFont(fontInfo().family(), 8));
    ui->left->streams[0]->setLineColor(QColor(144, 238, 144));
    ui->right->setTitle("RIGHT");
    ui->right->titleLabel()->setFont(QFont(fontInfo().family(), 8));
    ui->right->streams[0]->setLineColor(Qt::magenta);

    ui->replayButton->setProperty("default_mode", true); // 토글식 버튼을 위한 property ( REPLAY <-> STOP )
    ui->spotButton->setProperty("default_mode", true); // 토글식 버튼을 위한 property ( SPOT <-> LINES )
    ui->linkButton->setProperty("default_mode", true); // 토글식 버튼을 위한 property ( LINK <-> UNLINK )
    ui->manualButton->setProperty("default_mode", true); // 토글식 버튼을 위한 property ( LINK <-> UNLINK )

    ui->autoButton->setEnabled(false);
    ui->manualButton->setEnabled(false);
    ui->replayButton->setEnabled(true);
    ui->linkButton->setEnabled(true);

    connect(&autoTimer, SIGNAL(timeout()), this, SLOT(autoStop()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::toggleReplayButton(bool default_mode)
{
    if(default_mode)
    {
        ui->linkButton->setEnabled(true);
        ui->replayButton->setProperty("default_mode", true);
        ui->replayButton->setText("REPLAY");
        ui->resetButton->setEnabled(true);
    }
    else
    {
        ui->linkButton->setEnabled(false);
        ui->replayButton->setProperty("default_mode", false);
        ui->replayButton->setText("STOP");
        ui->resetButton->setEnabled(false);
    }
}

void MainWindow::toggleManualButton(bool default_mode)
{
    if(default_mode)
    {
        ui->manualButton->setProperty("default_mode", true);
        ui->manualButton->setText("MANUAL");
    }
    else
    {
        ui->manualButton->setProperty("default_mode", false);
        ui->manualButton->setText("STOP");
    }
}

void MainWindow::on_replayButton_clicked()
{
    static bool stop = false;
    if(!ui->replayButton->property("default_mode").toBool()) // DEFAULT 버튼이 아님 = STOP 버튼
    {
        stop = true;
        return;
    }

    auto fileName = QFileDialog::getOpenFileName(this, "데이터 파일 열기", "", tr("데이터 파일 (*.txt)"));
    if(fileName == "")
        return;

    toggleReplayButton(false);

    resetBoardPlot();

    ui->mainPlot->showMarker();
#if(defined(_WIN32) || defined(_WIN64))
    FILE* fp = _wfopen(fileName.toStdWString().c_str(), L"r");
#elif(defined(LINUX) || defined(__linux__))
    FILE* fp = fopen(fileName.toUtf8().toStdString().c_str(), "r");
#endif

    float a1, a2, a3, a4;
    while(fscanf(fp, "%f,%f,%f,%f", &a1, &a2, &a3, &a4) == 4)
    {
        processData(a1, a2, a3, a4);

        delay(33);

        if(stop)
            break;
    }
    toggleReplayButton(true);
    stop = false;
    fclose(fp);
}

void MainWindow::on_resetButton_clicked()
{
    resetBoardPlot();
}


void MainWindow::toggleSpotButton(bool default_mode)
{
    if(default_mode)
    {
        ui->spotButton->setProperty("default_mode", true);
        ui->spotButton->setText("SPOT");
    }
    else
    {
        ui->spotButton->setProperty("default_mode", false);
        ui->spotButton->setText("LINES");
    }
}

void MainWindow::on_spotButton_clicked()
{
    if(!ui->spotButton->property("default_mode").toBool()) // DEFAULT 버튼이 아님 = LINES 버튼
    {
        toggleSpotButton(true);

        ui->mainPlot->setCenterText("");
        mainStream->setType(PointPolarStream::Lines);
        convexHullStream->setType(PointPolarStream::Hidden);
        if(mainStream->getRawData().size() > 0)
        {
            if(appState == NotConnected)
                ui->mainPlot->setMarkerPosition(mainStream->getRawData()[mainStream->getRawData().size() - 1].toPoint());
            ui->mainPlot->showMarker();
        }
        if(appState == ConnectedNotRecording)
            ui->mainPlot->showMarker();
    }
    else
    {
        toggleSpotButton(false);

        ui->mainPlot->setCenterText("");
        mainStream->setType(PointPolarStream::Points);
        convexHullStream->setType(PointPolarStream::Hidden);
        ui->mainPlot->hideMarker();
    }
}

void MainWindow::on_analyzeButton_clicked()
{
    auto data = mainStream->getRawData();
    if(data.size() > 0)
    {
        std::vector<Point> pointList;
        for(int i = 0; i < data.size(); i++)
        {
            auto point = data[i].toPoint();
            Point p;
            p.x = point.x();
            p.y = point.y();
            pointList.push_back(p);
        }

        auto hull = convex_hull(pointList);
        QVector<QwtPointPolar> hullPolarPoints;
        for(auto p : hull)
            hullPolarPoints.push_back(QPointF(p.x, p.y));
        hullPolarPoints.push_back(QPointF(hull[0].x, hull[0].y));

        ui->mainPlot->hideMarker();
        mainStream->setType(PointPolarStream::Hidden);
        convexHullStream->clear();
        convexHullStream->setType(PointPolarStream::Lines);
        convexHullStream->setRawData(hullPolarPoints);

        double area = hull_area(hull);
        ui->mainPlot->setCenterText(QString::number(area));
    }
}

void MainWindow::resetBoardPlot()
{
    dataCount = 0;
    toggleSpotButton(true);
    ui->top->streams[0]->clear();
    ui->bottom->streams[0]->clear();
    ui->left->streams[0]->clear();
    ui->right->streams[0]->clear();
    ui->mainPlot->setCenterText("");

    mainStream->clear();
    convexHullStream->clear();
    mainStream->setType(PointPolarStream::Lines);
    convexHullStream->setType(PointPolarStream::Lines);
    ui->mainPlot->hideMarker();
    ui->mainPlot->setMarkerPosition(QPointF(0, 0));
    if(appState == ConnectedNotRecording)
        ui->mainPlot->showMarker();
}

void MainWindow::toggleLinkButton(bool default_mode)
{
    if(default_mode)
    {
        ui->linkButton->setProperty("default_mode", true);
        ui->linkButton->setText("LINK");
    }
    else
    {
        ui->linkButton->setProperty("default_mode", false);
        ui->linkButton->setText("UNLINK");
    }
}

void MainWindow::on_histogramButton_clicked()
{
    HistogramCreator creator(-boardplot_max_radius, boardplot_max_radius, histogram_divides);
    auto data = mainStream->getRawData();
    if(data.size() > 0)
    {
        TBLRHistograms hists = creator.createHistograms(data);

        HistogramDialog d;
        d.setHistogramData(hists);
        d.exec();
    }
}

void MainWindow::on_linkButton_clicked()
{
    static BluetoothThread* btThread = nullptr;
    if(!ui->linkButton->property("default_mode").toBool()) // DEFAULT 버튼이 아님 = UNLINK 버튼
    {
        btThread->stopLoop = 1;
        return;
    }

    ui->replayButton->setEnabled(false);
    ui->linkButton->setEnabled(false);
    if(btThread)
        delete btThread;
    btThread = new BluetoothThread();
    QObject* worker = new QObject();
    worker->moveToThread(btThread);
    connect(btThread, SIGNAL(sensorData(QString)), this, SLOT(sensorData(QString)));
    connect(btThread, SIGNAL(connected()), this, SLOT(connected()), Qt::BlockingQueuedConnection);
    connect(btThread, SIGNAL(disconnected(bool)), this, SLOT(disconnected(bool)));
    btThread->start();
}

void MainWindow::sensorData(QString data)
{
    if(appState == Recording)
    {
        originalSensorData += data + "\n";
    }

    float a1, a2, a3, a4;
    if(sscanf(data.toStdString().c_str(), "%f,%f,%f,%f", &a1, &a2, &a3, &a4) == 4)
    {
        processData(a1, a2, a3, a4);
    }
}

void MainWindow::connected()
{
    appState = ConnectedNotRecording;
    ui->autoButton->setEnabled(true);
    ui->manualButton->setEnabled(true);
    ui->linkButton->setEnabled(true);
    ui->replayButton->setEnabled(false);
    toggleLinkButton(false);
    resetBoardPlot();
    ui->mainPlot->showMarker();
}

void MainWindow::disconnected(bool onceConnected)
{
    ui->autoButton->setEnabled(false);
    ui->manualButton->setEnabled(false);
    ui->replayButton->setEnabled(true);
    ui->linkButton->setEnabled(true);
    toggleLinkButton(true);
    toggleManualButton(true);
    if(appState == Recording)
        endRecord();
    appState = NotConnected; // endRecord에서 appState를 변경할 경우에도 올바른 값으로 재변경하기 위해 마지막 라인에 배치

    if(onceConnected)
        QMessageBox(QMessageBox::Information, "알림", "연결이 종료되었습니다.").exec();
    else
        QMessageBox(QMessageBox::Information, "알림", "연결에 실패하였습니다.").exec();
}

void MainWindow::processData(float a1, float a2, float a3, float a4)
{
    int weight = a1 + a2 + a3 + a4;
    if((weight > 10) && !init)
        init = true;

    if (dataCount == 0 || (init && (weight <1)))
    {
        initial1 = a1;
        initial2 = a2;
        initial3 = a3;
        initial4 = a4;
        init = false;
    }

    a1 = a1 - initial1;
    a2 = a2 - initial2;
    a3 = a3 - initial3;
    a4 = a4 - initial4;
    if(a1<1) a1 = 0;
    if(a2<1) a2 = 0;
    if(a3<1) a3 = 0;
    if(a4<1) a4 = 0;
    dataCount++;

    ui->top->streams[0]->append(QPointF(dataCount, a1));
    ui->bottom->streams[0]->append(QPointF(dataCount, a2));
    ui->left->streams[0]->append(QPointF(dataCount, a3));
    ui->right->streams[0]->append(QPointF(dataCount, a4));

    /*
    double y = boardplot_max_radius * ( a1 - a2 ) / ( a1 + a2 + a3 + a4 );
    double x = boardplot_max_radius * ( a4 - a3 ) / ( a1 + a2 + a3 + a4 );
    */
	// 시계방향 45도 회전
    double y = boardplot_max_radius * (a1 + a3 - (a2 + a4)) / (a1 + a2 + a3 + a4);
    double x = boardplot_max_radius * (a1 + a2 - (a3 + a4)) / (a1 + a2 + a3 + a4);
    if(weight<1)
    {
        x = 0;
        y=0;
    }
    printf("a1: %f  a2: %f  a3: %f  a4: %f\n", a1, a2, a3, a4);
    printf("x: %f  y: %f\n", x, y);

    // a1, a2, a3, a4가 모두 0인 경우. device에서 1 이상의 값이 전달되므로 이 조건문은 필요없음. ( 필요없지만 안전상 )
    // 이외에 가운데로 놓는 방법도 고려 가능
    if(std::isnan(x) || std::isnan(y))
        return;

    if(appState == NotConnected || appState == Recording)
        mainStream->append( QPointF( x, y ) );
    ui->mainPlot->setMarkerPosition( QPointF( x, y ) );



}

void MainWindow::on_autoButton_clicked()
{
    ui->autoButton->setEnabled(false);
    ui->manualButton->setEnabled(false);
    autoTimer.setInterval(auto_measure_time_ms);
    autoTimer.setSingleShot(true);
    autoTimer.start();
    startRecord();
}

void MainWindow::on_manualButton_clicked()
{
    if(ui->manualButton->property("default_mode").toBool()) // DEFAULT 버튼 = MANUAL 버튼
    {
        toggleManualButton(false);
        ui->autoButton->setEnabled(false);
        startRecord();
    }
    else
    {
        toggleManualButton(true);
        ui->autoButton->setEnabled(true);
        endRecord();
    }
}

void MainWindow::autoStop()
{
    ui->autoButton->setEnabled(true);
    ui->manualButton->setEnabled(true);
    endRecord();
}

bool MainWindow::fileExists(QString path)
{
    QFileInfo checkFile(path);
    return checkFile.exists();
}

void MainWindow::startRecord()
{
    resetBoardPlot();
    ui->resetButton->setEnabled(false);
    originalSensorData = "";
    appState = Recording;
}

void MainWindow::endRecord()
{
    autoTimer.stop();
    appState = ConnectedNotRecording;
    ui->resetButton->setEnabled(true);
    QString fileName(ui->username->text());
    fileName += "_";
    QString finalName;
    int count = 1;
    do
    {
        finalName = fileName + QString::number(count) + ".txt";
        count++;
    } while(fileExists(finalName));

#if(defined(_WIN32) || defined(_WIN64))
    FILE* fp = _wfopen(finalName.toStdWString().c_str(), L"w");
#elif(defined(LINUX) || defined(__linux__))
    FILE* fp = fopen(finalName.toUtf8().toStdString().c_str(), "w");
#endif
    fputs(originalSensorData.toStdString().c_str(), fp);
    fclose(fp);
    QMessageBox(QMessageBox::NoIcon, "알림", "'" + ui->username->text() + "' 사용자의 " + QString::number(count - 1) + "번째 측정 결과가 기록되었습니다.").exec();
}
