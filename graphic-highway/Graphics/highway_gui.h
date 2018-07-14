#ifndef HIGHWAY_GUI_H
#define HIGHWAY_GUI_H

#include "highway.h"
#include "myscene.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QVector>

class HighwayGui: public QGraphicsView{
	Q_OBJECT
public:

	static HighwayGui& getInstance() {

		static HighwayGui highway_gui;

		return highway_gui;
	}
	// public methods
	void displayMainMenu();
	void initScene();

	//QVector<QPixmap*> vehicles;
	QVector<QGraphicsPixmapItem*> vehicles;
	//QVector<QGraphicsRectItem*> vehicles;

	QScrollArea* scrollAreaCandidate ;

	// public attributes
	QGraphicsScene* scene;
	//MyScene* scene;
	QScrollBar *scroll;

	bool started;
	//static bool first = false;
private:
	void vehicleStart(int vehicle_id);

public slots:
	void start();
	void moveVehicle(int vehicle_id, int lane, int x_pos, int y_pos);
private:
		HighwayGui() {}
		HighwayGui(HighwayGui const&);
		void operator=(HighwayGui const&);

private:
	// private methods
	void drawGUI();
	void drawPanel(int x, int y, int width, int height, QColor color, Qt::BrushStyle style, double opacity);
	void drawLine(int x, int y, int width, Qt::GlobalColor color, Qt::PenStyle style, double opacity);

};

#endif // HIGHWAY_GUI_H
