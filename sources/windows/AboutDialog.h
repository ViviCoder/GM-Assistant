#ifndef HEADER_ABOUTDIALOG
#define HEADER_ABOUTDIALOG

#include "ui_AboutDialog.h"

class AboutDialog: public QDialog, private Ui::AboutDialog
{
    public:
        AboutDialog(QWidget *parent=NULL);

    Q_OBJECT
    public slots:
        void on_pushClose_clicked();
};

#endif
