#include "createnew.h"
#include "ui_createnew.h"

CreateNew::CreateNew(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateNew)
{
    ui->setupUi(this);
    submit = false;

    this->setWindowTitle("Total Prediction");
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
                                                 "A test by this name already exists. Would you like to overwrite it?",
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
                                         "You must provide a name for the test.",
                                         QMessageBox::Ok);
        }
        else
        {
            QMessageBox::critical(this, "Error",
                                         "You must input text for this test",
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
