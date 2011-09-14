#ifndef HEADER_ITEMDIALOG
#define HEADER_ITEMDIALOG

#include "ui_ItemDialog.h"
#include "Item.h"

class ItemDialog: public QDialog, private Ui::itemDialog
{
    public:
        // result type
        enum Result {rBrother,rChild};
        // constructor
        ItemDialog(QWidget *parent=NULL);
        // accessors
        Item::State state() const;
        QString text() const;
        Item::Type type() const;
        QString fileName() const;
        Result selectionResult() const;
        void updateDisplay();
    protected:
        void showEvent(QShowEvent *e);
    private:
        Result rRes;

    Q_OBJECT
    public slots:
        void on_pushCancel_clicked();
        void on_pushChild_clicked();
        void on_pushBrother_clicked();
        void on_radioBasic_clicked();
        void on_radioSound_clicked();
        void on_toolBrowse_clicked();
};

#endif
