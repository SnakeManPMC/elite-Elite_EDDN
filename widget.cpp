#include "widget.h"
#include "ui_widget.h"

#include <QtWidgets>
#include <QJsonDocument>
#include <QJsonObject>

Widget::Widget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Widget)
{
	ui->setupUi(this);

	setWindowTitle("Elite EDDN by PMC");
}

Widget::~Widget()
{
	delete ui;
}


void Widget::OpenFile()
{
	QStringList nameFilter("*.txt");
	QDir directory(pathToArchives);
	QStringList txtFilesAndDirectories = directory.entryList(nameFilter, QDir::NoFilter, QDir::NoSort);

	int readlines = 0;

	QProgressDialog progressDialog(this);
	progressDialog.setRange(0, txtFilesAndDirectories.size());
	progressDialog.setWindowTitle(tr("Processing TXT files"));

	for (int i = 0; i < txtFilesAndDirectories.size(); i++)
	{
		QFile file(pathToArchives + "\\" + txtFilesAndDirectories[i]);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QMessageBox::information(this, tr("Unable to open file"),
			file.errorString());
			return;
		}
		else
		{
			while (!file.atEnd())
			{
				QByteArray line = file.readLine();
				readlines++;
				parseJSON(line);
			}

			file.close();

			ui->textEdit->append("txtFilesAndDirectories[" + QString::number(i) + "]: " + pathToArchives + "\\" + txtFilesAndDirectories[i]);
			ui->textEdit->append("readlines: " + QString::number(readlines));
			readlines = 0;
		}
		progressDialog.setValue(i);
		progressDialog.setLabelText(tr("File %1 / %2").arg(i).arg(txtFilesAndDirectories.size()));
		qApp->processEvents();
	}
}


void Widget::parseJSON(QByteArray line)
{
	QJsonDocument d = QJsonDocument::fromJson(line);
	//qDebug() << d.isNull();

	QJsonObject sett2 = d.object();
	QJsonValue value = sett2.value(QString("header"));
	//qDebug() << value;

	QJsonObject item = value.toObject();
	//qWarning() << item;

	QString gatewayTimestamp = item.value(QString("gatewayTimestamp")).toString();
	QString softwareName = item.value(QString("softwareName")).toString();
	QString softwareVersion = item.value(QString("softwareVersion")).toString();
	QString uploaderID = item.value(QString("uploaderID")).toString();

	if (!SoftwareNames.contains(softwareName))
		SoftwareNames.push_back(softwareName);

	if (!UploaderIDs.contains(uploaderID))
		UploaderIDs.push_back(uploaderID);
/*
	//qDebug() << str;
	ui->textEdit->append(gatewayTimestamp);
	ui->textEdit->append(softwareName);
	ui->textEdit->append(softwareVersion);
	ui->textEdit->append(uploaderID);
*/
	value = sett2.value(QString("message"));
	item = value.toObject();

	QString timestamp = item.value(QString("timestamp")).toString();
	QString systemName = item.value(QString("systemName")).toString();
	QString stationName = item.value(QString("stationName")).toString();
	QString itemName = item.value(QString("itemName")).toString();
//	int buyPrice = item.value(QString("buyPrice")).toInt();
//	long demand = item.value(QString("demand")).toDouble();
//	int sellPrice = item.value(QString("sellPrice")).toInt();
//	long stationStock = item.value(QString("stationStock")).toDouble();

	if (!Commodities.contains(itemName))
		Commodities.push_back(itemName);

	if (!Systems.contains(systemName))
		Systems.push_back(systemName);

	if (!Stations.contains(stationName))
		Stations.push_back(stationName);

/*
	ui->textEdit->append(timestamp);
	ui->textEdit->append(systemName);
	ui->textEdit->append(stationName);
	ui->textEdit->append(itemName);
	ui->textEdit->append(QString::number(buyPrice));
	ui->textEdit->append(QString::number(demand));
	ui->textEdit->append(QString::number(sellPrice));
	ui->textEdit->append(QString::number(stationStock));
*/
}


// browse EDDN archive directory
void Widget::on_browseArchiveDir_clicked()
{
	QString fileName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
							     "",
							     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (fileName.isEmpty())
	return;
	else
	{
		pathToArchives = fileName;
		ui->lineEdit->setText(pathToArchives);

		// process the files, BIG LOAD on many files ;)
		OpenFile();

		// when done, run statistics
		eddnStatistics();
	}
}


// print statistics into textEdit box
void Widget::eddnStatistics()
{
	Systems.sort();
	Stations.sort();
	Commodities.sort();

	ui->textEdit->append("\n\nStatistics:\nSystems: " + QString::number(Systems.size()));
	//for (int xx = 0; xx < Systems.size(); xx++) ui->textEdit->append(Systems[xx]);
	ui->textEdit->append("Stations: " + QString::number(Stations.size()));
	//for (int xc = 0; xc < Stations.size(); xc++) ui->textEdit->append(Stations[xc]);
	ui->textEdit->append("Commodities: " + QString::number(Commodities.size()));
	//for (int cx = 0; cx < Commodities.size(); cx++) ui->textEdit->append(Commodities[cx]);
	ui->textEdit->append("Uploader Names: " + QString::number(UploaderIDs.size()));
	ui->textEdit->append("SoftwareNames: " + QString::number(SoftwareNames.size()));
	for (int sx = 0; sx < SoftwareNames.size(); sx++) ui->textEdit->append(SoftwareNames[sx]);
}
