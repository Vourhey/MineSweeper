#ifndef THEFIELD_H
#define THEFIELD_H

#include <QWidget>

class QTimer;

class TheField : public QWidget
{
    Q_OBJECT
public:
    TheField(QWidget *parent = 0);
    ~TheField();

    QTimer *timer() const;

signals:
    void flagSet(int number, int mines);
    void resetTime();

public slots:    
    void newGame();
    void pause(bool b);
    void setDifficulty(int d = -1);

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent *ev);
    void resizeEvent(QResizeEvent *ev);
    
private:
    void randomMines();
    void setNumbers();
    bool isValid(int x, int y);
    qint8 nearMines(int x, int y);
    void cellFromPos(int x, int y, int *toX, int *toY);
    void discoverNear(int x, int y);
    void freeField();
    void gameWon();
    void gameOver();

    struct Cell
    {
        bool isMine;
        bool isHidden;
        bool isFlag;
        qint8 mines;
    };

    Cell **m_field;
    QTimer *m_timer;

    bool isReadOnly;

    qint8 m_row;
    qint8 m_column;
    qint8 m_mines;
    qint8 m_difficulty;
    int restOfCells;
    qint8 numberOfFlags;

    int lastX;
    int lastY;

    int squareSize;
    int offset;
};

#endif // THEFIELD_H
