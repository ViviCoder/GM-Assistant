#include "ItemDialog.h"
#include <QMessageBox>

ItemDialog::ItemDialog(QWidget *parent): QDialog(parent)
{
    setupUi(this);
}

Item::State ItemDialog::state() const
{
    if (radioNone->isChecked())
        return Item::sNone;
    else if (radioProgress->isChecked())
        return Item::sProgress;
    else if (radioFailure->isChecked())
        return Item::sFailure;
    else
        return Item::sSuccess;
}

QString ItemDialog::text() const
{
    return editItem->text();
}

void ItemDialog::on_pushCancel_clicked()
{
    reject();
}

void ItemDialog::on_pushChild_clicked()
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

void ItemDialog::on_pushBrother_clicked()
{
    if (editItem->text()!="")
    {
        rRes = rBrother;
        accept();
    }
    else
    {
        QMessageBox::critical(this,QApplication::translate("itemDialog","Uncomplete data",0),QApplication::translate("itemDialog","You must fill the content before validating.",0));
    }
}

void ItemDialog::showEvent(QShowEvent *e)
{
    // reset the window
    radioNone->setChecked(true);
    editItem->setText("");

    QDialog::showEvent(e);
}

ItemDialog::Result ItemDialog::selectionResult() const
{
    return rRes;
}
