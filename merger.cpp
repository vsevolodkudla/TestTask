#include "merger.h"

Merger::Merger(QObject *parent) :
    QThread(parent)
{
    QByteArray *buf;
    bufferLoad.reserve(1024);
    for (int i = 0; i < 1024; i++)
    {
        buf = new QByteArray;
        buf->resize(bufSize);
        bufferLoad.append(buf);
    }
}

void Merger::threadSetup(QThread *thread)
{
    connect(thread, SIGNAL(started()), this, SLOT(startMerger()));
    connect(thread, SIGNAL(finished()), this, SLOT(stopMerger()));
}

void Merger::setFiles(QList<QString> &load)
{
    loadFiles = load;
    countFiles = load.size();
    work = true;
}


void Merger::startMerger()
{
    loadFilesMas = new ifstream[loadFiles.size()];
    idCalls = new long[loadFiles.size()];
    sizeCalls = new long[loadFiles.size()];

    for (int i = 0; i < countFiles; i++)
    {
        qDebug() << loadFiles.last();
        loadFilesMas[i].open(loadFiles.takeLast().toStdString().c_str());
        loadFilesMas[i] >> idCalls[i];
        qDebug() << idCalls[i];
        loadFilesMas[i] >> sizeCalls[i];
        qDebug() << sizeCalls[i];
    }

    QByteArray *buf;
    int posFile;
    while(work)
    {
        while(bufferLoad.size() < 1)
            sleep(100);

        posFile = idMin();
        if(posFile != -1 && loadFilesMas[posFile].eof())
        {
            idCalls[posFile] = -1;
            sizeCalls[posFile] = -1;
            continue;
        }
        if (posFile == -1)
        {
            work = false;
            break;
        }

        buf = bufferLoad.takeFirst();

        QString headCall;
        headCall.append(QString::number(idCalls[posFile]));
        headCall.append(' ');
        headCall.append(QString::number(sizeCalls[posFile]));
        headCall.append(' ');
        strcpy(buf->data(),headCall.toStdString().c_str());
        buf->resize(headCall.size());

        emit sendData(buf);

        while (sizeCalls[posFile] > bufSize)
        {
            while(bufferLoad.size() < 1)
                sleep(100);

            buf = bufferLoad.takeFirst();
            sizeCalls[posFile] -= bufSize;
            loadFilesMas[posFile].read(buf->data(), bufSize);
            buf->resize(bufSize);
            emit sendData(buf);
        }
        while(bufferLoad.size() < 1)
            sleep(100);

        buf = bufferLoad.takeFirst();
        buf->resize(sizeCalls[posFile]+1);
        char t = ' ';
        int k=0;

        while(!loadFilesMas[posFile].eof() && t != '\0')
        {
            loadFilesMas[posFile] >> t;
            buf->insert(k++, t);
        }
        buf->resize(k);

        //loadFilesMas[posFile].read(buf->data(),sizeCalls[posFile]);

        emit sendData(buf);

        if(loadFilesMas[posFile].eof())
        {
            idCalls[posFile] = -1;
            sizeCalls[posFile] = -1;
        }
        else
        {
            loadFilesMas[posFile] >> idCalls[posFile];
            qDebug() << idCalls[posFile];
            loadFilesMas[posFile] >> sizeCalls[posFile];
            qDebug() << sizeCalls[posFile];
        }
    }
    for (int i = 0; i < countFiles; i++)
        loadFilesMas[i].close();
}

int Merger::idMin()
{
    int n = -1;
    long min = 32535;
    for (int i = 0; i < countFiles; i++)
    {
        if (min > idCalls[i] && idCalls[i] > -1)
        {
            min = idCalls[i];
            n = i;
        }
    }
    return n;
}

void Merger::stopMerger()
{
    work = false;
    for (int i = 0; i < countFiles; i++)
        loadFilesMas[i].close();
    delete[] loadFilesMas;
    delete[] idCalls;
    delete[] sizeCalls;
    qDebug() << "Stop Merger!";
}

void Merger::returnBuf(QByteArray* buf)
{
    buf->clear();
    bufferLoad.append(buf);
    qDebug() << "Return buffer!";

}
