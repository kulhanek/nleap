// return suffix relative to OB installation directory
#include <stdio.h>
#include <string>
#include <map>
#include <iostream>

// _WIN32_WINNT is because GetModuleHandleEx
// see  http://msdn.microsoft.com/en-us/library/ms683200%28v=vs.85%29.aspx (last remark)
#define _WIN32_WINNT 0x0501
#include <windows.h>

using namespace std;

// -----------------------------------------------------------------------------
// this is not probably the best solution - but it should work
// -----------------------------------------------------------------------------

const std::string nprefix(void)
{
    static string prefix;
    if( ! prefix.empty() ) return(prefix);

    // get the path to libnleap.dll module
    HMODULE lib_handle = NULL;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
                      reinterpret_cast <LPCTSTR>(&nprefix),&lib_handle);
    char buffer[2048];
    buffer[0] = '\0';
    if( lib_handle ){
        GetModuleFileName(lib_handle,buffer,2048);
        buffer[2047] = '\0';
        FreeLibrary(lib_handle);
    }
    std::string nleap_dll_name(buffer);

    // get app directory
    int last = 0;
    int count = 0;
    for(int i=(int)nleap_dll_name.size()-1; i >=0; i--){
        if( nleap_dll_name[i] == '\\' ){
            count++;
            if(count == 2){
                last = i;
            }
        }
    }
    prefix = nleap_dll_name.substr(0,last);

    return(prefix);
}

// -----------------------------------------------------------------------------
