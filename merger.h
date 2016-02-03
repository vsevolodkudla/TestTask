#ifndef MERGER_H
#define MERGER_H

#include <QDebug>
#include <QObject>
#include <QList>
#include <QString>
#include <QThread>
#include <iostream>
#include <fstream>
#include <QVector>
#include <QByteArray>
using namespace std;

class Merger : public QThread
{
    Q_OBJECT

    bool work = true;

    const int bufSize = 32000;
    QList<QByteArray *>  bufferLoad;

    QList<QString> loadFiles;
    int countFiles;

    ifstream *loadFilesMas;
    long *idCalls;
    long *sizeCalls;

    int idMin();
public:
    explicit Merger(QObject *parent = 0);
    void threadSetup(QThread*);
    void setFiles(QList<QString>&);

signals:
    void sendData(QByteArray*);
public slots:
    void startMerger();
    void stopMerger();

    void returnBuf(QByteArray*);
};

#endif // MERGER_H
