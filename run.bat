@echo off

REM Check if the required number of arguments is passed
IF "%~2"=="" (
    echo Usage: %~0 ^<number_of_moves^> ^<path_to_test_file^>
    exit /b 1
)

REM Extracting the arguments
SET "numMoves=%~1"
SET "testFilePath=%~2"

REM Compiling the C++ program
g++ -o humbug humbug.cpp

REM Check if the compilation was successful
IF ERRORLEVEL 1 (
    echo Compilation failed.
    exit /b 1
)

REM Running the compiled program with the provided arguments
humbug %numMoves% %testFilePath%

REM Keep the terminal open
cmd /k
