#include "settings.h"
#include <QFile>
#include <QDir>
#include <QRegularExpression>


quint64 dirFileSize(const QString &path)
{
    QDir dir(path);
    quint64 size = 0;
    //dir.entryInfoList(QDir::Files) возвращает информацию о файле
    foreach(QFileInfo fileInfo, dir.entryInfoList(QDir::Files))
    {
     // Рассчитать размер файла
    size += fileInfo.size();
    }
     //dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot) возвращает все подкаталоги и фильтрует их
    foreach(QString subDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
     // Если есть подкаталог, рекурсивно вызвать функцию dirFileSize ()
    size += dirFileSize(path + QDir::separator() + subDir);
    }
    return size;
}

Settings::Settings(QObject *parent)
    : QObject{parent}
{

}

void Settings::setServerLogFolder(const QString &path)
{
    serverLogFolder = path;
}

void Settings::get_server_info()
{
    QDir folder(serverLogFolder);
    if(!folder.exists())
        return;

    QFile lstFile(serverLogFolder + QDir::separator() + "1CV8Clst.lst");
    if(!lstFile.open(QFile::ReadOnly))
        return;

    QString text = QString::fromUtf8(lstFile.readAll());

    lstFile.close();

    QStringList ClusterSetting = ParseEventLogString(text);
    if(ClusterSetting.size() > 2){
        QStringList DatabaseSettings = ParseEventLogString(ClusterSetting[2]);

        for (auto itr : DatabaseSettings) {

            QStringList DatabaseDescription = ParseEventLogString(itr);

            if(DatabaseDescription.size() == 0)
                continue;

            auto inf = new Infobases();
            inf->setName(DatabaseDescription[1]);
            inf->setGUID(DatabaseDescription[0]);
            inf->setDescription(DatabaseDescription[2]);
            inf->setSizeEventLog(0);

            QDir ibDir(serverLogFolder + QDir::separator() + inf->GUID() + QDir::separator() + "1Cv8Log");
            if(ibDir.exists()){
               quint64 SizeLog = dirFileSize(ibDir.path());
               inf->setSizeEventLog(SizeLog / 1024 / 1024);
            }

            info_bases.insert(inf->Name(), inf);
        }
    }
}


QStringList Settings::ParseEventLogString(const QString &text)
{
    QStringList ArrayLines = QStringList();
    int end;
    if(text.startsWith(",")){
        end = text.length() - 3;
    }else
    {
        end = text.length() - 2;
    }

    QString text2 = text.mid(1, end) + ",";
    QString str;
    int delim = text2.indexOf(",");

    while (delim > 0) {
        str = str + text2.mid(0, delim).trimmed();
        text2 = text2.mid(delim + 1);

        if(CountSubstringInString(str, "{") == CountSubstringInString(str, "}") && std::remainder(CountSubstringInString(str, "\""), 2) == 0){

            if(str.startsWith("\"") && str.endsWith("\"")){
                str = str.mid(1, str.length() - 2);
            }
            ArrayLines.append(str);
            str = "";
        }else
            str = str + ",";

        delim = text2.indexOf(",");

    }

    return ArrayLines;
}

double Settings::CountSubstringInString(const QString &str, const QString &substr)
{
    QString _str = str;
    return (double)(str.length() - _str.replace(substr, "").length()) / (double)substr.length();
}
