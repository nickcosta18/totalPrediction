#ifndef CREATENEW_H
#define CREATENEW_H

#include <QDialog>

#include <QDir>
#include <QMessageBox>

#include <QDebug>

namespace Ui {
class CreateNew;
}

class CreateNew : public QDialog
{
    Q_OBJECT

public:
    explicit CreateNew(QWidget *parent);
    ~CreateNew();

private:
    Ui::CreateNew *ui; 

    void reject();
    bool submit;

private slots:
    void onCancel();
    void onSubmit();

signals:
    void send_newTest(QString name);
};

#endif // CREATENEW_H
