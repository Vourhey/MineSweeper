#include <QPainter>
#include <QTime>
#include <QtGlobal>
#include <QMouseEvent>

#include "thefield.h"

#include <stdio.h>

TheField::TheField(QWidget *parent)
    : QWidget(parent)
{
    m_field = 0;
    setFixedSize(160, 160);

    newGame();
}

TheField::~TheField()
{
    for(int i = 0; i < 8; ++i) {
        delete [] m_field[i];
    }
    delete [] m_field;
}

// field is 8x8 cells
void TheField::newGame()
{
    m_field = new Cell*[8];
    for(int i = 0; i < 8; ++i) {
        m_field[i] = new Cell[8];

        for(int j = 0; j < 8; j++) {
            m_field[i][j].isMine    = false;
            m_field[i][j].isHidden  = true;
            m_field[i][j].isFlag    = false;
            m_field[i][j].mines     = 0;
        }
    }

    randomMines();
    setNumbers();

    // DEBUG
    for(int i = 0; i < 8; ++i) {
        for(int j = 0; j < 8; ++j) {
            if(m_field[i][j].isMine) {
                printf("m ");
            } else {
                printf("%d ", m_field[i][j].mines);
            }
        }
        printf("\n");
    }
}

// add 10 bombs to the field
void TheField::randomMines()
{
    int n = 10;
    int x, y;

    qsrand(QTime::currentTime().msec());
    while(n != 0) {
        x = qrand() % 8;
        y = qrand() % 8;

        while(m_field[y][x].isMine) {   // it's a bad algorithm
            x = qrand() % 8;    // i'm looking for a new position
            y = qrand() % 8;
        }

        m_field[y][x].isMine = true;
        --n;
    }
}

bool TheField::isValid(int x, int y)
{
    if(y < 0 || x < 0 || y == 8 || x == 8) {
        return false;
    }

    return true;
}

qint8 TheField::nearMines(int x, int y)
{
    int count = 0;
    int i, j;

    for(i = y - 1; i <= (y+1); ++i) {
        for(j = x - 1; j <= (x+1); ++j) {
            if(isValid(j, i)) {
                if(m_field[i][j].isMine) {
                    ++count;
                }
            }
        }
    }

    return count;
}

void TheField::setNumbers()
{
    for(int i = 0; i < 8; ++i) {
        for(int j = 0; j < 8; ++j) {
            if(!m_field[i][j].isMine) {
                m_field[i][j].mines = nearMines(j, i);
            }
        }
    }
}

void TheField::cellFromPos(int x, int y, int *toX, int *toY)
{
    *toX = x / 20;
    *toY = y / 20;
}

void TheField::discoverNear(int x, int y)
{
    for(int i = y - 1; i <= (y+1); ++i) {
        for(int j = x - 1; j <= (x+1); ++j) {
            
            if(isValid(j, i) && m_field[i][j].isHidden) {
                m_field[i][j].isHidden = false;

                if(!m_field[i][j].mines) {
                    discoverNear(j, i);
                }
            }

        }
    }
}

void TheField::mousePressEvent(QMouseEvent *ev)
{
    cellFromPos(ev->x(), ev->y(), &lastX, &lastY);
    printf("lastX = %d lastY = %d\n", lastX, lastY);
}

void TheField::mouseReleaseEvent(QMouseEvent *ev)
{
    int x, y;
    
    cellFromPos(ev->x(), ev->y(), &x, &y);

    printf("x = %d y = %d\n", x, y);
    if(x == lastX && y == lastY) {
        if(ev->button() == Qt::LeftButton) {

            if(m_field[y][x].isFlag) {
                return;
            }
            
            if(m_field[y][x].isMine) {

                for(int i = 0; i < 8; ++i) {
                    for(int j = 0; j < 8; ++j) {
                        if(m_field[i][j].isMine) {
                            m_field[i][j].isHidden = false;
                        }
                    }
                }
                // gameOver(); 
            } else {
                m_field[y][x].isHidden = false;

                if(!m_field[y][x].mines) {
                    discoverNear(x, y);
                }
            }

        } else if(ev->button() == Qt::RightButton) {
            m_field[y][x].isFlag = !m_field[y][x].isFlag;
        }
        update();
    }
}

void TheField::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev)

    QPainter p(this);
    int i, j;

    int squareSize = 20;

    for(i = 0; i < 8; ++i) {
        for(j = 0; j < 8; ++j) {

            if(m_field[i][j].isHidden) {
                p.setBrush(Qt::gray);
                if(m_field[i][j].isFlag) {
                    p.setBrush(Qt::blue);
                }
            } else if(m_field[i][j].isMine) {
                p.setBrush(Qt::red);
            }

            p.drawRect(j*squareSize, i*squareSize, squareSize, squareSize);
            if(!m_field[i][j].isHidden && m_field[i][j].mines) {
                p.drawText(j*squareSize, i*squareSize+squareSize, QString::number(m_field[i][j].mines));
            }
            p.setBrush(Qt::white);

        }
    }
}

