#include <QFile>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>

#include "jigsaw/core/ColorPalette.h"


QHash<QString, QString> jigsaw::core::ColorPalette::hexs;
const QString jigsaw::core::ColorPalette::PRIMARY = "primary";
const QString jigsaw::core::ColorPalette::INFO = "info";
const QString jigsaw::core::ColorPalette::SUCCESS = "success";
const QString jigsaw::core::ColorPalette::WARNING = "warning";
const QString jigsaw::core::ColorPalette::DANGER = "danger";

void jigsaw::core::ColorPalette::LoadColorPalette()
{
    Merge(":/jigsaw/palette");
}

void jigsaw::core::ColorPalette::Merge(const QString &_file)
{
    QFile file(_file);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QByteArray bytes = file.readAll();
    file.close();

    QJsonParseError json_error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(bytes, &json_error);
    if (json_error.error != QJsonParseError::NoError)
        return;

    QJsonObject objJson = jsonDoc.object();
    for (auto itr = objJson.begin(); itr != objJson.end(); ++itr)
    {
        hexs[itr.key()] = itr.value().toString();
    }
}

jigsaw::core::StatusColor jigsaw::core::ColorPalette::Primary()
{
    QString normal = "#000000";
    if(hexs.contains("primary.normal"))
        normal = hexs.value("primary.normal");
    QString active= "#000000";
    if(hexs.contains("primary.active"))
        active= hexs.value("primary.active");
    QString disabled = "#000000";
    if(hexs.contains("primary.disabled"))
        disabled = hexs.value("primary.disabled");
    return StatusColor("primary", normal, active, disabled);
}

jigsaw::core::StatusColor jigsaw::core::ColorPalette::Info()
{
    QString normal = "#000000";
    if(hexs.contains("info.normal"))
        normal = hexs.value("info.normal");
    QString active= "#000000";
    if(hexs.contains("info.active"))
        active= hexs.value("info.active");
    QString disabled = "#000000";
    if(hexs.contains("info.disabled"))
        disabled = hexs.value("info.disabled");
    return StatusColor("info", normal, active, disabled);
}

jigsaw::core::StatusColor jigsaw::core::ColorPalette::Success()
{
    QString normal = "#000000";
    if(hexs.contains("success.normal"))
        normal = hexs.value("success.normal");
    QString active= "#000000";
    if(hexs.contains("success.active"))
        active= hexs.value("success.active");
    QString disabled = "#000000";
    if(hexs.contains("success.disabled"))
        disabled = hexs.value("success.disabled");
    return StatusColor("success", normal, active, disabled);
}

jigsaw::core::StatusColor jigsaw::core::ColorPalette::Warning()
{
    QString normal = "#000000";
    if(hexs.contains("warning.normal"))
        normal = hexs.value("warning.normal");
    QString active= "#000000";
    if(hexs.contains("warning.active"))
        active= hexs.value("warning.active");
    QString disabled = "#000000";
    if(hexs.contains("warning.disabled"))
        disabled = hexs.value("warning.disabled");
    return StatusColor("warning", normal, active, disabled);
}

jigsaw::core::StatusColor jigsaw::core::ColorPalette::Danger()
{
    QString normal = "#000000";
    if(hexs.contains("danger.normal"))
        normal = hexs.value("danger.normal");
    QString active= "#000000";
    if(hexs.contains("danger.active"))
        active= hexs.value("danger.active");
    QString disabled = "#000000";
    if(hexs.contains("danger.disabled"))
        disabled = hexs.value("danger.disabled");
    return StatusColor("danger", normal, active, disabled);
}


jigsaw::core::Color::Color(QString _name, QString _hex, QString _status)
    : name_(_name)
    , hex_(_hex)
    , status_(_status)
{

}

jigsaw::core::Color::Color(QString _name, QString _hex)
    : name_(_name)
    , hex_(_hex)
    , status_("")
{

}

QString jigsaw::core::Color::Hex()
{
    return hex_;
}

QString jigsaw::core::Color::FullName()
{
    if(status_.isEmpty() || status_.isNull())
        return name_;
    else
        return QString("%1.%2").arg(status_).arg(name_);
}

jigsaw::core::StatusColor::StatusColor(QString _name, QString _hexNormal, QString _hexActive, QString _hexDisabled)
    : name_(_name)
    , normal_(_hexNormal)
    , active_(_hexActive)
    , disabled_(_hexDisabled)
{

}

QString jigsaw::core::StatusColor::Name()
{
   return name_;
}

jigsaw::core::Color jigsaw::core::StatusColor::Normal()
{
    return Color("normal", normal_, name_);
}

jigsaw::core::Color jigsaw::core::StatusColor::Active()
{
    return Color("active", active_, name_);
}

jigsaw::core::Color jigsaw::core::StatusColor::Disabled()
{
    return Color("disabled", disabled_, name_);
}

jigsaw::core::Color jigsaw::core::ColorPalette::Black()
{
    return Color("black", "#000000");

}

jigsaw::core::Color jigsaw::core::ColorPalette::White()
{
    return Color("white", "#ffffff");
}

jigsaw::core::Color jigsaw::core::ColorPalette::Gray()
{
    return Color("gray", "#808080");
}

jigsaw::core::Color jigsaw::core::ColorPalette::Red()
{
    return Color("red", "#ff0000");
}

jigsaw::core::Color jigsaw::core::ColorPalette::Green()
{
    return Color("green", "#00ff00");
}

jigsaw::core::Color jigsaw::core::ColorPalette::Blue()
{
    return Color("blue", "#0000ff");
}
