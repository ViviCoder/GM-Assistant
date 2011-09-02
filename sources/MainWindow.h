#ifndef HEADER_MAINWINDOW
#define HEADER_MAINWINDOW

#include "ui_MainWindow.h"
#include "Engine.h"

class MainWindow : public QMainWindow, private Ui::mainWindow
{
    private:
        Engine eGame;
        QString fileName;
    public:
        // constructor
        MainWindow();

    Q_OBJECT
    private slots:
        void on_action_Quit_triggered();
        void on_action_Load_triggered();
        void on_action_Save_triggered();
        void on_actionS_ave_as_triggered();
        void on_textNotes_textChanged();
};

#endif
