#pragma once
#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QGroupBox>
#include "../common/protocol.h"
#include "../ipc_utils/shared_mem.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateDashboard();

private:
    Ui::MainWindow *ui;
    SharedMemoryManager* ipc;
    QTimer* timer;

    struct RegionUI {
        QGroupBox* box;
        QLabel* lblTemp;
        QLabel* lblHum;
        QLabel* lblWind;
    };
    RegionUI regions[4];

    void initRegionsMap();
};
