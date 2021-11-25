#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <QTimer>
#include <QObject>

class QSerialPort;

class SerialComm: public QObject
{
    Q_OBJECT
public:
    explicit SerialComm(QObject *parent=nullptr);
    static QStringList getSerialPorts(void);
    bool connectToDevice(QString port);

private:
     QSerialPort *mSerialPort;
     QTimer mTimer;

private slots:
     void readyRead();
     void fakeReadyRead();

};

#endif // SERIALCOMM_H
