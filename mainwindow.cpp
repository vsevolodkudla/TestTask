#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->progressBar->hide();
    ui->canselButton->hide();

    ui->TableFiles->horizontalHeader()->setStretchLastSection(true);

    mergerThread = new QThread;
    recorderThread = new QThread;
    mergerWork = new Merger;
    recorderWork = new Recorder;

    mergerWork->threadSetup(mergerThread);
    recorderWork->threadSetupS(recorderThread);

    connect(mergerWork, SIGNAL(sendData(QByteArray*)), recorderWork, SLOT(addData(QByteArray*)));
    connect(recorderWork, SIGNAL(returnBuf(QByteArray*)), mergerWork, SLOT(returnBuf(QByteArray*)));
    connect(recorderWork, SIGNAL(getProgress(qint64)), this, SLOT(changeProgressBar(qint64)));

    signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(QWidget*)),
                this, SLOT(on_DeleteFileButton_Clicked(QWidget*)));
    buttonCount = 0;


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_AddFileButtonSelect_clicked()
{
    ui->AddFileEdit->setText(QFileDialog::getOpenFileName(this,
                                                          tr("Open file"), "/home"));
}

void MainWindow::on_AddFileButton_clicked()
{
    ui->TableFiles->insertRow(ui->TableFiles->rowCount());

    QWidget* pWidget = new QWidget();
    QPushButton* btn_edit = new QPushButton();
    buttonCount++;
    btn_edit->setObjectName(QString::number(buttonCount));
    QPixmap pixmap(":/Icons/Icons/delete.png");
    QIcon ButtonIcon(pixmap);
    btn_edit->setIcon(ButtonIcon);
    QHBoxLayout* pLayout = new QHBoxLayout(pWidget);
    pLayout->addWidget(btn_edit);
    pLayout->setAlignment(Qt::AlignCenter);
    pLayout->setContentsMargins(0, 0, 0, 0);
    pWidget->setLayout(pLayout);
    ui->TableFiles->setCellWidget(ui->TableFiles->rowCount() - 1, 0, pWidget);
    connect(btn_edit, SIGNAL(clicked()),
            this, SLOT(on_DeleteFileButton_Clicked()));

    connect(btn_edit, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(btn_edit, pWidget);

    QFileInfo fileInfo(ui->AddFileEdit->text());
    qint64 fileSize = fileInfo.size();
    if (fileSize > 10000000000)
    {
        fileSize /= 1024 / 1024 / 1024;
        ui->TableFiles->setItem(ui->TableFiles->rowCount() - 1, 1, new QTableWidgetItem(QString::number(fileSize) + "G"));
    }
    else
        if(fileSize > 10000000)
        {
            fileSize /= 1024 / 1024;
            ui->TableFiles->setItem(ui->TableFiles->rowCount() - 1, 1, new QTableWidgetItem(QString::number(fileSize) + "M"));
        }
    else
            if(fileSize > 10000)
            {
                fileSize /= 1024;
                ui->TableFiles->setItem(ui->TableFiles->rowCount() - 1, 1, new QTableWidgetItem(QString::number(fileSize) + "K"));
            }
    else
        ui->TableFiles->setItem(ui->TableFiles->rowCount() - 1, 1, new QTableWidgetItem(QString::number(fileInfo.size())));
    ui->TableFiles->setItem(ui->TableFiles->rowCount() - 1, 2, new QTableWidgetItem(ui->AddFileEdit->text()));
}

void MainWindow::on_DeleteFileButton_Clicked(QWidget* wdg_btn)
{
    int i = 0;
    while(ui->TableFiles->cellWidget(i,0) != wdg_btn)
        i++;
    ui->TableFiles->removeRow(i);
}

void MainWindow::on_MergeFileButtonSelect_clicked()
{
    ui->MergeFileEdit->setText(QFileDialog::getSaveFileName(this,
                                                            tr("Save file"), "/home"));
}

void MainWindow::on_MergeFilesButton_clicked()
{
    allFileSize = 0;
    filesLoad.clear();
    for (int i = 0; i < ui->TableFiles->rowCount(); i++)
    {
        filesLoad.append(ui->TableFiles->item(i, 2)->text());
        QFileInfo fileInfo(ui->TableFiles->item(i, 2)->text());
        allFileSize += fileInfo.size();
    }
    ui->progressBar->setValue(0);
    ui->progressBar->show();
    ui->canselButton->show();
    ui->MergeFilesButton->hide();

    ofstream *saveFile = new ofstream;
    saveFile->open(ui->MergeFileEdit->text().toStdString().c_str());
    recorderWork->setFile(saveFile);
    mergerWork->setFiles(filesLoad);


    mergerThread->start();
    recorderThread->start();

    qDebug() << "GUI work";
}

void MainWindow::on_exitButton_clicked()
{
    close();
}

void MainWindow::on_canselButton_clicked()
{
    if (!mergerThread->isFinished())
        mergerThread->quit();
    if (!recorderThread->isFinished())
       recorderThread->quit();

    ui->progressBar->hide();
    ui->canselButton->hide();
    ui->MergeFilesButton->show();
}


void MainWindow::changeProgressBar(qint64 progress)
{
    qDebug() << "progress: " << progress << "; Size: " << allFileSize;
    ui->progressBar->setValue(((float)allFileSize / progress)*100);

    if(progress >= allFileSize)
    {
        ui->progressBar->hide();
        ui->canselButton->hide();
        ui->MergeFilesButton->show();
        QMessageBox::information(this, "Результат.","Всі дані злито в один файл.");

        if (!mergerThread->isFinished())
            mergerThread->quit();
        if (!recorderThread->isFinished())
           recorderThread->quit();

    }
}
