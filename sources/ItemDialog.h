#ifndef HEADER_ITEMDIALOG
#define HEADER_ITEMDIALOG

#include "ui_ItemDialog.h"

class ItemDialog: public QDialog, private Ui::itemDialog
{
    public:
        // constructor
        ItemDialog(QWidget *parent=NULL);
};

#endif
