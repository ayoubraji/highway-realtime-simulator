#include "highway_gui.h"
#include "button.h"
#include "threads_controller.h"
#include <QGraphicsTextItem>

bool car_entered = false;

void HighwayGui::start(){
	// clear the screen
	scene->clear();
	scene->setSceneRect(0,0,1200,500000);

	started = true;

	for(int i=0; i<50; i++ )
	{
		QGraphicsPixmapItem *pixmap = new QGraphicsPixmapItem();

		QImage image;

		if(i<15)
		{
			image = QImage(":/images/img/truck.png");
		}
		else if(i<25)
		{
			image = QImage(":/images/img/motorcycle.png");
		}
		else if(i == 40)
		{
			image = QImage(":/images/img/car_blue.png");
		}
		else
		{
			image = QImage(":/images/img/car.png");
		}

		QPainter p(&image);
		QPixmap pix = QPixmap();

		p.setPen(QPen(Qt::red));
		p.setFont(QFont("Times", 12, QFont::Bold));
		p.drawText(image.rect(), Qt::AlignCenter,QString::number(i));
		pixmap->setPixmap(QPixmap::fromImage(image));

		vehicles.push_back(pixmap);
	}

	initThreads();
	//initVehiclesThreads();

	drawGUI();

}

void HighwayGui::initScene()
{
	// set up the screen
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setFixedSize(1200,1000);

	// set up the scene
	scene = new QGraphicsScene();

	scene->setSceneRect(0,0,1200,500000);

	setScene(scene);

	started = false;


}

void HighwayGui::vehicleStart(int vehicle_id)
{
	vehicles[vehicle_id]->setPos(scene->width()-350, scene->height());

	scene->addItem(vehicles[vehicle_id]);
	if(vehicle_id == 40)
		car_entered = true;
}

void HighwayGui::moveVehicle(int vehicle_id, int lane, int x_pos, int y_pos)
{
	int x, y;

	if(y_pos == 0)
	{
		vehicleStart(vehicle_id);
	}
	else
	{
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

		y = scene->height()-(y_pos*30);

		vehicles[vehicle_id]->setPos(x, y);


	}

	if(car_entered)
	{
		//Adjust the scrollbar if the followed vehicle is near to pass the view zone
		int diff = this->verticalScrollBar()->value() - vehicles[40]->pos().y();

			if(diff > 200)
			{
				printf("diff: %d\n",diff);
				this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - 1300);
			}
	}

}

void HighwayGui::displayMainMenu(){

	initScene();

	//initGuiThread();

	// create the title text
	QGraphicsTextItem* titleText = new QGraphicsTextItem(QString("Autonomous vehicles highway"));
	QFont titleFont("comic sans",50);
	titleText->setFont(titleFont);
	int txPos = this->width()/2 - titleText->boundingRect().width()/2;
	int tyPos = 150;
	titleText->setPos(txPos,tyPos);
	scene->addItem(titleText);

	// create the play button
	Button* startButton = new Button(QString("Start"));
	int bxPos = this->width()/2 - startButton->boundingRect().width()/2;
	int byPos = 275;
	startButton->setPos(bxPos,byPos);
	connect(startButton,SIGNAL(clicked()),this,SLOT(start()));
	scene->addItem(startButton);

	// create the quit button
	Button* quitButton = new Button(QString("Quit"));
	int qxPos = this->width()/2 - quitButton->boundingRect().width()/2;
	int qyPos = 350;
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

	this->verticalScrollBar()->setSliderPosition(scene->height());
}

