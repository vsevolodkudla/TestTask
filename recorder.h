#ifndef RECORDER_H
#define RECORDER_H

#include <QThread>
#include <QObject>
#include <QDebug>
#include <QList>
#include <QString>
#include <iostream>
#include <fstream>
#include <QByteArray>
using namespace std;

class Recorder : public QThread
{
    Q_OBJECT

    qint64 progress = 0;
    bool work = true;
    ofstream *saveFile;
    QList<QByteArray *>  bufferSave;

public:
    explicit Recorder(QObject *parent = 0);
    void threadSetupS(QThread*);

signals:
    void returnBuf(QByteArray*);
    void getProgress(qint64);

public slots:
    void setFile(ofstream *);
    void addData(QByteArray*);
    void startRecorder();
    void stopRecorder();
};

#endif // RECORDER_H
