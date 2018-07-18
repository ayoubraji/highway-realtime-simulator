/*
	Ayoub Raji
	Project of Real Time Embedded Systems course
*/

#ifndef CUSTOMCHOICEDIALOG_H
#define CUSTOMCHOICEDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QRadioButton>
#include <QTextEdit>
#include <highway_parameters.h>

//Dialog used by the user in order to create a custom choice

namespace Ui {
class customChoiceDialog;
}

class customChoiceDialog : public QDialog
{
	Q_OBJECT
public:
	explicit customChoiceDialog(QWidget *parent = 0);
	~customChoiceDialog();

	struct highway_parameters_t parameters;

private slots:
	void on_customChoiceDialog_accepted();

	void on_buttonBox_accepted();

	void setValues();
private:
	Ui::customChoiceDialog *ui;
};

#endif // CUSTOMCHOICEDIALOG_H
