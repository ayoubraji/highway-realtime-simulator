#ifndef CUSTOMCHOICEDIALOG_H
#define CUSTOMCHOICEDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QRadioButton>
#include <QTextEdit>
#include <highway_parameters.h>

namespace Ui {
class customChoiceDialog;
}

class customChoiceDialog : public QDialog
{
	Q_OBJECT
public:
	explicit customChoiceDialog(QWidget *parent = 0);
	~customChoiceDialog();

	int cars_perc, trucks_perc, motorcycles_perc;
	bool rare_frequency, frequent_frequency;
	int vehicles_number, vehicle_to_track;

	struct highway_parameters_t parameters;

private slots:
	void on_customChoiceDialog_accepted();

	void on_buttonBox_accepted();

	void setValues();
private:
	Ui::customChoiceDialog *ui;
};

#endif // CUSTOMCHOICEDIALOG_H
