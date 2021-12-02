#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString string2send; // 1. Abzahl Zykle nPhasennummer + delta Zeit [ds] + delta Position [%]

    bool checkColumnPhases(QStringList rawList, bool fileOk);
    bool checkNumberOfCycles(QStringList rawList, bool fileOk);
    bool checkTimeFormat(QStringList rawList, bool fileOk);
    bool checkPositionEntries(QStringList rawList, bool fileOk);

    QString editList4protocol(QStringList List);

    static QStringList getSerialPorts(void);
    void send(QByteArray byteArr2Send);




private slots:
    void on_but_selectFile_clicked();
    void on_but_start_clicked();
    void on_but_stop_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
