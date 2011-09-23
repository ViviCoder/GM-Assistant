#ifndef HEADER_CHANGEHEADERDIALOG
#define HEADER_CHANGEHEADERDIALOG

#include "ui_ChangeHeaderDialog.h"

class ChangeHeaderDialog: public QDialog, private Ui::changeHeaderDialog
{
    public:
        // result type
        // constructor
        ChangeHeaderDialog(QWidget *parent=NULL);
        // accessors
        QString text() const;

};

#endif
