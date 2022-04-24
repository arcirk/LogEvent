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
    int SizeEventLog();
    void setSizeEventLog(int size);
    QString PathLogEvent();
    void setPathLogEvent(const QString& pth);
private:
    QString _Name;
    QString _GUID;
    QString _Description;
    int _SizeEventLog;
    QString _PathLogEvent;

signals:

};

#endif // INFOBASES_H
