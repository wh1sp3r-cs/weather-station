#include "./ui_mainwindow.h"
#include "mainwindow.h"
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initRegionsMap();

    ipc = new SharedMemoryManager(false);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateDashboard);
    timer->start(100);
}

MainWindow::~MainWindow() {
    delete ui;
    delete ipc;
}

void MainWindow::initRegionsMap() {

    regions[0] = { ui->group_0, ui->lblTemp_0, ui->lblHum_0, ui->lblWind_0 };
    regions[1] = { ui->group_1, ui->lblTemp_1, ui->lblHum_1, ui->lblWind_1 };
    regions[2] = { ui->group_2, ui->lblTemp_2, ui->lblHum_2, ui->lblWind_2 };
    regions[3] = { ui->group_3, ui->lblTemp_3, ui->lblHum_3, ui->lblWind_3 };

    const QString colors[] = { "blue", "red", "green", "orange" };
    for(int i=0; i<4; i++) {
        regions[i].box->setStyleSheet(QString("QGroupBox { font-weight: bold; color: %1; border: 1px solid gray; margin-top: 10px; } "
                                              "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 3px; }")
                                          .arg(colors[i]));
        regions[i].box->setEnabled(false);
    }
}

void MainWindow::updateDashboard() {
    DashboardState state = ipc->readAll();

    for (int i = 0; i < 4; ++i) {
        if (state.isActive[i]) {
            regions[i].box->setEnabled(true);

            WeatherPacket data = state.data[i];
            regions[i].lblTemp->setText(QString("Температура: %1 °C").arg(data.temperature, 0, 'f', 1));
            regions[i].lblHum->setText(QString("Вологість: %1 %").arg(data.humidity, 0, 'f', 1));
            regions[i].lblWind->setText(QString("Вітер: %1 м/с").arg(data.windSpeed, 0, 'f', 1));
        }
    }
}
