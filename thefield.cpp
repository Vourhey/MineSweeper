#include <QPainter>
#include <QTime>
#include <QtGlobal>
#include <QMouseEvent>
#include <QAction>
#include <QMessageBox>
#include <QTimer>

#include "thefield.h"

#include <stdio.h>

TheField::TheField(QWidget *parent)
    : QWidget(parent)
{
    m_field = 0;
    m_difficulty = 0;
    m_row = m_column = 0;

    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
}

TheField::~TheField()
{
    freeField();
}

void TheField::freeField()
{
    for(int i = 0; i < m_row; ++i) {
        delete [] m_field[i];
    }
    delete [] m_field;
}

QTimer *TheField::timer() const
{
    return m_timer;
}

void TheField::pause(bool b)
{
    if(b) {
        m_timer->stop();
    } else {
        m_timer->start();
    }
}

// field is m_row * m_column cells
void TheField::newGame()
{
    isReadOnly = false;
    restOfCells = m_row * m_column - m_mines;
    printf("%d\n", restOfCells);
    numberOfFlags = 0;
    emit flagSet(0, m_mines);
    emit resetTime();

    for(int i = 0; i < m_row; ++i) {
        for(int j = 0; j < m_column; j++) {
            m_field[i][j].isMine    = false;
            m_field[i][j].isHidden  = true;
            m_field[i][j].isFlag    = false;
            m_field[i][j].mines     = 0;
        }
    }

    randomMines();
    setNumbers();
    repaint();

    // DEBUG
    for(int i = 0; i < m_row; ++i) {
        for(int j = 0; j < m_column; ++j) {
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
    int n = m_mines;
    int x, y;

    qsrand(QTime::currentTime().msec());
    while(n != 0) {
        x = qrand() % m_column;
        y = qrand() % m_row;

        while(m_field[y][x].isMine) {   // it's a bad algorithm
            x = qrand() % m_column;    // i'm looking for a new position
            y = qrand() % m_row;
        }

        m_field[y][x].isMine = true;
        --n;
    }
}

bool TheField::isValid(int x, int y)
{
    if(y < 0 || x < 0 || y >= m_row || x >= m_column) {
        return false;
    }

    return true;
}

// how much are there a near mines
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
    for(int i = 0; i < m_row; ++i) {
        for(int j = 0; j < m_column; ++j) {
            if(!m_field[i][j].isMine) {
                m_field[i][j].mines = nearMines(j, i);
            }
        }
    }
}

void TheField::setDifficulty(int d)
{
    if(d == -1) {
        QAction *act = qobject_cast<QAction*>(sender());
        d = act->data().toInt();
    }

    if(d == m_difficulty) {
        newGame();
        return;
    }

    m_difficulty = d;
    freeField();

    switch(d) {
    case 1:     // Easy
        m_row = m_column = 9;
        m_mines = 10;
        break;
    case 2:     // Medium
        m_row = m_column = 16;
        m_mines = 40;
        break;
    case 3:     // Hard
        m_row = 16;
        m_column = 30;
        m_mines = 99;
        break;
    case 4:     // Custom
        m_row = m_column = 10;
        m_mines = 20;
        break;
    }

    m_field = new Cell*[m_row];
    for(int i = 0; i < m_row; ++i) {
        m_field[i] = new Cell[m_column]; 
    }

    int cellsOnWidth = width() / m_column;
    int cellsOnHeight = height() / m_row;

    squareSize = qMin(cellsOnWidth, cellsOnHeight);

    newGame();
}

void TheField::gameWon()
{
    m_timer->stop();
//    emit resetTime();
    isReadOnly = true;

    QMessageBox::information(this, tr("You won!"), tr("You won!"));
}

void TheField::gameOver()
{
    m_timer->stop();
//    emit resetTime();

    for(int i = 0; i < m_row; ++i) {
        for(int j = 0; j < m_column; ++j) {
            if(m_field[i][j].isMine) {
                m_field[i][j].isHidden = false;
            }
        }
    }

    isReadOnly = true;
}

void TheField::cellFromPos(int x, int y, int *toX, int *toY)
{
    *toX = (x - offset) / squareSize;
    *toY = y / squareSize;
}

// show near cells
void TheField::discoverNear(int x, int y)
{
    for(int i = y - 1; i <= (y+1); ++i) {
        for(int j = x - 1; j <= (x+1); ++j) {
            
            if(isValid(j, i) && m_field[i][j].isHidden) {
                m_field[i][j].isHidden = false;
                --restOfCells;

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
}

void TheField::mouseReleaseEvent(QMouseEvent *ev)
{
    int x, y;

    if(isReadOnly) {
        return;
    }
    
    cellFromPos(ev->x(), ev->y(), &x, &y);

    if(x == lastX && y == lastY && isValid(x, y)) {
        if(!m_field[y][x].isHidden) {
            return;
        }

        if(!m_timer->isActive()) {
            m_timer->start();
        }

        if(ev->button() == Qt::LeftButton) {

            if(m_field[y][x].isFlag) {
                return;
            }
            
            if(m_field[y][x].isMine) {
                gameOver();
            } else {
                m_field[y][x].isHidden = false;
                --restOfCells;

                if(!m_field[y][x].mines) {
                    discoverNear(x, y);
                }
            }

        } else if(ev->button() == Qt::RightButton) {
            if(numberOfFlags == m_mines && !m_field[y][x].isFlag) {
                return;
            }

            m_field[y][x].isFlag = !m_field[y][x].isFlag;
            if(m_field[y][x].isFlag) {
                ++numberOfFlags;
            } else {
                --numberOfFlags;
            }
            
            emit flagSet(numberOfFlags, m_mines);
        }
        repaint();

        if(restOfCells == 0) {
            gameWon();
        }
    }
}

void TheField::resizeEvent(QResizeEvent *ev)
{
    int cellsOnWidth = ev->size().width() / m_column;
    int cellsOnHeight = ev->size().height() / m_row;

    squareSize = qMin(cellsOnWidth, cellsOnHeight);
}

void TheField::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev)

    QPainter p(this);
    int i, j;

    offset = (width() - m_column * squareSize) / 2;

    for(i = 0; i < m_row; ++i) {
        for(j = 0; j < m_column; ++j) {

            if(m_field[i][j].isHidden) {
                p.setBrush(Qt::gray);
                if(m_field[i][j].isFlag) {
                    p.setBrush(Qt::blue);
                }
            } else if(m_field[i][j].isMine) {
                p.setBrush(Qt::red);
            }

            p.drawRect(j*squareSize + offset, i*squareSize, squareSize, squareSize);
            if(!m_field[i][j].isHidden && m_field[i][j].mines) {
                p.drawText(j*squareSize + offset, i*squareSize+squareSize, QString::number(m_field[i][j].mines));
            }
            p.setBrush(Qt::white);

        }
    }
}

