/**
 * Copyright (C) 2006  Ole Andr� Vadla Ravn�s <oleavr@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#pragma once

#include "byte_buffer.h"
#include <map>

template <class T>
class ContextTracker
{
public:
    ContextTracker()
    {
        InitializeCriticalSection(&cs);
    }

    ~ContextTracker()
    {
    }

    DWORD GetContextID(T handle)
    {
        DWORD id;

        EnterCriticalSection(&cs);

        ContextMap::iterator iter = contexts.find(handle);
        if (iter != contexts.end())
        {
            id = iter->second;
        }
        else
        {
            id = ospy_rand();
            contexts[handle] = id;
        }

        LeaveCriticalSection(&cs);

        return id;
    }

    void RemoveContextID(T handle)
    {
        EnterCriticalSection(&cs);

        ContextMap::iterator iter = contexts.find(handle);
        if (iter != contexts.end())
        {
            contexts.erase(iter);
        }

        LeaveCriticalSection(&cs);
    }

private:
    CRITICAL_SECTION cs;

    typedef map<T, DWORD, less<T>, MyAlloc<pair<T, DWORD>>> ContextMap;
    ContextMap contexts;
};

void get_module_name_for_address(LPVOID address, TCHAR *buf, int buf_size);
BOOL get_module_base_and_size(const TCHAR *module_name, LPVOID *base, DWORD *size, char **error);
BOOL address_has_bytes(LPVOID address, unsigned char *buf, int len);
bool cur_process_is(const TCHAR *name);

DWORD ospy_rand();

typedef struct {
    OICString name;
    DWORD preferredStartAddress;
    DWORD startAddress;
    DWORD endAddress;
} OModuleInfo;

typedef LPTSTR (NTAPI * RtlIpv4AddressToStringFunc)(const IN_ADDR *Addr, LPTSTR S);

class CUtil
{
public:
    static void Init();
    static void UpdateModuleList();

    static void Ipv4AddressToString(const IN_ADDR *addr, TCHAR *str);

    static const OTString &GetProcessName() { return m_processName; }
    static OString GetModuleNameForAddress(DWORD address);
    static OModuleInfo GetModuleInfo(const OICString &name) { return m_modules[name]; }
    static OVector<OModuleInfo>::Type GetAllModules();
    static bool AddressIsWithinExecutableModule(DWORD address);

    static OTString CreateBackTrace(void *address);

private:
    static DWORD FindPreferredImageBaseOf(const WCHAR *filename);
    static OModuleInfo *GetModuleInfoForAddress(DWORD address);

    static CRITICAL_SECTION m_cs;
    static RtlIpv4AddressToStringFunc m_rtlIpv4AddressToStringImpl;
    static OTString m_processName;
    static OMap<OICString, OModuleInfo>::Type m_modules;
    static DWORD m_lowestAddress;
    static DWORD m_highestAddress;
};
