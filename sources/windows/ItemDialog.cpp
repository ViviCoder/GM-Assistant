#include "ItemDialog.h"
#include <QMessageBox>

ItemDialog::ItemDialog(QWidget *parent): QDialog(parent)
{
    setupUi(this);
}

State ItemDialog::state() const
{
    if (radioNone->isChecked())
        return sNone;
    else if (radioProgress->isChecked())
        return sProgress;
    else if (radioFailure->isChecked())
        return sFailure;
    else
        return sSuccess;
}

QString ItemDialog::text() const
{
    return editItem->text();
}

void ItemDialog::on_pushCancel_clicked()
{
    setResult(rCancel);
    close();
}

void ItemDialog::on_pushChild_clicked()
{
    if (editItem->text()!="")
    {
        setResult(rChild);
        close();
    }
    else
    {
        QMessageBox::critical(this,QApplication::translate("itemDialog","Uncomplete data",0),QApplication::translate("itemDialog","You must fill the content before validating.",0));
    }
}

void ItemDialog::on_pushBrother_clicked()
{
    if (editItem->text()!="")
    {
        setResult(rBrother);
        close();
    }
    else
    {
        QMessageBox::critical(this,QApplication::translate("itemDialog","Uncomplete data",0),QApplication::translate("itemDialog","You must fill the content before validating.",0));
    }
}
