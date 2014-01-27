#include "mainwindow.h"
#include "thefield.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_field = new TheField;
    setCentralWidget(m_field);
}

