/*
	Ayoub Raji
	Project of Real Time Embedded Systems course
*/

#include "customchoicedialog.h"
#include "ui_customchoicedialog.h"

customChoiceDialog::customChoiceDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::customChoiceDialog)
{
	ui->setupUi(this);
}

customChoiceDialog::~customChoiceDialog()
{
	delete ui;
}

// add final form values in the internal variables
void customChoiceDialog::setValues()
{
	parameters.cars_perc = ui->cars_perc->value();
	parameters.trucks_perc = ui->trucks_perc->value();
	parameters.motorcycles_perc = ui->motorcycles_perc->value();

	parameters.vehicles_number = ui->vehicles_number->value();
	parameters.vehicle_to_track = ui->vehicle_to_track->toPlainText().toInt();

	parameters.rare_frequency = ui->rare_frequency->isChecked();
	parameters.frequent_frequency = ui->frequent_frequency->isChecked();
}

void customChoiceDialog::on_customChoiceDialog_accepted()
{
	setValues();
}

void customChoiceDialog::on_buttonBox_accepted()
{
	//setValues();
}

