@echo off
set pdbFieName=%1
call :extract %pdbFieName%
goto :eof

:extract
set symFileName=%~n1.sym
dump_syms.exe %pdbFieName% > %symFileName%

for /f "delims=" %%a in (%symFileName%) do (
    set contentLine0=%%a
    goto 0
)
:0
for /f "tokens=4* delims= " %%a in ("%contentLine0%") do (
    set identity=%%a
)

set symbolDir=symbols\%pdbFieName%\%identity%
md %symbolDir%
move %symFileName% %symbolDir%