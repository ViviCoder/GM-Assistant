#ifndef HEADER_ITEMDIALOG
#define HEADER_ITEMDIALOG

#include "ui_ItemDialog.h"
#include "Item.h"

class ItemDialog: public QDialog, private Ui::itemDialog
{
    public:
        // result type
        enum {rBrother,rChild,rCancel} Result;
        // constructor
        ItemDialog(QWidget *parent=NULL);
        // accessors
        State state() const;
        QString text() const;

    Q_OBJECT
    public slots:
        void on_pushCancel_clicked();
        void on_pushChild_clicked();
        void on_pushBrother_clicked();
};

#endif
