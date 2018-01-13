#pragma once

#include <QHash>
#include <QColor>

namespace jigsaw
{
namespace core
{

class Color
{
public:
    Color(QString _name, QString _hex, QString _status);
    Color(QString _name, QString _hex);
    QString Hex();
    QString FullName();

private:
    QString name_;
    QString hex_;
    QString status_;
};

class StatusColor
{
public:
    StatusColor(QString _name,
                QString _hexNormal,
                QString _hexActive,
                QString _hexDisabled);
    QString Name();
    Color Normal();
    Color Active();
    Color Disabled();
private:
    QString name_;
    QString normal_;
    QString active_;
    QString disabled_;
};

class ColorPalette
{
public:
    static const QString PRIMARY;
    static const QString INFO;
    static const QString SUCCESS;
    static const QString WARNING;
    static const QString DANGER;

    static void LoadColorPalette();
    static void Merge(const QString& _file);

    static StatusColor Primary();
    static StatusColor Info();
    static StatusColor Success();
    static StatusColor Warning();
    static StatusColor Danger();

    static Color Black();
    static Color White();
    static Color Gray();
    static Color Red();
    static Color Green();
    static Color Blue();
private:
    static QHash<QString, QString> hexs;

};//class ColorPalette

}//namespace core
}//namespace jigsaw
