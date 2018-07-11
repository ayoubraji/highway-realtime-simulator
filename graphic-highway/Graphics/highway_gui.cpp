#include "highway_gui.h"
#include "button.h"
#include "threads_controller.h"
#include <QGraphicsTextItem>

/*HighwayGui::HighwayGui(QWidget *parent){
	// set up the screen
	//setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//setFixedSize(1024,768);
	setFixedSize(1200,1080);

	// set up the scene
	scene = new QGraphicsScene();
	//scene->setSceneRect(0,0,1024,768);
	scene->setSceneRect(0,0,1200,1080);
	//scene->setSceneRect(0,0,1200,50000);
	setScene(scene);

	started = false;
}*/

//HighwayGui::HighwayGui() { }

//HighwayGui::~HighwayGui() { }


void HighwayGui::start(){
	// clear the screen
	scene->clear();

	started = true;

	//connect(startButton,SIGNAL(clicked()),this,SLOT(start()));

	for(int i=0; i<50; i++ )
	{
		//QGraphicsRectItem *vehicle = new QGraphicsRectItem();
		vehicles.push_back(new QGraphicsRectItem());
		//vehicles[i] = new QGraphicsRectItem();
	}

	initThreads();
	//initVehiclesThreads();

	drawGUI();

}

void HighwayGui::initScene()
{
	// set up the screen
	//setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//setFixedSize(1024,768);
	setFixedSize(1200,1080);

	// set up the scene
	scene = new QGraphicsScene();
	//scene->setSceneRect(0,0,1024,768);
	scene->setSceneRect(0,0,1200,1080);
	//scene->setSceneRect(0,0,1200,50000);
	setScene(scene);

	centerOn(scene->width(),scene->height());

	started = false;


}

void HighwayGui::vehicleStart(int vehicle_id)
{
	vehicles[vehicle_id]->setRect(0, 0, 100, 100);
	vehicles[vehicle_id]->setPos(scene->width()-350, scene->height());
	scene->addItem(vehicles[vehicle_id]);
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

		y = scene->height()-(y_pos*100);

		vehicles[vehicle_id]->setPos(x, y);

	}

	if(vehicle_id == 30)
	{
		centerOn(vehicles[vehicle_id]);
	}

	//drawPanel(250,0,100,1000,Qt::blue,Qt::SolidPattern,1);
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

	scene->setSceneRect(0,0,1200,500000);

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


	// place player 1 text
	//QGraphicsTextItem* p1 = new QGraphicsTextItem("Player 1's Cards: ");
	//p1->setPos(25,0);
	//scene->addItem(p1);

	// place player 1 text
	/*QGraphicsTextItem* p2 = new QGraphicsTextItem("Player 2's Cards: ");
	p2->setPos(874+25,0);
	scene->addItem(p2);

	// place whosTurnText
	whosTurnText = new QGraphicsTextItem();
	setWhosTurn(QString("PLAYER1"));
	whosTurnText->setPos(490,0);
	scene->addItem(whosTurnText);
*/
}

