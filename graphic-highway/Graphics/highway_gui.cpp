/*
	Ayoub Raji
	Project of Real Time Embedded Systems course
*/

#include "highway_gui.h"
#include "button.h"
#include "threads_controller.h"
#include <QGraphicsTextItem>
#include "highway_parameters.h"
#include <QApplication>
#include <QScrollBar>

//#define ROAD_LENGHT 5000
#define ROAD_LENGHT 2500

int to_be_tracked = -1;

customChoiceDialog *dialog;

void HighwayGui::standardStart()
{
	bool isAmotorcycle;

	// clear the screen
	scene->clear();
	scene->setSceneRect(0,0,1200,SCENE_LENGHT);

	started = true;
	//creating the vehicles in the scene
	for(int i=0; i<50; i++ )
	{
		isAmotorcycle = false;
		QGraphicsPixmapItem *pixmap = new QGraphicsPixmapItem();
		QImage image;

		if(i<=15)
		{
			image = QImage(":/images/img/truck.png");
		}
		else if(i<=25)
		{
			image = QImage(":/images/img/motorcycle.png");
			isAmotorcycle = true;
		}
		else
		{
			image = QImage(":/images/img/car_blue.png");
		}

		QPainter p(&image);
		Qt::GlobalColor color;
		int font;

		if(i == 23)
		{
			color = Qt::yellow;
		}
		else
		{
			color = Qt::black;
		}

		if(isAmotorcycle)
		{
			font = 8;
		}
		else
		{
			font = 12;
		}

		p.setFont(QFont("Times", font, QFont::Bold));
		p.setPen(QPen(color));
		p.drawText(image.rect(), Qt::AlignCenter,QString::number(i));
		pixmap->setPixmap(QPixmap::fromImage(image));

		vehicles.push_back(pixmap);
	}

	//init the threads
	initStandardThreads();

	drawGUI();
}

void HighwayGui::customStart()
{
	int trucks, motorcycles;
	bool isAmotorcycle;

	started = true;

	//Taking values from the dialog
	trucks = dialog->parameters.vehicles_number * dialog->parameters.trucks_perc / 100;
	motorcycles = dialog->parameters.vehicles_number * dialog->parameters.motorcycles_perc / 100;

	for(int i=0; i<dialog->parameters.vehicles_number; i++ )
	{
		QGraphicsPixmapItem *pixmap = new QGraphicsPixmapItem();

		QImage image;

		isAmotorcycle = false;

		if(i<= trucks && trucks != 0)
		{
			image = QImage(":/images/img/truck.png");
		}
		else if(i<=motorcycles+trucks && motorcycles != 0)
		{
			image = QImage(":/images/img/motorcycle.png");
			isAmotorcycle = true;
		}
		else
		{
			image = QImage(":/images/img/car_blue.png");
		}

		QPainter p(&image);
		Qt::GlobalColor color;
		int font;

		if(i == dialog->parameters.vehicle_to_track)
		{
			color = Qt::yellow;
		}
		else
		{
			color = Qt::black;
		}

		if(isAmotorcycle)
		{
			font = 8;
			color = Qt::white;
		}
		else
		{
			font = 12;
		}

		p.setFont(QFont("Times", font, QFont::Bold));
		p.setPen(QPen(color));
		p.drawText(image.rect(), Qt::AlignCenter,QString::number(i));
		pixmap->setPixmap(QPixmap::fromImage(image));

		vehicles.push_back(pixmap);
	}

	//initThreads with parameters
	initCustomThreads(dialog->parameters);

	//reset the scene
	scene = new QGraphicsScene();
	scene->setSceneRect(0,0,1200,SCENE_LENGHT);
	setScene(scene);

	drawGUI();
}

void HighwayGui::displayCustomForm()
{
	dialog = new customChoiceDialog(this);
	connect( dialog, SIGNAL(accepted()),SLOT(customStart() ) );
	dialog->show();
}

