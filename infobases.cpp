#include "infobases.h"

Infobases::Infobases(QObject *parent)
    : QObject{parent}
{
    _SizeEventLog = 0;
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

int Infobases::SizeEventLog()
{
    return _SizeEventLog;
}

void Infobases::setSizeEventLog(int size)
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
