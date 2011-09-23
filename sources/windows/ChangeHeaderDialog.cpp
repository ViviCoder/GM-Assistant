#include "ChangeHeaderDialog.h"
#include <QMessageBox>
#include <QFileDialog>

ChangeHeaderDialog::ChangeHeaderDialog(QWidget *parent): QDialog(parent)
{
    setupUi(this);
}

QString ChangeHeaderDialog::text() const
{
    return editItem->text();
}


/*
void ChangeHeaderDialog::on_pushCancel_clicked()
{
    reject();
}

void ChangeHeaderDialog::on_pushEdit_clicked()
{
    if (editItem->text()!="")
    {
        rRes = rChild;
        accept();
    }
    else
    {
        QMessageBox::critical(this,QApplication::translate("itemDialog","Uncomplete data",0),QApplication::translate("itemDialog","You must fill the content before validating.",0));
    }
}
*/