void HighwayGui::initScene()
{
	// set up the screen
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setFixedSize(1200,1000);

	// set up the scene
	scene = new QGraphicsScene();

	scene->setSceneRect(0,0,1200,1000);

	setScene(scene);

	started = false;

}

void HighwayGui::vehicleStart(int vehicle_id)
{
	vehicles[vehicle_id]->setPos(scene->width()-350, scene->height());

	scene->addItem(vehicles[vehicle_id]);
}

void HighwayGui::moveVehicle(int vehicle_id, int lane, int x_pos, int y_pos, bool track)
{
	int x, y;

	if(x_pos == 3)//The vehicle have overpassed the limit of the lane
	{
		//It never happens but... in the case it should be better to display a pop-up
	}

	//The vehicle have completed the travel
	if(y_pos == ROAD_LENGHT)
	{
		//The vehicle should be removed from the scene
		scene->removeItem(vehicles[vehicle_id]);
	}
	else
	{
		if(y_pos == 0)//The vehicle is just started, so it should be displayed for the first time
		{
			vehicleStart(vehicle_id);
			if(track)
			{
				to_be_tracked = vehicle_id;
			}
		}
		else
		{
			//setting the movement on the scene
			switch(lane)
			{
			case 0:
				x = scene->width()-350;
				break;
			case 1:
				x = scene->width()-550;
				break;
			case 2:
				x = scene->width()-750;
				break;
			case 3:
				x = scene->width()-950;
				break;
			case 4:
				x = scene->width()-1050;
				break;
			}

			switch(x_pos)
			{
			case 1:
				x = x;
				break;
			case 0:
				x = x - 33;
				break;
			case 2:
				x = x + 33;
				break;
			case 3:
				x = x - 66;
				break;
			case 4:
				x = x + 66;
				break;
			}

			y = scene->height()-(y_pos*30);

			//setting the new position of the vehicle
			vehicles[vehicle_id]->setPos(x, y);

		}

		//for the vehicle that should be tracked
		if(to_be_tracked != -1)
		{
			//Adjust the scrollbar if the followed vehicle is near to pass the view zone
			int diff = this->verticalScrollBar()->value() - vehicles[to_be_tracked]->pos().y();

				if(diff > 200)
				{
					this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - 1300);
				}
		}
	}
}

void HighwayGui::displayMainMenu(){

	int txPos, tyPos, bxPos, byPos, qxPos, qyPos;
	initScene();

	// create the title text
	QGraphicsTextItem* titleText = new QGraphicsTextItem(QString("Autonomous vehicles highway"));
	QFont titleFont("comic sans",50);
	titleText->setFont(titleFont);
	txPos = this->width()/2 - titleText->boundingRect().width()/2;
	tyPos = 25;
	titleText->setPos(txPos,tyPos);
	scene->addItem(titleText);

	// create the description
	QGraphicsTextItem* description =
			new QGraphicsTextItem(
				QString("The user can start a simulation of a highway's portion chosing the vehicles "
						"to put on the road:\n\n"
						"Truck [max speed: 60 km/h, can_overtake: no]\n"
						"Motorcycle [max speed: 80 km/h, can_overtake: yes]\n"
						"Car [max speed: 130 km/h, can_overtake: yes]\n\n"
						"Standard options: \n"
						"50 vehicles [15 Trucks, 25 motorcycles, 10 cars]\n"
						"Vehicle tracked: 23 - "
						"Horizontal moviment simulation frequency: rare\n\n"
						"Custom choice:\n"
						"The user can chose the presence percentage of each vehicle type\n"
						"(The order is alway truck, then motorcycles and then cars, to involve more overtaking)"));

	QFont font("comic sans",18);
	description->setFont(font);
	txPos = 30;
	tyPos = 150;
	description->setPos(txPos,tyPos);
	scene->addItem(description);

	// create the standard start button
	Button* standardStartButton = new Button(QString("Start with standard options"));
	bxPos = this->width()/2 - standardStartButton->boundingRect().width();
	byPos = 700;
	standardStartButton->setPos(bxPos,byPos);
	connect(standardStartButton,SIGNAL(clicked()),this,SLOT(standardStart()));
	scene->addItem(standardStartButton);

	// create the custom start button
	Button* customStartButton = new Button(QString("Set custom choices"));
	bxPos = this->width()/2 + 20;
	byPos = 700;
	customStartButton->setPos(bxPos,byPos);
	connect(customStartButton,SIGNAL(clicked()),this,SLOT(displayCustomForm()));
	scene->addItem(customStartButton);

	// create the quit button
	Button* quitButton = new Button(QString("Quit"));
	qxPos = this->width()/3 ;
	qyPos = 820;
	quitButton->setPos(qxPos,qyPos);
	connect(quitButton,SIGNAL(clicked()),this,SLOT(close()));
	scene->addItem(quitButton);

}

