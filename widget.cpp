
// TODO - add software name/version list, add username count (list too long)

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

	OpenFile();
}

Widget::~Widget()
{
	delete ui;
}


void Widget::OpenFile()
{
	QString elite_path = "D://Warez//Elite.Dangerous//eddn.archives.12-31-14";
	QStringList nameFilter("archive-markets-*.txt");
	QDir directory(elite_path);
	QStringList txtFilesAndDirectories = directory.entryList(nameFilter, QDir::NoFilter, QDir::NoSort);

	//QFile file("D://Warez//Elite.Dangerous//eddn.archives.12-31-14//archive-markets-01.txt");

	int readlines = 0;

	for (int i = 0; i < txtFilesAndDirectories.size(); i++)
	{
		QFile file(elite_path + "\\" + txtFilesAndDirectories[i]);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QMessageBox::information(this, tr("Unable to open file"),
			file.errorString());
			return;
		}

		while (!file.atEnd())
		{
			QByteArray line = file.readLine();
			readlines++;
			parseJSON(line);
		}

		file.close();
		ui->textEdit->append("txtFilesAndDirectories[" + QString::number(i) + "]: " + elite_path + "\\" + txtFilesAndDirectories[i]);
		ui->textEdit->append("read and done!");
		ui->textEdit->append("readlines: " + QString::number(readlines));
		readlines = 0;
	}

	Systems.sort();
	Stations.sort();
	Commodities.sort();

	ui->textEdit->append("Systems: " + QString::number(Systems.size()));
	//for (int xx = 0; xx < Systems.size(); xx++) ui->textEdit->append(Systems[xx]);
	ui->textEdit->append("Stations: " + QString::number(Stations.size()));
	//for (int xc = 0; xc < Stations.size(); xc++) ui->textEdit->append(Stations[xc]);
	ui->textEdit->append("Commodities: " + QString::number(Commodities.size()));
	//for (int cx = 0; cx < Commodities.size(); cx++) ui->textEdit->append(Commodities[cx]);
	ui->textEdit->append("Uploader Names: " + QString::number(UploaderIDs.size()));
	ui->textEdit->append("SoftwareNames: " + QString::number(SoftwareNames.size()));
	for (int sx = 0; sx < SoftwareNames.size(); sx++) ui->textEdit->append(SoftwareNames[sx]);
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
/*
	"buyPrice": 0,
	"timestamp": "2014-12-30T00:38:33+00:00",
	"stationStock": 0,
	"systemName": "V1688 Aquilae",
	"stationName": "Rangarajan Port",
	"demand": 2882520,
	"sellPrice": 1305,
	"itemName": "Titanium"
*/
}


/*
{'header':
{'softwareVersion': 'v0.3 test', 'softwareName': 'EliteOCRReader', 'uploaderID': 'abcdef0123456789'},
'$schema': 'http://schemas.elite-markets.net/eddn/commodity/1', 'message':
{'categoryName': u'', 'buyPrice': 102, 'timestamp': '2014-12-16T12:43:00', 'stationStock': 13672, 'systemName': 'SomeSystem', 'stationName': 'Azeban City [SomeSystem]', 'demand': 0, 'sellPrice': 101, 'itemName': 'Hydrogen Fuel'
}}
*/

/*
	QString str = "{"
	" \"Herausgeber\": \"Xema\","
	" \"Nummer\": \"1234-5678-9012-3456\","
	" \"Deckung\": 2e+6,"
	" \"Währung\": \"EURO\","
	" \"Inhaber\": {"
	" \"Name\": \"Mustermann\","
	" \"Vorname\": \"Max\","
	" \"männlich\": true,"
	" \"Hobbys\": [ \"Reiten\", \"Golfen\", \"Lesen\" ],"
	" \"Alter\": 42,"
	" \"Kinder\": [],"
	" \"Partner\": null"
	" }"
	"}";
*/
