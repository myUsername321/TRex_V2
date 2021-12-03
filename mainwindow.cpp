#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QTextStream>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QChar>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //design of ui-elements
    ui->but_selectFile->setStyleSheet("QPushButton { background-color: #6d6d6d; border-width: 2px; border-color: #333333; color: white; border-radius: 2px} QPushButton:hover { background-color: #797878; border-width: 2px; border-color: #333333; color: white; border-radius: 2px } QPushButton:pressed{background-color: #909090; border-width: 2px; border-color: #333333; color: white; border-radius: 2p}");
    ui->but_start->setStyleSheet("QPushButton { background-color: #008F96; border: 3; border-width: 5px; border-color: #333333; color: white; border-radius: 2px} QPushButton:hover { background-color: #0299A1; border: 3; border-width: 5px; border-color: #333333; color: white; border-radius: 2px } QPushButton:pressed { background-color: #05B4BD; border: 3; border-width: 5px; border-color: #333333; color: white; border-radius: 2px }");
    ui->but_stop->setStyleSheet("QPushButton { background-color: #6f0000; border: 3; border-width: 2px; border-color: #333333; color: white; border-radius: 2px} QPushButton:hover { background-color: #890101; border: 3; border-width: 5px; border-color: #333333; color: white; border-radius: 2px } QPushButton:pressed { background-color: #A10303; border: 3; border-width: 5px; border-color: #333333; color: white; border-radius: 2px }");
    ui->txt_file->setStyleSheet("QLineEdit { background-color: #333333; border: 3; border-color: #333333; color: white; border-radius: 2px} QLineEdit:hover {background-color: #3A3A3A; border: 3; border-color: #333333; color: white; border-radius: 2px }");
    //TODO: arrow dark (currently cannot be seen)
    ui->comboPort->setStyleSheet("QComboBox {background-color: #333333; border: 3; border-color: #333333; color: white; border-radius: 2px} QComboBox:hover {background-color: #3A3A3A; border: 3; border-color: #333333; color: white; border-radius: 2px} QComboBox:down-arrow {color: white; background-color : #6d6d6d; border-color: #6d6d6d;} QComboBox:drop-down {color: #333333; background-color: #6d6d6d; border-left-color: #333333;}");

    QStringList ports = getSerialPorts();
    ui->comboPort->addItems(ports);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::checkColumnPhases(QStringList rawList, bool fileOk)
{
    int j=8;
    int ID = 1;
    while(j<rawList.length()-3)
    {
        //check if fields with phases only contain numbers
        for(int i=0; i<rawList[j].length(); i++)
        {
            QChar tmp = rawList[j][i];
            if(!tmp.isDigit())
            {
                QString currentText = ui->txt_file->text();
                if(currentText.mid(currentText.length()-58, 58) != " Please only insert ascending numbers in the phase-column!")
                {
                    ui->txt_file->insert(" Please only insert ascending numbers in the phase-column!");
                }
                fileOk = false;
            }
        }

        //check if phase-ID numbers are ascending beginnging from 1
        if(rawList[j].toInt() != ID)
        {
            QString currentText = ui->txt_file->text();
            if(currentText.mid(currentText.length()-58, 58) != " Please only insert ascending numbers in the phase-column!")
            {
                ui->txt_file->insert(" Please only insert ascending numbers in the phase-column!");
            }
            fileOk = false;
        }
        ID = ID + 1;
        j = j + 4;
    }
    return fileOk;
}

bool MainWindow::checkNumberOfCycles(QStringList rawList, bool fileOk)
{
    for(int i=0; i<rawList[1].length(); i++)
    {
        QChar tmp = rawList[1][i];
        if(!tmp.isDigit())
        {
            QString currentText = ui->txt_file->text();
            if(currentText.mid(currentText.length()-17, 17) != "number of cycles!")
            {
                ui->txt_file->insert(" Please insert the number of cycles!");
            }
            fileOk = false;
        }
    }
    if(rawList[1] == "")
    {
        QString currentText = ui->txt_file->text();
        if(currentText.mid(currentText.length()-17, 17) != "number of cycles!")
        {
            ui->txt_file->insert(" Please insert the number of cycles!");
        }
        fileOk = false;
    }
    return fileOk;
}

