#include <QFile>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include "jigsaw/core/NotifyCenter.h"
#include "jigsaw/core/Translator.h"


QHash<QString, QString> jigsaw::core::Translator::kvmap;


void jigsaw::core::Translator::LoadSystemLanguage()
{
	//QVariant val = locale.query(QSystemLocale::CountryId, val);
    LoadLanguage("zh_CN");
}

void jigsaw::core::Translator::LoadLanguage(const QString& _language)
{
    QString path = QString(":/languages/%1").arg(_language);
    QFile file(path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        NotifyCenter::Fatal(QString("Open %1.json failed!").arg(_language));

	QByteArray bytes = file.readAll();
	file.close();

	QJsonParseError json_error;
	QJsonDocument jsonDoc = QJsonDocument::fromJson(bytes, &json_error);
	if (json_error.error != QJsonParseError::NoError)
        NotifyCenter::Fatal(QString("parse %1 has error: %2").arg(_language).arg(json_error.errorString()));

	QJsonObject objJson = jsonDoc.object();
	for (auto itr = objJson.begin(); itr != objJson.end(); ++itr)
	{
		kvmap[itr.key()] = itr.value().toString();
	}
	
}

QString jigsaw::core::Translator::Translate(const QString& _text)
{
	if (kvmap.contains(_text))
		return kvmap.value(_text);
	return _text;
}
