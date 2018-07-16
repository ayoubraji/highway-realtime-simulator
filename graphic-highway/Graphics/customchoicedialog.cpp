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

//add function to add values to variables