bool MainWindow::checkTimeFormat(QStringList rawList, bool fileOk)
{
    int k=9;
    while(k<rawList.length()-2)
    {
        //check each time entry
        for(int i=0; i<rawList[k].length(); i++)
        {
            QString strTmp = QString(rawList[k][i]);
            QChar tmp = rawList[k][i];
            if(i == 0 | i == 1 | i == 3 | i == 4 | i == 6 | i == 7 | i == 9)
            {
                //check if digit
                if(!tmp.isDigit())
                {
                    QString currentText = ui->txt_file->text();
                    if(currentText.mid(currentText.length()-60, 60) != " Please insert the time in the correct format (hh:mm:ss,ds)!")
                    {
                        ui->txt_file->insert(" Please insert the time in the correct format (hh:mm:ss,ds)!");
                    }
                    fileOk = false;
                }
            }
            else if(i == 2 | i == 5)
            {
                //check if ":"
                if(strTmp != ":")
                {
                    QString currentText = ui->txt_file->text();
                    if(currentText.mid(currentText.length()-60, 60) != " Please insert the time in the correct format (hh:mm:ss,ds)!")
                    {
                        ui->txt_file->insert(" Please insert the time in the correct format (hh:mm:ss,ds)!");
                    }
                    fileOk = false;
                }
            }
            else if(i==8)
            {
                //check if ","
                if(strTmp != ",")
                {
                    QString currentText = ui->txt_file->text();
                    if(currentText.mid(currentText.length()-60, 60) != " Please insert the time in the correct format (hh:mm:ss,ds)!")
                    {
                        ui->txt_file->insert(" Please insert the time in the correct format (hh:mm:ss,ds)!");
                    }
                    fileOk = false;
                }
            }
        }
        k = k + 4;
    }
    return fileOk;
}

bool MainWindow::checkPositionEntries(QStringList rawList, bool fileOk)
{
    //check if position entries are numbers
    int l=10;
    while(l<rawList.length()-1)
    {
        int k=0;
        while(k<2)
        {
            if(rawList[l].toInt()<0 || rawList[l].toInt()>100)
            {
                QString currentText = ui->txt_file->text();
                if(currentText.mid(currentText.length()-18, 18) != "between 0 and 100!")
                {
                    ui->txt_file->insert(" The position can only be values between 0 and 100!");
                }
                fileOk = false;
            }
            for(int i=0; i<rawList[l].length(); i++)
            {
                QChar tmp = rawList[l][i];
                if(!tmp.isDigit())
                {
                    QString currentText = ui->txt_file->text();
                    if(currentText.mid(currentText.length()-18, 18) != "between 0 and 100!")
                    {
                        ui->txt_file->insert(" The position can only be values between 0 and 100!");
                    }
                    fileOk = false;
                }
            }
            l=l+1;
            k=k+1;
        }
        l=l+2;
    }
    //check if start is end of last position
    int i=11;
    while(i <= rawList.length() - 5)
    {
        int lastEnd = rawList[i].toInt();
        int currentStart = rawList[i + 3].toInt();
        if(lastEnd != currentStart)
        {
            QString currentText = ui->txt_file->text();
            if(currentText.mid(currentText.length()-14, 14) != "last position!")
            {
                ui->txt_file->insert(" The start of a position must be equal to the end of the last position!");
            }
            fileOk = false;
        }
        i = i + 4;
    }
    if(rawList[8] != "1")
    {
        ui->txt_file->insert(" The format of the csv file is not correct! Please select a csv file with the correct template!");
        fileOk = false;
    }
    return fileOk;
}

QString MainWindow::editList4protocol(QStringList List)
{
    QString string2send = "CYCLE";
    for(int i=0; i<List.length(); i++)
    {
        if(i==0)
        {
            QString tmp = QStringLiteral("%1").arg(List[i].toInt(), 4, 10, QLatin1Char('0'));
            string2send.append(tmp);
        }
        else
        {
            int tmp1 = i%3;
            switch(tmp1)
            {
                case 0:
                {
                    if(List[i].toInt() < 0)
                    {
                        string2send.append("L");
                        List[i].remove(0, 1);
                    }
                    else
                    {
                        string2send.append("R");
                    }
                    QString tmp = QStringLiteral("%1").arg(List[i].toInt(), 3, 10, QLatin1Char('0'));
                    string2send.append(tmp);
                    break;
                }
                case 1:
                {
                    QString tmp = QStringLiteral("%1").arg(List[i].toInt(), 3, 10, QLatin1Char('0'));
                    string2send.append(tmp);
                    break;
                }
                case 2:
                {
                    QString tmp = QStringLiteral("%1").arg(List[i].toInt(), 9, 10, QLatin1Char('0'));
                    string2send.append(tmp);
                    break;
                }

            }
        }
    }
    string2send.append("\n");
    return string2send;
}

QStringList MainWindow::getSerialPorts()
{
    QStringList ports;
    QList<QSerialPortInfo> serialPortInfos=QSerialPortInfo::availablePorts();

    for(QSerialPortInfo &portInfo:serialPortInfos)
    {
       ports.append(portInfo.portName());
    }
    return ports;
}

