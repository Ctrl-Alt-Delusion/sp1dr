@echo off
setlocal enabledelayedexpansion
for /R %1 %%f in (*.cpp) do (
    set "file=%%f"
    echo !file:\=/!
)
