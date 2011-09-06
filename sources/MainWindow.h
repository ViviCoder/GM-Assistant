#ifndef HEADER_MAINWINDOW
#define HEADER_MAINWINDOW

#include "ui_MainWindow.h"
#include "Engine.h"

class MainWindow : public QMainWindow, private Ui::mainWindow
{
    private:
        Engine eGame;
        QString sFileName;
        bool bModified;
    public:
        // constructor
        MainWindow();

    Q_OBJECT
    private slots:
        // actions
        void on_action_Quit_triggered();
        void on_action_Load_triggered();
        void on_action_Save_triggered();
        void on_actionS_ave_as_triggered();
        void on_action_New_triggered();
        // textNotes
        void on_textNotes_textChanged();
        // general slots
        void updateDisplay();
};

#endif
