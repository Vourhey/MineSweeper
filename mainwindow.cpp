#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QLabel>
#include <QComboBox>

#include "mainwindow.h"
#include "thefield.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_field = new TheField;
    resize(400, 500);
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

    easyAct = new QAction(tr("Easy"), this);
    easyAct->setCheckable(true);
    easyAct->setData(1);
    connect(easyAct, SIGNAL(triggered()), m_field, SLOT(setDifficulty()));

    mediumAct = new QAction(tr("Medium"), this);
    mediumAct->setCheckable(true);
    mediumAct->setData(2);
    connect(mediumAct, SIGNAL(triggered()), m_field, SLOT(setDifficulty()));

    hardAct = new QAction(tr("Hard"), this);
    hardAct->setCheckable(true);
    hardAct->setData(3);
    connect(hardAct, SIGNAL(triggered()), m_field, SLOT(setDifficulty()));

    customAct = new QAction(tr("Custom"), this);
    customAct->setCheckable(true);
    customAct->setData(4);
    connect(customAct, SIGNAL(triggered()), m_field, SLOT(setDifficulty()));

    QActionGroup *actGroup = new QActionGroup(this);
    actGroup->addAction(easyAct);
    actGroup->addAction(mediumAct);
    actGroup->addAction(hardAct);
    actGroup->addAction(customAct);
    easyAct->setChecked(true);

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
    m = m->addMenu(tr("Difficulty"));
    m->addAction(easyAct);
    m->addAction(mediumAct);
    m->addAction(hardAct);
    m->addAction(customAct);

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

    minesLabel = new QLabel(tr("Mines: %d/%d"), this);
    m_statusBar->addPermanentWidget(minesLabel);

    timerLabel = new QLabel(tr("Time: 00:50"), this);
    m_statusBar->addPermanentWidget(timerLabel);

    difficultyBox = new QComboBox(this);
    difficultyBox->addItem(tr("Easy"));
    difficultyBox->addItem(tr("Medium"));
    difficultyBox->addItem(tr("Hard"));
    difficultyBox->addItem(tr("Custom"));
    m_statusBar->addPermanentWidget(difficultyBox);
}

void MainWindow::aboutSlot()
{
    QMessageBox::about(this, tr("About"), tr("MineSweeper"));
}

