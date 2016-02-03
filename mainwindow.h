#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "merger.h"
#include "recorder.h"

#include <QMainWindow>
#include <QList>
#include <QFileDialog>
#include <QStringList>
#include <QDebug>
#include <QSignalMapper>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_AddFileButtonSelect_clicked();

    void on_AddFileButton_clicked();

    void on_MergeFileButtonSelect_clicked();

    void on_MergeFilesButton_clicked();

    void on_DeleteFileButton_Clicked(QWidget*);

    void on_exitButton_clicked();

    void on_canselButton_clicked();

    void changeProgressBar(qint64);

signals:
    void startProcess(QString);

private:
    Ui::MainWindow *ui;
    QSignalMapper *signalMapper;
    int buttonCount;
    QList<QString> filesLoad;

    QThread *mergerThread;
    QThread *recorderThread;
    Merger *mergerWork;
    Recorder *recorderWork;
    qint64 allFileSize = 0;
};

#endif // MAINWINDOW_H
