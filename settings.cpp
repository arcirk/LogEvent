#include "settings.h"
#include <QFile>
#include <QDir>
#include <QRegularExpression>
#include <QLibrary>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#ifdef _WINDOWS
#include <string>
#include <atlbase.h> // For ATL autorelease classes (CComBSTR, CComPtr)
#include <wbemidl.h> // For WMI
#pragma comment(lib, "wbemuuid.lib")
#endif

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

#ifdef _WINDOWS
QString Settings::get_service_command_line()
{
    HRESULT hr = ::CoInitializeSecurity(NULL, -1, NULL, NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL, EOAC_NONE, NULL);

    CComPtr<IWbemLocator> pWbemLocator;
    hr = pWbemLocator.CoCreateInstance(CLSID_WbemLocator);

    CComPtr<IWbemServices> pWbemServices;
    hr = pWbemLocator->ConnectServer(CComBSTR(L"root\\cimv2"), NULL, NULL, 0, NULL, 0, NULL, &pWbemServices);

    CComPtr<IEnumWbemClassObject> pEnum;
    CComBSTR cbsQuery = L"SELECT * FROM Win32_Service WHERE PathName like '%ragent.exe%'";
    hr = pWbemServices->ExecQuery(CComBSTR("WQL"), cbsQuery, WBEM_FLAG_FORWARD_ONLY, NULL, &pEnum);

    ULONG uObjectCount = 0;
    CComPtr<IWbemClassObject> pWmiObject;
    hr = pEnum->Next(WBEM_INFINITE, 1, &pWmiObject, &uObjectCount);

    CComVariant cvtVersion;
    hr = pWmiObject->Get(L"PathName", 0, &cvtVersion, 0, 0);

    std::basic_string<TCHAR> intermediate(cvtVersion.bstrVal);

    QString q;

    #ifdef _UNICODE
    q = QString::fromStdWString(intermediate);
    #else
    q = QString::fromStdString(intermediate);
    #endif

    return q;
}

void Settings::parse_command_line(const QString& cmd, QString& result){

    //QString ClusterFiles;
    QString s_temp = cmd;
    int indPort = cmd.indexOf("-regport");
    int indSrvinfo = cmd.indexOf("-d");
    QString srvinfoDir;

    QString PortStr;

    if (indPort > 0){
        int i = 0;
        QString Simb = s_temp.mid(indPort + 8 + i, 1);

        while (QString("0123456789 ").contains(Simb)){

            PortStr = PortStr + Simb;
            i = i + 1;
            Simb = s_temp.mid(indPort + 8 + i, 1);
        }
        srvinfoDir = "reg_" + PortStr.trimmed();
      }

    if(indSrvinfo > 0){

        QString PathStr = s_temp.mid(indSrvinfo + 2);
        QString Simb = PathStr.mid(0, 1);

        while (QString(" \"").contains(Simb)){
            PathStr = PathStr.mid(2);
            Simb = PathStr.mid(1, 1);
        }

        int ind = PathStr.indexOf("\"");
        if(ind > 0)
            result = PathStr.mid(0, ind);
    }

    if(!result.isEmpty()){
        result.append(QDir::separator());
        result.append(srvinfoDir);
        result = QDir::fromNativeSeparators(result);
    }else
        result = "";
}

void Settings::getSettings()
{
    QFile file("settings.json");
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if(!doc.isNull()){
        QJsonObject obj = doc.object();
        auto _logDbPateh = obj.find("dirsrvinfo");
        if(_logDbPateh->isString()){
            srv_v8info = _logDbPateh->toString().toStdString();
        }
        auto _mColVisible = obj.find("ColumnVisible");
        if(_mColVisible->isArray()){
            QJsonArray arr = _mColVisible->toArray();
            for (auto iter : arr) {
                QJsonObject item = iter.toObject();
                bool val = item["value"].toBool();
                selectedCols[item["name"].toString()] = val;
            }

        }
    }
    file.close();
}

#endif

Settings::Settings()//QObject *parent)
   // : QObject{parent}
{
    getSettings();
}

void Settings::get_server_info(const QString& dirsrvinfo)
{

    QString v8srvinfo = "";

    if(!dirsrvinfo.isEmpty()){
        v8srvinfo = dirsrvinfo;
    }
#ifdef _WINDOWS

    parse_command_line(get_service_command_line(), v8srvinfo);

#endif

    if(v8srvinfo.isEmpty())
        return;

    QDir folder(v8srvinfo);
    if(!folder.exists())
        return;

    QFile lstFile(folder.path() + QDir::separator() + "1CV8Clst.lst");
    if(!lstFile.open(QFile::ReadOnly))
        return;

    QString text = QString::fromUtf8(lstFile.readAll());

    lstFile.close();

    QStringList ClusterSetting = ParseEventLogString(text);
    if(ClusterSetting.size() > 2){
        QStringList DatabaseSettings = ParseEventLogString(ClusterSetting[2]);

        for (const auto& itr : DatabaseSettings) {

            QStringList DatabaseDescription = ParseEventLogString(itr);

            if(DatabaseDescription.size() == 0)
                continue;

            auto inf = new Infobases();
            inf->setName(DatabaseDescription[1]);
            inf->setGUID(DatabaseDescription[0]);
            inf->setDescription(DatabaseDescription[2]);
            inf->setSizeEventLog(0);
            inf->setPathLogEvent(v8srvinfo + QDir::separator() + inf->GUID() + QDir::separator() + "1Cv8Log");

            QDir ibDir(inf->PathLogEvent());
            if(ibDir.exists()){
               quint64 SizeLog = dirFileSize(ibDir.path());
               inf->setSizeEventLog(SizeLog / 1024 / 1024);
            }

            info_bases.insert(inf->Name(), inf);
        }
    }

    //f(srv_v8info != v8srvinfo)
        setV8Root(v8srvinfo);
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

QString Settings::v8srvinfo_catalog(){
    return QString::fromStdString(srv_v8info);
}
