#include "customchoicedialog.h"
#include "ui_customchoicedialog.h"

customChoiceDialog::customChoiceDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::customChoiceDialog)
{
	//connect( this, SIGNAL( accepted() ), SLOT(setValues() ) );
	ui->setupUi(this);
}

customChoiceDialog::~customChoiceDialog()
{
	delete ui;
}

// add final form values to variables
void customChoiceDialog::setValues()
{
	cars_perc = ui->cars_perc->value();
	trucks_perc = ui->trucks_perc->value();
	motorcycles_perc = ui->motorcycles_perc->value();

	vehicles_number = ui->vehicles_number->value();
	vehicle_to_track = ui->vehicle_to_track->toPlainText().toInt();

	rare_frequency = ui->rare_frequency->isChecked();
	frequent_frequency = ui->frequent_frequency->isChecked();
}

/*void customChoiceDialog::setValues()
{
	parameters.cars_perc = ui->cars_perc->value();
	parameters.trucks_perc = ui->trucks_perc->value();
	parameters.motorcycles_perc = ui->motorcycles_perc->value();

	parameters.vehicles_number = ui->vehicles_number->value();
	parameters.vehicle_to_track = ui->vehicle_to_track->toPlainText().toInt();

	parameters.rare_frequency = ui->rare_frequency->isChecked();
	parameters.frequent_frequency = ui->frequent_frequency->isChecked();
}*/

void customChoiceDialog::on_customChoiceDialog_accepted()
{
	setValues();
}

void customChoiceDialog::on_buttonBox_accepted()
{
	//setValues();
}

