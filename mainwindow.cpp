#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QLabel>
#include <QComboBox>
#include <QTimer>

#include "mainwindow.h"
#include "thefield.h"
#include "highscore.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_highScore = 0;

    m_field = new TheField;
    connect(m_field, SIGNAL(flagSet(int,int)), SLOT(updateFlags(int,int)));
    connect(m_field->timer(), SIGNAL(timeout()), SLOT(updateTime()));
    connect(m_field, SIGNAL(resetTime()), SLOT(resetTime()));
    connect(m_field, SIGNAL(addScore()), SLOT(addToHighScore()));
    m_time = 0;
    resize(400, 500);
    setCentralWidget(m_field);

    createActs();
    createMenus();
    createToolbar();
    createStatusbar();

    m_field->setDifficulty(1);
}

void MainWindow::createActs()
{
    newAct = new QAction(tr("New"), this);
    newAct->setShortcut(QKeySequence::New);
    connect(newAct, SIGNAL(triggered()), m_field, SLOT(newGame()));

    pauseAct = new QAction(tr("Pause"), this);
    pauseAct->setCheckable(true);
    pauseAct->setShortcut(Qt::Key_P);
    connect(pauseAct, SIGNAL(toggled(bool)), m_field, SLOT(pause(bool)));

    showHighScoreAct = new QAction(tr("Show High Scores"), this);
    connect(showHighScoreAct, SIGNAL(triggered()), SLOT(showHighScore()));

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

    minesLabel = new QLabel(this);
    m_statusBar->addPermanentWidget(minesLabel);

    timerLabel = new QLabel(tr("Time: 00:00"), this);
    m_statusBar->addPermanentWidget(timerLabel);

    difficultyBox = new QComboBox(this);
    difficultyBox->addItem(tr("Easy"));
    difficultyBox->addItem(tr("Medium"));
    difficultyBox->addItem(tr("Hard"));
    difficultyBox->addItem(tr("Custom"));
    m_statusBar->addPermanentWidget(difficultyBox);
    connect(difficultyBox, SIGNAL(activated(int)), SLOT(difficulty(int)));
}

void MainWindow::updateFlags(int flags, int mines)
{
    minesLabel->setText(tr("Mines: %1/%2").arg(flags).arg(mines));
}

void MainWindow::updateTime()
{
    ++m_time;

    int min = m_time / 60;
    int sec = m_time % 60;
    timerLabel->setText(tr("Time: %1:%2").arg(min).arg(sec));
}

void MainWindow::difficulty(int item)
{
    m_field->setDifficulty(item + 1);
}

void MainWindow::resetTime()
{
    m_time = 0;
    timerLabel->setText(tr("Time: 00:00"));
}

void MainWindow::showHighScore()
{
    if(m_highScore == 0) {
        m_highScore = new HighScore(this);
    }

    m_highScore->exec();
}

void MainWindow::addToHighScore()
{
    if(m_highScore == 0) {
        m_highScore = new HighScore(this);
    }

    m_highScore->exec();
}

void MainWindow::aboutSlot()
{
    QMessageBox::about(this, tr("About"), tr("MineSweeper"));
}

