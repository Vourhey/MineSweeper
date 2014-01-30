#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class TheField;
class QAction;
class QToolBar;
class QStatusBar;
class QLabel;
class QComboBox;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

private slots:
    void aboutSlot();

private:
    void createActs();
    void createMenus();
    void createToolbar();
    void createStatusbar();

    TheField *m_field;

    QAction *newAct,
        *pauseAct,
        *showHighScoreAct,
        *quitAct;
    QAction *showToolbarAct,
        *showStatusbarAct;
    QAction *easyAct,
        *mediumAct,
        *hardAct,
        *customAct;
    QAction *aboutAct;

    QToolBar *m_toolBar;
    QStatusBar *m_statusBar;
    QLabel *minesLabel,
        *timerLabel;
    QComboBox *difficultyBox;
};

#endif // MAINWINDOW_H
