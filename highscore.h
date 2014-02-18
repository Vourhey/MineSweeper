#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <QDialog>

class QTabWidget;

class HighScore : public QDialog
{
    Q_OBJECT
public:
    HighScore(QWidget *parent = 0);

private:
    void createPage(const QString &lbl);
    QWidget *createLine(const QString &name = QString(), const QString &time = QString());

    QTabWidget *tabWidget;
};

#endif  // HIGHSCORE_H
