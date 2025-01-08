# OSWReport
Adds a version of OSReport that writes to a log file.
Make sure to add it as a dependency to your `ModuleInfo.json` like so:

```json
{
  "Name": "My cool module",
  "Author": "Me",
  "Description": "Does stuff.",
  "InstallDependencies": ["OSWReport"]
}
```

Included functions:
## `void appendToFile(const char *msg, const char *pFilePath)`
Similar to OSWrite, but without formatting.
## `void OSWReport(const char *msg, ...)`
Writes a formatted message to `log.txt` in the NAND, next to the game's save data.
Similar to OSReport, but not a replacement. This is significantly slower, and can slow down the game if used too frequently. It also does not write to the log viewable with Dolphin or a USBGecko.

The log is automatically deleted upon booting up the game, and is created when the first OSWReport is done.

Examples:
```cpp
OSWReport("Hello World!\n");
```

```cpp
int a = 3;
OSWReport("When are we getting Super Mario Galaxy %d?\n", a);
```

## `void OSWrite(const char *msg, const char *pFilePath, ...)`
Does the same thing as OSWReport, but allows specifying a file path. **Custom logs do not get automatically deleted.**

Examples:
```cpp
OSWReport("Woah, a custom log?\n", "custom.txt");
```

```cpp
int daysWithoutNonsense = 0;
OSWReport("%d days without nonsense\n", "custom.txt", daysWithoutNonsense);
```