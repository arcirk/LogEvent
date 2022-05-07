#ifndef INFOBASES_H
#define INFOBASES_H

#include <QObject>

class Infobases : public QObject
{
    Q_OBJECT
public:
    explicit Infobases(QObject *parent = nullptr);

    QString Name();
    void setName(const QString& name);
    QString GUID();
    void setGUID(const QString& guid);
    QString Description();
    void setDescription(const QString& desc);
    double SizeEventLog();
    void setSizeEventLog(double size);
    QString PathLogEvent();
    void setPathLogEvent(const QString& pth);

    bool isOpen();
    void setIsOpen(bool val);

private:
    QString _Name;
    QString _GUID;
    QString _Description;
    double _SizeEventLog;
    QString _PathLogEvent;
    bool _isOpen;

signals:

};

#endif // INFOBASES_H