void HighwayGui::drawPanel(int x, int y, int width, int height, QColor color, Qt::BrushStyle style, double opacity){
	// draws a panel at the specified location with the specified properties
	QGraphicsRectItem* panel = new QGraphicsRectItem(x,y,width,height);
	QBrush brush;

	brush.setStyle(style);
	brush.setColor(color);
	panel->setBrush(brush);
	panel->setOpacity(opacity);
	scene->addItem(panel);
}

void HighwayGui::drawLine(int x, int y, int width, Qt::GlobalColor color, Qt::PenStyle style, double opacity){

	QGraphicsLineItem* line = new QGraphicsLineItem();
	QPen pen;
	pen.setColor(color);
	pen.setStyle(style);
	pen.setWidth(width);
	line->setPen(pen);
	line->setLine(QLineF(x, 0, x, y));
	scene->addItem(line);
}

void HighwayGui::drawGUI(){

	int i, txPos, tyPos;
	QGraphicsTextItem* text;

	// draw the left panel
	drawPanel(0,0,200,scene->height(),Qt::green,Qt::SolidPattern,1);

	// draw the right panel
	drawPanel(scene->width()-200,0,200,scene->height(),Qt::green,Qt::SolidPattern,1);

	// draw the road
	drawPanel(200,0,800,scene->height(),Qt::black,Qt::Dense4Pattern,1);

	// draw the lines
	drawLine(400, scene->height(), 12, Qt::GlobalColor::white, Qt::DashLine, 1);
	drawLine(600, scene->height(), 12, Qt::GlobalColor::white, Qt::DashLine, 1);
	drawLine(800, scene->height(), 12, Qt::GlobalColor::white, Qt::DashLine, 1);

	// draw road sings
	for(i=0; i<= SCENE_LENGHT; i = i + 1000)
	{
		tyPos = (i == SCENE_LENGHT) ? SCENE_LENGHT - i : SCENE_LENGHT - i - 100;

		drawPanel(scene->width()-200,tyPos,200,100,Qt::blue,Qt::SolidPattern,1);

		// create the text of the road sign
		if(i == 0)
		{
			text = new QGraphicsTextItem(QString("BEGINNING!"));
		}
		else if(i == SCENE_LENGHT)
		{
			text = new QGraphicsTextItem(QString("ENDING!"));
		}
		else
		{
			text = new QGraphicsTextItem(QString("KM: %1 of %2").arg(i/1000).arg(SCENE_LENGHT/1000));
		}

		QFont font("comic sans",18);
		text->setFont(font);
		text->setDefaultTextColor(Qt::white);
		txPos = scene->width()-200;
		tyPos = (i == SCENE_LENGHT) ? SCENE_LENGHT - i : SCENE_LENGHT - i - 100;
		text->setPos(txPos,tyPos);
		scene->addItem(text);

	}

	this->verticalScrollBar()->setSliderPosition(scene->height());
}
