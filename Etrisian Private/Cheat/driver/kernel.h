#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>

inline uintptr_t BaseAddress;

#define PHOENIX_READ   CTL_CODE(FILE_DEVICE_UNKNOWN, 0x5912, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define PHOENIX_WRITE  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x5913, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define PHOENIX_BASE   CTL_CODE(FILE_DEVICE_UNKNOWN, 0x5914, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

#define PHOENIX_SECURITY 0x09A73C01

typedef struct _phoenix_rw {
    INT32 security;
    INT32 process_id;
    ULONGLONG address;
    ULONGLONG buffer;
    ULONGLONG size;
} phoenix_rw, * pphoenix_rw;

typedef struct _phoenix_ba {
    INT32 security;
    INT32 process_id;
    ULONGLONG* address;
} phoenix_ba, * pphoenix_ba;

namespace mem {
    inline HANDLE driver_handle = INVALID_HANDLE_VALUE;
    inline INT32 process_id = 0;

    inline bool find_driver() {
        driver_handle = CreateFileW(L"\\\\.\\PhoenixCore", GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

        if (!driver_handle || driver_handle == INVALID_HANDLE_VALUE)
            return false;

        return true;
    }

    inline void read_physical(PVOID address, PVOID buffer, DWORD size) {
        phoenix_rw arguments = { 0 };
        arguments.security = PHOENIX_SECURITY;
        arguments.address = (ULONGLONG)address;
        arguments.buffer = (ULONGLONG)buffer;
        arguments.size = (ULONGLONG)size;
        arguments.process_id = process_id;

        DeviceIoControl(driver_handle, PHOENIX_READ, &arguments, sizeof(arguments), nullptr, 0, nullptr, nullptr);
    }

    inline void write_physical(PVOID address, PVOID buffer, DWORD size) {
        phoenix_rw arguments = { 0 };
        arguments.security = PHOENIX_SECURITY;
        arguments.address = (ULONGLONG)address;
        arguments.buffer = (ULONGLONG)buffer;
        arguments.size = (ULONGLONG)size;
        arguments.process_id = process_id;

        DeviceIoControl(driver_handle, PHOENIX_WRITE, &arguments, sizeof(arguments), nullptr, 0, nullptr, nullptr);
    }

    inline uintptr_t find_image() {
        uintptr_t image_address = 0;
        phoenix_ba arguments = { 0 };

        arguments.security = PHOENIX_SECURITY;
        arguments.process_id = process_id;
        arguments.address = (ULONGLONG*)&image_address;

        DeviceIoControl(driver_handle, PHOENIX_BASE, &arguments, sizeof(arguments), nullptr, 0, nullptr, nullptr);

        return image_address;
    }

    inline INT32 find_process(LPCTSTR process_name) {
        PROCESSENTRY32 pt;
        HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hsnap == INVALID_HANDLE_VALUE)
            return 0;

        pt.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hsnap, &pt)) {
            do {
                if (!lstrcmpi(pt.szExeFile, process_name)) {
                    CloseHandle(hsnap);
                    process_id = pt.th32ProcessID;
                    return pt.th32ProcessID;
                }
            } while (Process32Next(hsnap, &pt));
        }
        CloseHandle(hsnap);

        return 0;
    }

    inline void close_driver() {
        if (driver_handle && driver_handle != INVALID_HANDLE_VALUE) {
            CloseHandle(driver_handle);
            driver_handle = INVALID_HANDLE_VALUE;
        }
    }
}

template <typename T>
T read(uint64_t address) {
    T buffer{ };
    mem::read_physical((PVOID)address, &buffer, sizeof(T));
    return buffer;
}

template <typename T>
T write(uint64_t address, T value) {
    mem::write_physical((PVOID)address, &value, sizeof(T));
    return value;
}
