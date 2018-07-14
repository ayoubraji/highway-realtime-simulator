#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>
#include <QScrollBar>
class MyScene : public QGraphicsScene{
public:
	explicit MyScene(QScrollBar* s, QObject *parent);
	QScrollBar *scroll;
};

#endif // MYSCENE_H
