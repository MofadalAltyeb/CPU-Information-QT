#include <QCoreApplication>
#include <CPUInfo.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qInfo() << "#################### Reading cpu information ###################";

    qCPUInfo info = {};

    if(getCPUInfo(info))
    {
        qInfo() << QString("qCPUInfo::cpu_uuid:%0").arg(info.cpu_uuid);
        qInfo() << QString("qCPUInfo::cpumanfc:%0").arg(info.cpumanfc);
        qInfo() << QString("qCPUInfo::cpumodel:%0").arg(info.cpumodel);
    }

    return a.exec();
}
