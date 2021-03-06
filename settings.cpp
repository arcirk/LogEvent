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

void Settings::parse_command_line(const QString& cmd, QString& result, QString& root_result){

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
        root_result = QDir::fromNativeSeparators(result);
        result.append(QDir::separator());
        result.append(srvinfoDir);
        result = QDir::fromNativeSeparators(result);
    }else
        result = "";
}
#endif

QMap<QString, Infobases *> Settings::get_infobases()
{
    return info_bases;
}

QMap<QString, bool>& Settings::get_selected_cols()
{
    return selectedCols;
}

QString Settings::get_code_table_from_alias_index(int index)
{
    auto itr = _code_tables_from_alias_index.find(index);
    if(itr != _code_tables_from_alias_index.end())
        return itr.value();
    else
        return "";

}

QString Settings::get_field_name_from_alias_index(int index)
{
    auto itr = _fields_from_alias_index.find(index);
    if(itr != _fields_from_alias_index.end())
        return itr.value();
    else
        return "";
}

void Settings::set_v8srvinfo_logevent_catalog(const QString &val)
{
    _v8srvinfo = val;
}


void Settings::getSettings()
{
    QFile file("settings.json");
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    selectedCols.clear();

    for(auto itr : ColumnNamesAliases){
        selectedCols.insert(itr, true);
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if(!doc.isNull()){
        QJsonObject obj = doc.object();
        auto _logDbPateh = obj.find("dirsrvinfo");
        if(_logDbPateh->isString()){
            _root_path = _logDbPateh->toString();
        }
        auto _mColVisible = obj.find("ColumnVisible");
        if(_mColVisible->isObject()){
            QJsonObject arr = _mColVisible->toObject();
            for (auto iter = arr.begin(); iter != arr.end(); ++iter) {
                QString key = iter.key();
                bool val = iter.value().toBool();
                if(selectedCols.find(key) != selectedCols.end())
                    selectedCols[key] = val;
            }

        }
    }
    file.close();
}

void Settings::saveSettings()
{
    QJsonDocument m_doc{};
    QJsonObject obj = QJsonObject();
    obj.insert("dirsrvinfo", _root_path);
    QJsonObject item = QJsonObject();

    for (auto itr = selectedCols.begin(); itr != selectedCols.end(); ++itr) {       
        if(!itr.key().isEmpty()){
            item.insert(itr.key(), itr.value());
        }
    }

    obj.insert("ColumnVisible", item);

    m_doc.setObject(obj);

    QString fileName = "settings.json";

    QFile jsonFile(fileName);
    if (!jsonFile.open(QIODevice::WriteOnly))
    {
        return;
    }
    jsonFile.write(QJsonDocument(m_doc).toJson(QJsonDocument::Indented));
    jsonFile.close();
}

Settings::Settings(QObject *parent)
    : QObject{parent}
{
    _code_tables_from_alias_index.insert(1, "AppCodes");
    _code_tables_from_alias_index.insert(2, "EventCodes");
    _code_tables_from_alias_index.insert(3, "ComputerCodes");
    _code_tables_from_alias_index.insert(5, "MetadataCodes");
    _code_tables_from_alias_index.insert(6, "UserCodes");
    _fields_from_alias_index.insert(0, "date");
    _fields_from_alias_index.insert(1, "appCode");
    _fields_from_alias_index.insert(2, "eventCode");
    _fields_from_alias_index.insert(3, "computerCode");
    _fields_from_alias_index.insert(4, "comment");
    _fields_from_alias_index.insert(5, "metadataCodes");
    _fields_from_alias_index.insert(6, "userCode");
    _fields_from_alias_index.insert(7, "dataType");
    _fields_from_alias_index.insert(9, "dataType");
    _fields_from_alias_index.insert(9, "dataPresentation");
    _fields_from_alias_index.insert(10, "rowID");
}

void Settings::get_server_info(const QString& dirsrvinfo)
{

    info_bases.clear();

    QString v8srvinfo = "";
    QString _root;

    if(!dirsrvinfo.isEmpty()){

        QDir fl(dirsrvinfo);
        if(!fl.exists())
            return;

        QFile lst(fl.path() + QDir::separator() + "1cv8wsrv.lst");
        if(!lst.open(QFile::ReadOnly))
            return;

        QString lst_text = QString::fromUtf8(lst.readAll());

        lst.close();

        QStringList rootSettings = ParseEventLogString(lst_text);
        if(rootSettings.size() == 0)
            return;
        QStringList opt = ParseEventLogString(rootSettings[0]);
        QStringList opt1 = ParseEventLogString(opt[1]);
        QString sz_port;
        if(opt1.size() > 2)
            sz_port = opt1[2].trimmed();
        else
            return;

        _root = fl.path();

        v8srvinfo = QDir::fromNativeSeparators(fl.path() + QDir::separator() + "reg_" + sz_port);

    }else{
#ifdef _WINDOWS
    parse_command_line(get_service_command_line(), v8srvinfo, _root);
#endif
    }

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
               double dbl = (double)SizeLog / (double)1024 / (double)1024;
               inf->setSizeEventLog(dbl);
            }


            info_bases.insert(inf->Name(), inf);
        }
    }

    set_v8srvinfo_catalog(_root);
    set_v8srvinfo_logevent_catalog(folder.path());
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

QString Settings::v8srvinfo_catalog() const{
    return _root_path;
}

void Settings::set_v8srvinfo_catalog(const QString &val)
{
    _root_path = val;
}

QString Settings::v8srvinfo_logevent_catalog()
{
    return _v8srvinfo;
}
