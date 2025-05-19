@echo off
REM Clean old object files and executable
del *.o 2>nul
del *.obj 2>nul
del main.exe 2>nul

REM Compile all .cpp files together
g++ main.cpp Client.cpp Wallet.cpp Blockchain.cpp Transaction.cpp TransactionList.cpp ClientBST.cpp EntityVector.cpp -o main.exe

REM Check if compilation succeeded
if errorlevel 1 (
    echo Compilation failed.
    pause
    exit /b 1
)

REM Run the program
echo Compilation succeeded. Running the program...
main.exe

pause
