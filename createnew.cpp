#include "createnew.h"
#include "ui_createnew.h"

CreateNew::CreateNew(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateNew)
{
    ui->setupUi(this);
    submit = false;

    this->setWindowTitle("Total Prediction");

    ui->headingLabel->setStyleSheet("font-family: EA Sports Covers SC;color: rgb(48, 76, 135);font-size: 45px;");
    ui->headingLabel2->setStyleSheet("font-family: EA Sports Covers SC;color: rgb(48, 76, 135);font-size: 45px;");
    ui->importButton->setStyleSheet("font-family: EA Sports Covers SC;color: rgb(48, 76, 135);font-size: 25px;");

}

CreateNew::~CreateNew()
{
    delete ui;
}


void CreateNew::onSubmit()
{
    if(ui->name->text() != "" && ui->contents->toPlainText() != "")
    {
        //Check if name already exists
        bool found = false;
        QMessageBox::StandardButton reply = QMessageBox::No;
        QDir dir(QDir::currentPath());
        foreach(QString file, dir.entryList())
        {
            if(file.right(13) == "_original.txt" && file.left(file.length() - 13) == ui->name->text())
            {
                found = true;
                reply = QMessageBox::question(this, "Overwrite?",
                                                 "An exercise by this name already exists. Would you like to overwrite it?",
                                                 QMessageBox::Yes | QMessageBox::No);

                break;
            }
        }

        if(!found || reply == QMessageBox::Yes)
        {
            submit = true;
            reject();
        }
    }
    else
    {
        if(ui->name->text() == "")
        {
            QMessageBox::critical(this, "Error",
                                         "You must provide a name for the exercise.",
                                         QMessageBox::Ok);
        }
        else
        {
            QMessageBox::critical(this, "Error",
                                         "You must input text for this exercise.",
                                         QMessageBox::Ok);
        }
    }
}

void CreateNew::onCancel()
{
    reject();
}

void CreateNew::reject()
{

    this->parentWidget()->show();

    if(submit)
    {
        //create .txt file
        QFile file( ui->name->text() + "_original.txt" );
        if ( file.open(QIODevice::WriteOnly | QIODevice::Text) )
        {
            QTextStream stream( &file );
            QString originalText = ui->contents->toPlainText();
            QString formattedText = "";

            //remove newline characters

            for(int i = 0; i < originalText.length(); i++){
                if (originalText[i] != QLatin1Char('\n')){
                    formattedText.append(originalText[i]);
                }else{
                    if(originalText[i-1] != QLatin1Char(' ') && i+1<originalText.length()){
                        formattedText.append(QLatin1Char(' '));
                    }
                }
            }

            stream << formattedText;
            file.close();
        }





        //Also create .txt file to keep track of missesed Indicies
        QFile file2( ui->name->text() + "_missed.txt" );
        if ( file2.open(QIODevice::WriteOnly | QIODevice::Text) )
        {
            QTextStream stream( &file2 );
            QString text = "0";
            stream << text;
            file2.close();
        }


        //remove _calibrated.txt if it exists
        QFile file3( ui->name->text() + "_calibrated.txt" );
        file3.remove();


        this->hide();
        connect(this, SIGNAL(send_newTest(QString)), this->parent(), SLOT(on_recv_newTest(QString)));
        emit send_newTest(ui->name->text());
    }

    delete this;
}



void CreateNew::onImport()
{
    QString desktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString importFile = QFileDialog::getOpenFileName(this, "Import File", desktop, "Text File (*.txt)");

    //On Cancel
    if(importFile == "")
    {
        return;
    }


    QStringList pathParts = importFile.split("/");
    QString fileBase = pathParts[pathParts.length() - 1].left(pathParts[pathParts.length() - 1].length() - 4);


    //Check if name already exists
    bool found = false;
    QMessageBox::StandardButton reply = QMessageBox::No;
    QDir dir(QDir::currentPath());
    foreach(QString file, dir.entryList())
    {
        if(file.right(13) == "_original.txt" && file.left(file.length() - 13) == fileBase)
        {
            found = true;
            reply = QMessageBox::question(this, "Overwrite?",
                                             "An exercise by this name already exists. Would you like to overwrite it?",
                                             QMessageBox::Yes | QMessageBox::No);

            break;
        }
    }

    if( found && reply == QMessageBox::No)
    {
        return;
    }








    QString fileText;

    QFile file( importFile );
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file);
        fileText = in.readAll();
        file.close();
    }else{
        QMessageBox::critical(this, "Error",
                              "There was an error importing this file.",
                               QMessageBox::Ok);
        return;
    }

    QStringList parts = fileText.split("<>");
    bool calibratePresent;
    if(parts.length() == 5)
    {
        calibratePresent = true;
    }
    else if(parts.length() == 4)
    {
        calibratePresent = false;
    }
    else
    {
        QMessageBox::critical(this, "Error",
                              "There was an error importing this file.",
                               QMessageBox::Ok);
        return;
    }

    QFile file2( fileBase + "_original.txt" );
    if ( file2.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QTextStream stream( &file2 );
        stream << parts[1];
        file2.close();
    }else{
        QMessageBox::critical(this, "Error",
                              "There was an error importing this file.",
                               QMessageBox::Ok);
        return;
    }


    QFile file3( fileBase + "_missed.txt" );
    if ( file3.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QTextStream stream( &file3 );
        stream << parts[2];
        file3.close();
    }else{
        QMessageBox::critical(this, "Error",
                              "There was an error importing this file.",
                               QMessageBox::Ok);
        return;
    }

    if(calibratePresent)
    {
        QFile file3( fileBase + "_calibrated.txt" );
        if ( file3.open(QIODevice::WriteOnly | QIODevice::Text) )
        {
            QTextStream stream( &file3 );
            stream << parts[3];
            file3.close();
        }else{
            QMessageBox::critical(this, "Error",
                                  "There was an error importing this file.",
                                   QMessageBox::Ok);
            return;
        }
    }


    this->parentWidget()->show();
    this->hide();
    connect(this, SIGNAL(send_importTest(QString)), this->parent(), SLOT(on_recv_importTest(QString)));
    emit send_importTest(fileBase);
    delete this;

}
