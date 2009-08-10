rem @echo off

cd ../../..

if exist temp rmdir /S /Q temp
mkdir temp

@echo ****************** APPS ***************************************************
mkdir temp\Apps
if exist  Apps\StMfgTool.exe\Release rmdir /S /Q Apps\StMfgTool.exe\Release
if exist  Apps\StMfgTool.exe\Debug   rmdir /S /Q Apps\StMfgTool.exe\Debug
del /Q Apps\StMfgTool.exe\*.aps
del /Q Apps\StMfgTool.exe\*.bak
del /Q Apps\StMfgTool.exe\*.user
del /Q Apps\StMfgTool.exe\*.ncb
del /Q Apps\StMfgTool.exe\*.suo	

xcopy /I Apps\StMfgTool.exe temp\Apps\StMfgTool.exe
xcopy /I Apps\StMfgTool.exe\docs temp\Apps\StMfgTool.exe\docs
xcopy /I Apps\StMfgTool.exe\res temp\Apps\StMfgTool.exe\res

@echo ****************** COMMON *************************************************
mkdir temp\Common
xcopy /S /I Common temp\Common

@echo ****************** DRIVERS ************************************************
mkdir temp\Drivers
xcopy /I Drivers\StRcvryDriver\bin\*.*	temp\Drivers\StRcvryDriver\bin

@echo ****************** RESOURCES *****************************************
mkdir temp\resources
xcopy resources\closedfolder.bmp temp\resources
xcopy resources\openfolder.bmp temp\resources
xcopy resources\resourcefile.bmp temp\resources
xcopy resources\freescale_logo.bmp temp\resources

@echo ****************** LIBS ***************************************************
mkdir temp\Libs

mkdir temp\Libs\DevSupport
if exist  Libs\DevSupport\Release rmdir /S /Q Libs\DevSupport\Release
if exist  Libs\DevSupport\Debug   rmdir /S /Q Libs\DevSupport\Debug
del /Q Libs\DevSupport\*.bak
del /Q Libs\DevSupport\*.user
del /Q Libs\DevSupport\*.ncb
del /Q Libs\DevSupport\*.suo	
xcopy /S /I Libs\DevSupport	temp\Libs\DevSupport

mkdir temp\Libs\Loki
xcopy /S /I Libs\Loki temp\Libs\Loki

mkdir temp\Libs\WinSupport
if exist Libs\WinSupport\Release rmdir /S /Q Libs\WinSupport\Release 
if exist Libs\WinSupport\Debug rmdir /S /Q Libs\WinSupport\Debug
del /Q Libs\WinSupport\*.bak
del /Q Libs\WinSupport\*.user
del /Q Libs\WinSupport\*.ncb
del /Q Libs\WinSupport\*.suo	
xcopy /S /I Libs\WinSupport temp\Libs\WinSupport

mkdir temp\Libs\OtpAccessPitc\bin\Debug
copy Libs\OtpAccessPitc\StOtpAccessPitc.h temp\Libs\OtpAccessPitc\StOtpAccessPitc.h
copy Libs\OtpAccessPitc\bin\OtpAccess.* temp\Libs\OtpAccessPitc\bin
copy Libs\OtpAccessPitc\bin\Debug\OtpAccess.* temp\Libs\OtpAccessPitc\bin\Debug
mkdir temp\Libs\OtpAccessPitc\bin\VS2005
mkdir temp\Libs\OtpAccessPitc\bin\VS2005\Debug
copy Libs\OtpAccessPitc\bin\VS2005\OtpAccess.* temp\Libs\OtpAccessPitc\bin\VS2005
copy Libs\OtpAccessPitc\bin\VS2005\Debug\OtpAccess.* temp\Libs\OtpAccessPitc\bin\VS2005\Debug


@echo ****************** VERSIONING ****************************************************
copy Apps\StMfgTool.exe\version.h version.h
%PERL%\bin\perl.exe "Customization\bin\version.pl" release
call ver.bat

@echo ******************* ZIP **********************************************************
if exist "stmfgtool.v%STMP_BUILD_VERSION%_src.zip" del "stmfgtool.v%STMP_BUILD_VERSION%_src.zip"
"%TOOLS%\pkzipc.exe" -silent -add  -exclude=?svn -dir=relative "stmfgtool.v%STMP_BUILD_VERSION%_src.zip"  "temp\*.*"

if exist "stmfgtool.v%STMP_BUILD_VERSION%_profiles.zip" del "stmfgtool.v%STMP_BUILD_VERSION%_profiles.zip"
"%TOOLS%\pkzipc.exe" -silent -add  -exclude=?svn -dir=relative "stmfgtool.v%STMP_BUILD_VERSION%_profiles.zip"  "Apps\StMfgTool.exe\bin\profiles\*.*"

cd Apps/StMfgTool.exe/bin