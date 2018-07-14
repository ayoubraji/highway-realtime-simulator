#ifndef VEHICLE_ITEM_H
#define VEHICLE_ITEM_H

#include <QGraphicsRectItem>
#include <QGraphicsView>
class VehicleItem : QGraphicsRectItem{

public:
	VehicleItem(int x, int y);

private:
	QPen pen;
};

#endif // VEHICLE_ITEM_H
