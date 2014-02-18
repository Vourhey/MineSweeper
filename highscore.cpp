#include <QTabWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include <highscore.h>

HighScore::HighScore(QWidget *parent)
    : QDialog(parent)
{
    tabWidget = new QTabWidget;
    tabWidget->setTabPosition(QTabWidget::West);

    // read settings

    createPage(tr("Easy"));
    createPage(tr("Medium"));
    createPage(tr("Hard"));
    createPage(tr("Custom"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);
}

void HighScore::createPage(const QString &lbl)
{
    int i;
    QVBoxLayout *vLayout = new QVBoxLayout;

    for(i = 0; i < 10; ++i) {
        vLayout->addWidget(createLine());
    }

    QWidget *page = new QWidget;
    page->setLayout(vLayout);

    tabWidget->addTab(page, lbl);
}

QWidget *HighScore::createLine(const QString &name, const QString &time)
{
/*    if(name.isEmpty()) {
        name = time = QString(" - ");
    } */

    QLabel *rankLbl = new QLabel(tr("#%1").arg(1));
    QLabel *nameLbl = new QLabel(name);
    QLabel *timeLbl = new QLabel(time);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(rankLbl);
    hLayout->addWidget(nameLbl);
    hLayout->addWidget(timeLbl);

    QWidget *w = new QWidget;
    w->setLayout(hLayout);

    return w;
}

