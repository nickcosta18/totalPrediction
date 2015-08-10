#include "fileselect.h"
#include "ui_fileselect.h"

FileSelect::FileSelect(QWidget *parent, QString user) :
    QDialog(parent),
    ui(new Ui::FileSelect)
{
    ui->setupUi(this);
    this->setWindowTitle("Total Prediction");

    m_user = user;
    m_isCalibrateMode = (user == "");
    QDir dir(QDir::currentPath());

    if(m_isCalibrateMode)
    {
        foreach(QString file, dir.entryList())
        {
            if(file.right(13) == "_original.txt")
            {
                ui->fileList->addItem(file.left(file.length() - 13));
            }
        }
    }
    else
    {
        foreach(QString file, dir.entryList())
        {
            if(file.right(15) == "_calibrated.txt")
            {
                ui->fileList->addItem(file.left(file.length() - 15));
            }
        }
    }

    ui->fileList->setFocus();
    ui->fileList->setCurrentRow(0);
}

FileSelect::~FileSelect()
{
    delete ui;
}

void FileSelect::onCancel()
{
    reject();
}

void FileSelect::onSubmit()
{
    QString file = ui->fileList->currentItem()->text();

    if(m_isCalibrateMode)
    {
        file.append("_original.txt");
    }
    else
    {
        file.append("_calibrated.txt");
    }

    TestWindow *win = new TestWindow(this->parentWidget(), file, m_user);
    win->show();
    win->startOnClick();
    delete this;
}

void FileSelect::reject()
{
    this->parentWidget()->show();
    delete this;
}
