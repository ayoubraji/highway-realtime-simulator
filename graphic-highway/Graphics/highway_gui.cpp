#include "highway_gui.h"
#include "button.h"
#include "threads_controller.h"
#include <QGraphicsTextItem>

bool car_entered = false;

void HighwayGui::standardStart(){
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
		else if(i<23)
		{
			image = QImage(":/images/img/motorcycle.png");
		}
		else if(i == 25)
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

void HighwayGui::displayCustomForm()
{
	dialog = new customChoiceDialog();
	dialog->show();

	connect( dialog, SIGNAL( accepted() ), SLOT( customStart() ) );
}

void HighwayGui::customStart()
{
	//Prendo i valori dalla dialog

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
	//if(vehicle_id == 40)
	if(vehicle_id == 20)
		car_entered = true;
}

void HighwayGui::moveVehicle(int vehicle_id, int lane, int x_pos, int y_pos)
{
	int x, y;

	if(x_pos == 3)//The vehicle have overpassed the limit of the lane
	{
		//fermare tutto e mostrare schermata failed
	}

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
		vehicles[vehicle_id]->setPos(x, y);


	}

	if(car_entered)
	{
		//Adjust the scrollbar if the followed vehicle is near to pass the view zone
		int diff = this->verticalScrollBar()->value() - vehicles[20]->pos().y();

			if(diff > 200)
			{
				printf("diff: %d\n",diff);
				this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - 1300);
			}
	}

}

void HighwayGui::displayMainMenu(){

	int txPos, tyPos, bxPos, byPos, qxPos, qyPos;
	initScene();

	//initGuiThread();

	// create the title text
	QGraphicsTextItem* titleText = new QGraphicsTextItem(QString("Autonomous vehicles highway"));
	QFont titleFont("comic sans",50);
	titleText->setFont(titleFont);
	txPos = this->width()/2 - titleText->boundingRect().width()/2;
	tyPos = 25;
	titleText->setPos(txPos,tyPos);
	scene->addItem(titleText);


	// create the standard start button
	Button* standardStartButton = new Button(QString("Start with standard options"));
	bxPos = this->width()/2 - standardStartButton->boundingRect().width();
	byPos = 500;
	standardStartButton->setPos(bxPos,byPos);
	connect(standardStartButton,SIGNAL(clicked()),this,SLOT(standardStart()));
	scene->addItem(standardStartButton);

	// create the custom start button
	Button* customStartButton = new Button(QString("Set custom choices"));
	bxPos = this->width()/2 + 20;
	byPos = 500;
	customStartButton->setPos(bxPos,byPos);
	connect(customStartButton,SIGNAL(clicked()),this,SLOT(displayCustomForm()));
	scene->addItem(customStartButton);

	// create the quit button
	Button* quitButton = new Button(QString("Quit"));
	qxPos = this->width()/3 + 125 ;
	qyPos = 600;
	quitButton->setPos(qxPos,qyPos);
	connect(quitButton,SIGNAL(clicked()),this,SLOT(close()));
	scene->addItem(quitButton);

}

/*void HighwayGui::displayMainMenu_old(){

	int txPos, tyPos, bxPos, byPos, qxPos, qyPos;
	initScene();

	//initGuiThread();

	// create the title text
	QGraphicsTextItem* titleText = new QGraphicsTextItem(QString("Autonomous vehicles highway"));
	QFont titleFont("comic sans",50);
	titleText->setFont(titleFont);
	txPos = this->width()/2 - titleText->boundingRect().width()/2;
	tyPos = 25;
	titleText->setPos(txPos,tyPos);
	scene->addItem(titleText);

	// create the vehicle choice form
	QGraphicsTextItem* vehicle_choice = new QGraphicsTextItem(QString("Vehicles choise:"));
	QFont subtitleFont("comic sans",25);
	vehicle_choice->setFont(subtitleFont);
	txPos = 90;
	tyPos = 150;
	vehicle_choice->setPos(txPos,tyPos);
	scene->addItem(vehicle_choice);

	standard = new QRadioButton("Standard (50 vehicles, %trucks %cars %motorcycles)", this);
	standard->setGeometry(100,100,200,200);
	custom = new QRadioButton("Custom:", this);

	/*vehicles_number = QLineEdit();
	// vehicles_number.setValidator(QIntValidator())
	vehicles_number.setMaxLength(3);
	vehicles_number.setAlignment(Qt.AlignRight);
	vehicles_number.setFont(QFont("Arial",20));*/

	// create the play button
	/*Button* startButton = new Button(QString("Start"));
	bxPos = this->width()/2 - startButton->boundingRect().width();
	byPos = 900;
	startButton->setPos(bxPos,byPos);
	connect(startButton,SIGNAL(clicked()),this,SLOT(start()));
	scene->addItem(startButton);

	// create the quit button
	Button* quitButton = new Button(QString("Quit"));
	qxPos = this->width()/2 + 20;
	qyPos = 900;
	quitButton->setPos(qxPos,qyPos);
	connect(quitButton,SIGNAL(clicked()),this,SLOT(close()));
	scene->addItem(quitButton);

}
*/

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

