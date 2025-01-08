#pragma once

void removeLog();
void appendToFile(const char *msg, const char *pFilePath);
void OSWReport(const char *msg, ...);
void OSWrite(const char *msg, const char *pFilePath, ...);