void MainWindow::send(QByteArray byteArr2Send)
{
    QSerialPort serial;

    serial.setPortName(ui->comboPort->currentText());
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);



    if (serial.open(QIODevice::WriteOnly))
    {
        qDebug() << ("Connected");
    } else
    {
        qDebug() << ("Error: Could not connect");
    }
    serial.flush();
    //serial.putChar(0x00);
    serial.waitForBytesWritten();
    //_sleep(1000);
    if(serial.isWritable())
    {
//        int int_sizeofByteArr = byteArr2Send.size();
//        QString tmp = QStringLiteral("%1").arg(int_sizeofByteArr, 8, 10, QLatin1Char('0')); //fill with zeros -> 8 digits
//        QByteArray sizeOfByteArr;
//        sizeOfByteArr += tmp;

//        serial.write(sizeOfByteArr);
//        serial.write(byteArr2Send);
//        qDebug() << "Data sent";
//        qDebug() << byteArr2Send;
//        serial.write(byteArr2Send);

        //Test
        QByteArray test;
        test.append(0x61);
        serial.write(test);
    }
    else
    {
        qDebug() << "Not writeable";
    }

    if (serial.error() == QSerialPort::WriteError)
        qDebug() << QObject::tr("Failed to write from port %1, error: %2").arg(ui->comboPort->currentText()).arg(serial.errorString()) << endl;

    serial.waitForBytesWritten();
    serial.flush();
    //_sleep(5000);
    if (serial.isOpen())
        serial.close();

    qDebug() << ("Disconnected");
}

void MainWindow::on_but_selectFile_clicked()
{
    //get file-path
    QString path= QFileDialog::getOpenFileName(this, "Select File");
    ui->txt_file->insert(path);

    //check if csv-file
    QString ending = path.mid(path.length()-3, 3);
    if(ending != "csv")
    {
        ui->txt_file->clear();
        ui->txt_file->insert("Please select a csv-file!");
    }

    //read csv-file
    QFile file(path);
    if ( !file.open(QFile::ReadOnly | QFile::Text) )
    {
        qDebug() << "File does not exist!";
    }
    else
    {
       // Create a thread to retrieve data from a file
       QTextStream in(&file);
       QStringList rawList;
       QStringList editedList;

       //Read data up to the end of the file
       while (!in.atEnd())
       {
           QString line = in.readLine();
           // Add values to a StringList
           int checkLineLength = 0;
           for (QString item : line.split(";"))
           {
               checkLineLength ++;
               if(checkLineLength < 5)
               {
                   //only values in the first four columns are read
                   rawList.append(item);
               }
           }
       }
       //check
       bool fileOk = true;
       //check if phases are ascending numbers
       fileOk = checkColumnPhases(rawList, fileOk);

       //check if number of cycles is a digit
       fileOk = checkNumberOfCycles(rawList, fileOk);

       //check if time is in the correct format (hh:mm:ss,ds)
       fileOk = checkTimeFormat(rawList, fileOk);

       //check if startposition is 0
       if(rawList[10] != "0")
       {
           ui->txt_file->insert(" The start has to be at position 0!");
           fileOk = false;
       }

       //check position entries
       fileOk = checkPositionEntries(rawList, fileOk);

       //number of cycles
       editedList.append(rawList[1]);

       // Remove unnecessary data
       for(int i=8; i<rawList.length(); i++)
       {
           int tmp1 = i%4;
           switch(tmp1)
           {
               case 0:
               {
                   editedList.append(rawList[i]);
                   break;
               }
               case 1:
               {
                   QString tmp2 = rawList[i];
                   int intHours = tmp2.mid(0, 2).toInt();
                   int intMinutes = tmp2.mid(3, 2).toInt();
                   int intSeconds = tmp2.mid(6, 2).toInt();
                   int tmpDeciSeconds = 0;
                   if(tmp2.size() == 10)
                   {
                       int intDeciseconds = tmp2.mid(9, 1).toInt();
                       tmpDeciSeconds = intHours * 36000 + intMinutes * 600 + intSeconds * 10 + intDeciseconds;
                   }
                   else
                   {
                       tmpDeciSeconds = intHours * 36000 + intMinutes * 600 + intSeconds * 10;
                   }
                   QString milliSeconds = QString::number(tmpDeciSeconds * 100);
                   editedList.append(milliSeconds);
                   break;
                }
               case 2:
               {
                   int tmp3 = (rawList[i+1].toInt()) - (rawList[i].toInt());
                   editedList.append(QString::number(tmp3));
                   break;
               }
               case 3:
               {
                   break;
               }
           }
       }
       if(fileOk == true)
       {
           string2send = editList4protocol(editedList);
           QByteArray byteArr2Send;
           byteArr2Send += string2send;
           send(byteArr2Send);
       }
       else
       {
           string2send = "not valid";
           ui->txt_file->insert(" Please select another csv-file!");
       }
       file.close();
    }
}

void MainWindow::on_but_start_clicked()
{
    QString str2send = "START\n";
    QByteArray byteArr2Send;
    byteArr2Send += str2send;

    send(byteArr2Send);
}

void MainWindow::on_but_stop_clicked()
{
    QString str2send = "STOP\n";
    QByteArray byteArr2Send;
    byteArr2Send += str2send;

    send(byteArr2Send);
}


