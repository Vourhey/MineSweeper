#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class TheField;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

private:
    TheField *m_field;
};

#endif // MAINWINDOW_H
