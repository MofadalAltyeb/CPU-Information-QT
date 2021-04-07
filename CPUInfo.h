#ifndef CPUINFO_H
#define CPUINFO_H

#include <QtCore>
#include <intrin.h>

#define ProcessorInfoLoc "HKEY_LOCAL_MACHINE\\HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"
#define ProcessorNameStr "ProcessorNameString"

struct qCPUInfo
{
    uint32_t regs[4];
    QString cpu_uuid;
    QString cpumanfc;
    QString cpumodel;
};

static inline void GetCPUID(uint *eax, uint *ebx, uint *ecx, uint *edx)
{
#ifdef _WIN32FFFF //works well also
    uint32_t native_regs[4] = {};
    __cpuidex((int *)&native_regs, 0, 0);
    memcpy(eax, &native_regs[0], sizeof (uint32_t));
    memcpy(ebx, &native_regs[1], sizeof (uint32_t));
    memcpy(ecx, &native_regs[2], sizeof (uint32_t));
    memcpy(edx, &native_regs[3], sizeof (uint32_t));
#else
    /* ecx is often an input as well as an output. */
    asm volatile("cpuid"
                 : "=a" (*eax),
                 "=b" (*ebx),
                 "=c" (*ecx),
                 "=d" (*edx)
                 : "0" (*eax), "2" (*ecx)
                 : "memory");

#endif
}

int getCPUInfo(qCPUInfo &cpuinfo)
{
    cpuinfo.regs[0] = 0;

    GetCPUID(&cpuinfo.regs[0], &cpuinfo.regs[1], &cpuinfo.regs[2], &cpuinfo.regs[3]);

    const QString cpuid[4] =
    {
        QString("%0").arg(cpuinfo.regs[0]
        , sizeof(uint32_t)*2, 16, QChar('0')),
        QString("%0").arg(cpuinfo.regs[1]
        , sizeof(uint32_t)*2, 16, QChar('0')),
        QString("%0").arg(cpuinfo.regs[2]
        , sizeof(uint32_t)*2, 16, QChar('0')),
        QString("%0").arg(cpuinfo.regs[3]
        , sizeof(uint32_t)*2, 16, QChar('0'))
    };

    const QString venid[3] =
    {
        QByteArray::fromHex(QString("%0").arg(qFromBigEndian(cpuinfo.regs[1])
        , sizeof(uint32_t)*2, 16, QChar('0')).toUtf8()),
        QByteArray::fromHex(QString("%0").arg(qFromBigEndian(cpuinfo.regs[3])
        , sizeof(uint32_t)*2, 16, QChar('0')).toUtf8()),
        QByteArray::fromHex(QString("%0").arg(qFromBigEndian(cpuinfo.regs[2])
        , sizeof(uint32_t)*2, 16, QChar('0')).toUtf8()),
    };

    QSettings *qcpu = new QSettings(ProcessorInfoLoc, QSettings::NativeFormat);
    cpuinfo.cpumodel.append(qcpu->value(ProcessorNameStr).toString());
    delete qcpu;

    cpuinfo.cpu_uuid.append(QString("%0-%1-%2-%3").arg(cpuid[0], cpuid[1], cpuid[2], cpuid[3]).toUpper());
    cpuinfo.cpumanfc.append(QString("%0%1%2").arg(venid[0], venid[1], venid[2]));

    return cpuinfo.cpu_uuid.size();
}

#endif // CPUINFO_H
