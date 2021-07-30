#ifndef CCRASHSTACK_H
#define CCRASHSTACK_H

#ifdef _WIN32

#include <windows.h>
#include <QString>

class CCrashStack
{
private:
    PEXCEPTION_POINTERS m_pException;

private:
    QString GetModuleByRetAddr(PBYTE Ret_Addr, PBYTE & Module_Addr);
    QString GetCallStack(PEXCEPTION_POINTERS pException);
    QString GetVersionStr();
    bool GetHardwareInaformation(QString &graphics_card, QString &sound_deivce);

public:
    CCrashStack(PEXCEPTION_POINTERS pException);

    QString GetExceptionInfo();
};

#endif

#endif // CCRASHSTACK_H
