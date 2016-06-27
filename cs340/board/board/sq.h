#ifndef SQ_H
#define SQ_H
#define SQ_S 40
#include <QGraphicsItem>

typedef struct sq{
    int x;
    int y;
}sq;
/*class sq : public QGraphicsItem{
public:
    sq();
    int x;
    int y;

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

};*/
#endif // SQ_H
