#pragma once

#include "revolution.h"
#include "kamek/hooks.h"
#include "oswreport.h"
#include "syati.h"

/**
 * See the included README for documentation.
 */

void removeLog()
{
    NANDDelete("log.txt");
}

kmBranch(0x8003B344, removeLog);

void appendToFile(const char *msg, const char *pFilePath)
{
    int createCode = NANDCreate(pFilePath, 0x30, 0);
    // -6 = File Already Exists. 0 = Success
    if (createCode == -6 || createCode == 0)
    {
        NANDFileInfo info;
        int code = NANDOpen(pFilePath, &info, 3);
        if (code == 0)
        {
            u32 size;
            NANDGetLength(&info, &size);
            u32 newSize = strlen(msg) + size;

            // create buffer and add original data into it
            unsigned char *buffer = new (0x20) u8[newSize];
            NANDRead(&info, buffer, size);

            // add message into buffer
            for (int i = 0; i < newSize; i++)
            {
                buffer[size + i] = msg[i]; 
            }

            // go to the start of the file and write the data
            NANDSeek(&info, 0, 0); 
            NANDWrite(&info, buffer, newSize);
            delete[] buffer;
        }
        NANDClose(&info);
    }
}

void OSWReport(const char *msg, ...)
{
    va_list mark;
    va_start(mark, msg);
    char buf[0x100];
    vsprintf(buf, msg, mark);
    va_end(mark);
    appendToFile(buf, "log.txt");
}

void OSWrite(const char *msg, const char *pFilePath, ...)
{
    va_list mark;
    va_start(mark, msg);
    char buf[0x100];
    vsprintf(buf, msg, mark);
    va_end(mark);
    appendToFile(buf, pFilePath);
}