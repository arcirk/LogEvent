#include "infobases.h"

Infobases::Infobases(QObject *parent)
    : QObject{parent}
{
    _SizeEventLog = 0;
    _isOpen = false;
}

QString Infobases::Name()
{
    return _Name;
}

void Infobases::setName(const QString &name)
{
    _Name = name;
}

QString Infobases::GUID()
{
    return _GUID;
}

void Infobases::setGUID(const QString &guid)
{
    _GUID = guid;
}

QString Infobases::Description()
{
    return _Description;
}

void Infobases::setDescription(const QString &desc)
{
    _Description = desc;
}

double Infobases::SizeEventLog()
{
    return _SizeEventLog;
}

void Infobases::setSizeEventLog(double size)
{
    _SizeEventLog = size;
}

QString Infobases::PathLogEvent()
{
    return _PathLogEvent;
}

void Infobases::setPathLogEvent(const QString &pth)
{
    _PathLogEvent = pth;
}

bool Infobases::isOpen() {
    return _isOpen;
}

void Infobases::setIsOpen(bool val) {
    _isOpen = val;
}
