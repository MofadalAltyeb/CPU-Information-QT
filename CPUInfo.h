#ifndef CPUINFO_H
#define CPUINFO_H

#include <QtCore>
#include <intrin.h>
#include <cpuid.h>

struct qCPUInfo
{
    QString cpuID;
    QString devID;
};

QByteArray GetMD5Hash(QByteArray buff)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(buff);
    return hash.result().toHex();
}
QString GetCpuId()
{
    QString cpuid = "";
    unsigned int dwBuf[4];
    int InfoType = 1;
    __cpuid(InfoType, dwBuf[0], dwBuf[1], dwBuf[2], dwBuf[3]);
    __cpuid_count(InfoType, 0, dwBuf[0],dwBuf[1],dwBuf[2],dwBuf[3]);
    cpuid = QString::number(dwBuf[3], 16).toUpper();
    cpuid = cpuid + QString::number(dwBuf[0], 16).toUpper();
    return cpuid;
}

QString GetMeCode()
{
    QString strMe = GetCpuId() ;
    QByteArray ba = strMe.toLatin1();
    QString strTemp = ba.toBase64();
    return strTemp;
}

int getCPUInfo(qCPUInfo &cpuinfo)
{
    QByteArray hwid = GetMeCode().toUtf8().toHex();
    QByteArray algo = {};
    uint8_t hwid_cnt = hwid.length()/2;
    for (int i = 0; i < hwid_cnt; i++)
    {
        QByteArray sum0 = GetMD5Hash(hwid.mid(i*2, 2)).mid(hwid_cnt, 2);
        QByteArray csum = GetMD5Hash(sum0).mid(i, 2).toUpper();
        algo.append(csum);
    }

    const QString pcuid =
    {
        QString("%0").arg(GetMD5Hash(algo).data())
    };

    cpuinfo.cpuID.append(QString("%0").arg(hwid.data()).toUpper());
    cpuinfo.devID.append(QString("%0").arg(pcuid).toUpper());

    return 1;
}

#endif // CPUINFO_H
