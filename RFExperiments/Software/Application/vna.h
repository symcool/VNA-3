#ifndef VNA_H
#define VNA_H

#include <QWidget>
#include <QMainWindow>
#include <QGridLayout>
#include <QComboBox>
#include "device.h"
#include "Traces/traceplot.h"
#include "calibration.h"
#include <QProgressDialog>
#include "Menu/menuaction.h"
#include "Traces/tracemodel.h"
#include "Traces/tracemarkermodel.h"
#include "averaging.h"

namespace Ui {
class MainWindow;
}

class VNA : public QMainWindow
{
    Q_OBJECT
public:
    VNA(QWidget *parent = nullptr);
protected:
    void closeEvent(QCloseEvent *event) override;
private:
    static constexpr double minFreq = 0;
    static constexpr double maxFreq = 6000000000;
    static constexpr Protocol::SweepSettings defaultSweep = {
        .f_start = 1000000,
        .f_stop = (uint64_t) maxFreq,
        .points = 501,
        .if_bandwidth = 1000,
        .mdbm_excitation = 0,
    };
private slots:
    void NewDatapoint(Protocol::Datapoint d);
    void ConnectToDevice(QString serial = QString());
    void DisconnectDevice();
    void UpdateDeviceList();
    void StartManualControl();
    void StartImpedanceMatching();
    void SetStartFreq(double freq);
    void SetStopFreq(double freq);
    void SetCenterFreq(double freq);
    void SetSpan(double span);
    void SetFullSpan();
    void SpanZoomIn();
    void SpanZoomOut();
    void ConstrainAndUpdateFrequencies();

private:
    void UpdateStatusPanel();
    void SettingsChanged();
    void DeviceConnectionLost();
    void CreateToolbars();

    struct {
        QComboBox *referenceType;
    } toolbars;

    Device *device;
    QString deviceSerial;
    QActionGroup *deviceActionGroup;
    Protocol::SweepSettings settings;
    unsigned int averages;
    TraceModel traceModel;
    TraceMarkerModel *markerModel;
    Averaging average;

    // Calibration
    Calibration cal;
    Calkit calkit;
    bool calValid;
    Calibration::Measurement calMeasurement;
    bool calMeasuring;
    bool calWaitFirst;
    QProgressDialog calDialog;

    // Calibration menu
    MenuAction *mCalSOL1, *mCalSOL2, *mCalFullSOLT;

    // Status Labels
    QLabel lStart, lCenter, lStop, lSpan, lPoints, lBandwidth;
    QLabel lCalibration;
    QLabel lAverages;

    Ui::MainWindow *ui;
signals:
    void dataChanged();
    void startFreqChanged(double freq);
    void stopFreqChanged(double freq);
    void centerFreqChanged(double freq);
    void spanChanged(double span);
};

#endif // VNA_H
