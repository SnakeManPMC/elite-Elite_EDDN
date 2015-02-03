#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
	Q_OBJECT

public:
	explicit Widget(QWidget *parent = 0);
	~Widget();

private:
	Ui::Widget *ui;

	void OpenFile();
	void parseJSON(QByteArray line);
	QStringList Systems;
	QStringList Stations;
	QStringList Commodities;
	QStringList SoftwareNames;
	QStringList UploaderIDs;
};

#endif // WIDGET_H
