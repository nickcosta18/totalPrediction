#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Total Prediction");
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::onExitClick()
{
    exit(0);
}

void MainWindow::onInstructionsClick()
{

    m_msg = QMessageBox::information(this, "Instructions",
                                     "This application is intended to be used collaboratively by both physicians and their patients to assist with cognitive rehabilitation. Using a letter prediction exercise, this app allows the patient to continually guess the next letter in a text file, until the end is reached. \n\n"
                                     "1) Create a new exercise, or import an existing one. Note that exercises are hosted at www.totalprediction.weebly.com. \n\n"
                                     "2) Calibrate the exercise. Have a well-minded person complete the letter prediction exercise in order to calibrate the test based on their results. It is recommended that each test be calibrated 10 times. \n\n"
                                     "3) Proctor the exercise to patients. Enter the name of the patient and choose the test to give them. Instruct them how to use the software by continually guessing the next letter until the exercise is complete. \n\n"
                                     "4) You can view and edit information about the exercises you have created and patients with whom you have worked. You are also able to export exercises that you have created in order to share them with patients and other physicians.",
                                     QMessageBox::Ok);
}

void MainWindow::onCreateNewClick()
{
    CreateNew *win = new CreateNew(this);
    win->show();
    this->hide();
}

void MainWindow::onCalibrateTestClick()
{
    //Verify > 0  _original.txt files exist
    bool present = false;
    QDir dir(QDir::currentPath());
    foreach(QString file, dir.entryList())
    {
        if(file.right(13) == "_original.txt")
        {
            present = true;
            break;
        }
    }


    if(present)
    {
        FileSelect *win = new FileSelect(this, "");
        win->show();
        this->hide();
    }
    else
    {
        m_msg = QMessageBox::critical(this, "Error",
                                         "No exercises have been created yet.",
                                         QMessageBox::Ok);
    }
}

void MainWindow::onProctorTestClick()
{
    //Verify > 0  _original.txt files exist
    bool present = false;
    QDir dir(QDir::currentPath());
    foreach(QString file, dir.entryList())
    {
        if(file.right(13) == "_original.txt")
        {
            present = true;
            break;
        }
    }

    if(present)
    {
        UserSelect *win = new UserSelect(this);
        win->show();
        this->hide();
    }
    else
    {
        m_msg = QMessageBox::critical(this, "Error",
                                         "No exercises have been created yet.",
                                         QMessageBox::Ok);
    }
}


void MainWindow::onViewEditDataClick()
{
    ViewEditChoice *win = new ViewEditChoice(this);
    win->show();
    this->hide();
}




void MainWindow::on_recv_newTest(QString name)
{
    m_msg = QMessageBox::information(this, "Update",
                                     "New Exercise '" + name + "' Created!",
                                     QMessageBox::Ok);
}

void MainWindow::on_recv_importTest(QString name)
{
    m_msg = QMessageBox::information(this, "Update",
                                     "Exercise '" + name + "' Imported!",
                                     QMessageBox::Ok);
}


void MainWindow::on_recv_calibrated(QString name, int num)
{
    qDebug() << num;
    m_msg = QMessageBox::information(this, "Update",
                                     "Exercise '" + name + "' has been calibrated "
                                     + "based on " + QString::number(num) + " input(s)!",
                                     QMessageBox::Ok);
}
