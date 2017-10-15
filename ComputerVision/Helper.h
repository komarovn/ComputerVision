#pragma once

string SystemToStl(String ^s) {
    using namespace Runtime::InteropServices;
    const char* ptr = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
    return string(ptr);
};