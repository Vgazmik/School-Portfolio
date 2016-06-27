#include "widget.h"
#include "ui_widget.h"
#include "sq.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QGraphicsScene *scene = new QGraphicsScene();
    //QGraphicsView *view = new QGraphicsView(scene);
    //QGraphicsItem s = new QGraphicsItem();
    sq spots[8][8];
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            spots[i][j].x = 0 + (j*SQ_S);
            spots[i][j].y = i*SQ_S;
        }
    }
    int x = 0;
    int y = 0;  
    QBrush redBrush(Qt::red);
    QBrush blackBrush(Qt::black);
    QPen blackPen(QColor(255, 255, 255));
    blackPen.setWidth(3);
    for(int m = 0; m < 8; m++) {
        for(int n = 0; n < 8; n++) {

            x = spots[m][n].x;
            y = spots[m][n].y;
            //scene->foregroundBrush();
            //scene->addRect(QRectF(x,y,SQ_S,SQ_S));
            if(m%2 == 0 && n%2 == 0)
                item1 = scene->addRect(x,y,SQ_S,SQ_S, blackPen, redBrush);
            else if(m%2 == 0)
                item1 = scene->addRect(x,y,SQ_S,SQ_S, blackPen, blackBrush);
            if(m%2 != 0 && n %2 != 0)
                item1 = scene->addRect(x,y,SQ_S,SQ_S, blackPen, redBrush);
            else if(m%2 != 0)
                item1 = scene->addRect(x,y,SQ_S,SQ_S, blackPen, blackBrush);
        }
    }

    ui->gV->setScene(scene);
    //ui->gV->show();
}

Widget::~Widget()
{
    delete ui;
}
