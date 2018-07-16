#ifndef HIGHWAY_GUI_H
#define HIGHWAY_GUI_H

#include "highway.h"
#include "myscene.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QVector>
#include <QRadioButton>
#include <QLineEdit>
#include <QGraphicsLineItem>
#include <Graphics/customchoicedialog.h>

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

	QScrollArea* scrollAreaCandidate ;

	customChoiceDialog *dialog;

	//Main Menu elements
	//vehicles choice
	QRadioButton *standard;
	QRadioButton *custom;

	//custom choices
	QLineEdit vehicles_number, trucks_perc, cars_perc, motorcycle_perc;



	// public attributes
	QGraphicsScene* scene;
	QScrollBar *scroll;

	bool started;
private:
	void vehicleStart(int vehicle_id);
	void customStart();
public slots:
	void standardStart();
	void displayCustomForm();
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
