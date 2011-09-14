#include "ItemDialog.h"
#include <QMessageBox>
#include <QFileDialog>

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

Item::Type ItemDialog::type() const
{
    if (radioBasic->isChecked())
        return Item::tBasic;
    else return Item::tSound;
}

QString ItemDialog::fileName() const
{
    return editFile->text();
}

void ItemDialog::on_pushCancel_clicked()
{
    reject();
}

void ItemDialog::on_pushChild_clicked()
{
    if (editItem->text()!="")
    {
        if (radioBasic->isChecked() || editFile->text()!="")
        {
            rRes = rChild;
            accept();
        }
        else
        {
        QMessageBox::critical(this,QApplication::translate("itemDialog","Uncomplete data",0),QApplication::translate("itemDialog","You must select a file before validating.",0));
        }
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
        if (radioBasic->isChecked() || editFile->text()!="")
        {
            rRes = rBrother;
            accept();
        }
        else
        {
        QMessageBox::critical(this,QApplication::translate("itemDialog","Uncomplete data",0),QApplication::translate("itemDialog","You must select a file before validating.",0));
        }
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
    radioBasic->setChecked(true);
    updateDisplay();
    editItem->setText("");
    editFile->setText("");

    QDialog::showEvent(e);
}

ItemDialog::Result ItemDialog::selectionResult() const
{
    return rRes;
}

void ItemDialog::on_radioBasic_clicked()
{
    updateDisplay();
}

void ItemDialog::on_radioSound_clicked()
{
    updateDisplay();
}

void ItemDialog::updateDisplay()
{
    toolBrowse->setEnabled(radioSound->isChecked());
}

void ItemDialog::on_toolBrowse_clicked()
{
    editFile->setText(QFileDialog::getOpenFileName(this,QApplication::translate("itemDialog","Select the sound file to associate to the item",0),"",QApplication::translate("itemDialog","Audio files (*.mp3 *.wav *.ogg)",0)));    
}
