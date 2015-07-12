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

private slots:
	void on_browseArchiveDir_clicked();

private:
	Ui::Widget *ui;

	void OpenFile();
	void parseJSON(QByteArray line);
	QStringList Systems;
	QStringList Stations;
	QStringList Commodities;
	QStringList SoftwareNames;
	QStringList UploaderIDs;
	QString pathToArchives;
	void eddnStatistics();
};

#endif // WIDGET_H
