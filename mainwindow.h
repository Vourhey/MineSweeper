#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class TheField;
class QAction;
class QToolBar;
class QStatusBar;
class QLabel;
class QComboBox;
class HighScore;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

private slots:
    void aboutSlot();
    void updateFlags(int flags, int mines);
    void updateTime();
    void difficulty(int item);
    void resetTime();
    void showHighScore();
    void addToHighScore();

private:
    void createActs();
    void createMenus();
    void createToolbar();
    void createStatusbar();

    TheField *m_field;
    HighScore *m_highScore;
    int m_time;

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
