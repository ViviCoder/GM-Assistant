#include "ItemDialog.h"

ItemDialog::ItemDialog(QWidget *parent): QDialog(parent)
{
    setupUi(this);
}

void ItemDialog::on_pushCancel_clicked()
{
    setResult(rCancel);
}

void ItemDialog::on_pushChild_clicked()
{
    setResult(rChild);
}

void ItemDialog::on_pushBrother_clicked()
{
    setResult(rBrother);
}
