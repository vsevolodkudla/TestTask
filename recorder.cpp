#include "recorder.h"

Recorder::Recorder(QObject *parent) :
    QThread(parent)
{
}
void Recorder::threadSetupS(QThread *thread)
{
    connect(thread, SIGNAL(started()), this, SLOT(startRecorder()));
    connect(thread, SIGNAL(finished()), this, SLOT(stopRecorder()));
}

void Recorder::setFile(ofstream *save)
{
    saveFile = save;
    progress = 0;
}

void Recorder::addData(QByteArray* buf)
{
    bufferSave.append(buf);
    this->start();
}

void Recorder::startRecorder()
{
    qDebug() << "Start 2 thread";
    QByteArray *buf;
    while(bufferSave.size() > 0)
    {
        buf = bufferSave.takeFirst();
        saveFile->write(buf->data(), buf->size());

        progress += buf->size();
        emit returnBuf(buf);
        emit getProgress(progress);
    }
}

void Recorder::stopRecorder()
{
    saveFile->close();
    delete saveFile;
    qDebug() << "Stop recording!";
}
