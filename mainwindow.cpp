#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>

#include "mainwindow.h"
#include "thefield.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_field = new TheField;
    setCentralWidget(m_field);

    createActs();
    createMenus();
    createToolbar();
    createStatusbar();
}

void MainWindow::createActs()
{
    newAct = new QAction(tr("New"), this);
    newAct->setShortcut(QKeySequence::New);
    connect(newAct, SIGNAL(triggered()), m_field, SLOT(newGame()));

    pauseAct = new QAction(tr("Pause"), this);
    pauseAct->setCheckable(true);
    pauseAct->setShortcut(Qt::Key_P);

    showHighScoreAct = new QAction(tr("Show High Scores"), this);

    quitAct = new QAction(tr("Quit"), this);
    quitAct->setShortcut(QKeySequence::Quit);
    connect(quitAct, SIGNAL(triggered()), SLOT(close()));

    showToolbarAct = new QAction(tr("Show Toolbar"), this);
    showToolbarAct->setCheckable(true);
    showToolbarAct->setChecked(true);

    showStatusbarAct = new QAction(tr("Show Statusbar"), this);
    showStatusbarAct->setCheckable(true);
    showStatusbarAct->setChecked(true);

    aboutAct = new QAction(tr("About"), this);
    aboutAct->setShortcut(QKeySequence::HelpContents);
    connect(aboutAct, SIGNAL(triggered()), SLOT(aboutSlot()));
}

void MainWindow::createMenus()
{
    QMenuBar *mBar = menuBar();

    QMenu *m = mBar->addMenu(tr("Game"));
    m->addAction(newAct);
    m->addSeparator();
    m->addAction(pauseAct);
    m->addAction(showHighScoreAct);
    m->addSeparator();
    m->addAction(quitAct);

    m = mBar->addMenu(tr("Settings"));
    m->addAction(showToolbarAct);
    m->addAction(showStatusbarAct);
    m->addSeparator();
    m->addMenu(tr("Difficulty"));

    m = mBar->addMenu(tr("Help"));
    m->addAction(aboutAct);
}

void MainWindow::createToolbar()
{
    m_toolBar = addToolBar(tr("Toolbar"));
    m_toolBar->addAction(newAct);
    m_toolBar->addAction(pauseAct);

    connect(showToolbarAct, SIGNAL(toggled(bool)), m_toolBar, SLOT(setVisible(bool)));
}

void MainWindow::createStatusbar()
{
    m_statusBar = statusBar();
    connect(showStatusbarAct, SIGNAL(toggled(bool)), m_statusBar, SLOT(setVisible(bool)));
}

void MainWindow::aboutSlot()
{
    QMessageBox::about(this, tr("About"), tr("MineSweeper"));
}

