#include <windows.h>
#include <wincrypt.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include"sha1.h"

#pragma comment(lib, "Advapi32.lib")

std::string sha1(const std::string& data)
{
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    BYTE hash[20];
    DWORD hashLen = 20;

    if (!CryptAcquireContext(
            &hProv,
            NULL,
            NULL,
            PROV_RSA_FULL,
            CRYPT_VERIFYCONTEXT))
    {
        throw std::runtime_error("CryptAcquireContext failed");
    }

    if (!CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash))
    {
        CryptReleaseContext(hProv, 0);
        throw std::runtime_error("CryptCreateHash failed");
    }

    if (!CryptHashData(
            hHash,
            reinterpret_cast<BYTE*>(const_cast<char*>(data.c_str())),
            static_cast<DWORD>(data.size()),
            0))
    {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        throw std::runtime_error("CryptHashData failed");
    }

    if (!CryptGetHashParam(
            hHash,
            HP_HASHVAL,
            hash,
            &hashLen,
            0))
    {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        throw std::runtime_error("CryptGetHashParam failed");
    }

    std::stringstream ss;

    for (DWORD i = 0; i < hashLen; i++)
    {
        ss << std::hex
           << std::setw(2)
           << std::setfill('0')
           << static_cast<int>(hash[i]);
    }

    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);

    return ss.str();
}
