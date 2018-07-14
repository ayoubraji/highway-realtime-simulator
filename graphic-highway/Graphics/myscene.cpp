#include "myscene.h"

MyScene::MyScene(QScrollBar*s , QObject *parent)
{
	QGraphicsScene(0,0,1200,500000, parent);
	scroll = s;
}

