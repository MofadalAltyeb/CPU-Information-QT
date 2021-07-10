#include <QCoreApplication>
#include <CPUInfo.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qInfo() << "#################### Reading cpu information ###################";

    qCPUInfo info = {};

    if(getCPUInfo(info))
    {
        qInfo() << QString("qCPUInfo::cpuID:%0").arg(info.cpuID);
        qInfo() << QString("qCPUInfo::devID:%0").arg(info.devID);
    }

    return a.exec();
}
