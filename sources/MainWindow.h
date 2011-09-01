#ifndef HEADER_MAINWINDOW
#define HEADER_MAINWINDOW

#include "ui_MainWindow.h"
#include "Engine.h"

class MainWindow : public QMainWindow, private Ui::mainWindow
{
    private:
        Engine eGame;
    public:
        // constructor
        MainWindow();

    Q_OBJECT
    private slots:
        void on_action_Quit_triggered();
};

#endif
