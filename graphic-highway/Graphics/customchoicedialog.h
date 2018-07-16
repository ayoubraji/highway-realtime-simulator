#ifndef CUSTOMCHOICEDIALOG_H
#define CUSTOMCHOICEDIALOG_H

#include <QDialog>

namespace Ui {
class customChoiceDialog;
}

class customChoiceDialog : public QDialog
{
	Q_OBJECT

public:
	explicit customChoiceDialog(QWidget *parent = 0);
	~customChoiceDialog();

private:
	Ui::customChoiceDialog *ui;
};

#endif // CUSTOMCHOICEDIALOG_H
