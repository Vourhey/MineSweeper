#ifndef THEFIELD_H
#define THEFIELD_H

#include <QWidget>

class TheField : public QWidget
{
    Q_OBJECT
public:
    TheField(QWidget *parent = 0);
    ~TheField();
    
    void newGame();

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent *ev);
    
private:
    void randomMines();
    void setNumbers();
    bool isValid(int x, int y);
    qint8 nearMines(int x, int y);
    void cellFromPos(int x, int y, int *toX, int *toY);
    void discoverNear(int x, int y);

    struct Cell
    {
        bool isMine;
        bool isHidden;
        bool isFlag;
        qint8 mines;
    };

    Cell **m_field;

    int lastX;
    int lastY;
};

#endif // THEFIELD_H
