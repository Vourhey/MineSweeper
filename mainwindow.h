#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class TheField;
class QAction;
class QToolBar;
class QStatusBar;

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
    QAction *aboutAct;

    QToolBar *m_toolBar;
    QStatusBar *m_statusBar;
};

#endif // MAINWINDOW_H
