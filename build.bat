@echo off
echo detect_delim C语言版本编译脚本

REM 检查是否有GCC
where gcc >nul 2>&1
if %errorlevel% == 0 (
    echo 找到GCC编译器，开始编译...
    gcc -Wall -Wextra -O2 -std=c99 -o detect_delim.exe detect_delim.c
    if %errorlevel% == 0 (
        echo 编译成功！生成文件: detect_delim.exe
        echo.
        echo 测试基本功能:
        detect_delim.exe
    ) else (
        echo 编译失败！
    )
    goto :end
)

REM 检查是否有MinGW
where mingw32-gcc >nul 2>&1
if %errorlevel% == 0 (
    echo 找到MinGW编译器，开始编译...
    mingw32-gcc -Wall -Wextra -O2 -std=c99 -o detect_delim.exe detect_delim.c
    if %errorlevel% == 0 (
        echo 编译成功！生成文件: detect_delim.exe
        echo.
        echo 测试基本功能:
        detect_delim.exe
    ) else (
        echo 编译失败！
    )
    goto :end
)

REM 检查是否有Visual Studio编译器
where cl >nul 2>&1
if %errorlevel% == 0 (
    echo 找到Visual Studio编译器，开始编译...
    cl /O2 /Fe:detect_delim.exe detect_delim.c
    if %errorlevel% == 0 (
        echo 编译成功！生成文件: detect_delim.exe
        echo.
        echo 测试基本功能:
        detect_delim.exe
    ) else (
        echo 编译失败！
    )
    goto :end
)

echo 错误: 未找到C编译器！
echo.
echo 请安装以下编译器之一:
echo 1. MinGW-w64: https://www.mingw-w64.org/
echo 2. Visual Studio: https://visualstudio.microsoft.com/
echo 3. TDM-GCC: https://jmeubank.github.io/tdm-gcc/
echo.
echo 或者使用在线编译器测试代码:
echo - https://godbolt.org/
echo - https://replit.com/

:end
pause