/*
	Ayoub Raji
	Project of Real Time Embedded Systems course
*/

#ifndef HIGHWAY_GUI_H
#define HIGHWAY_GUI_H

#include "highway.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QVector>
#include <QGraphicsLineItem>
#include <Graphics/customchoicedialog.h>

//#define SCENE_LENGHT 150000
#define SCENE_LENGHT 75000

class HighwayGui: public QGraphicsView{
	Q_OBJECT
public:
	//SINGLETON
	static HighwayGui& getInstance() {

		static HighwayGui highway_gui;

		return highway_gui;
	}

	// public methods
	void displayMainMenu();
	void initScene();

	QVector<QGraphicsPixmapItem*> vehicles;

	QScrollArea* scrollAreaCandidate;

	// public attributes
	QGraphicsScene* scene;
	QScrollBar *scroll;

	bool started;
	int arrived, n_vehicles;

private:
	void vehicleStart(int vehicle_id);

public slots:
	void standardStart();
	void customStart();
	void displayCustomForm();
	void moveVehicle(int vehicle_id, int lane, int x_pos, int y_pos, bool to_be_tracked);

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
