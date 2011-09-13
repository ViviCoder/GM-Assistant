#include "AboutDialog.h"
#include <QMessageBox>

AboutDialog::AboutDialog(QWidget *parent): QDialog(parent)
{
    setupUi(this);
}

void AboutDialog::on_pushClose_clicked()
{
    close();
}
