@echo off
rem -----------------------------------------------------------------
rem This batch file will call the makeConfig.bat file for all
rem Reference Framework applications.  The makeConfig.bat file will 
rem generate the DSP/BIOS configuration file for a given application.
rem Run the CCS dosrun.bat first to get correct path settings
rem 
rem Note: This batch file will only run on Win NT, 2000, XP, etc.
rem       Win 98 users need to run makeConfig.bat manually
rem
rem Note: You may need to run this on each separate CCS installation
rem -----------------------------------------------------------------

pushd .
for /R .\apps %%i in (.) do (
    if exist %%i\makeConfig.bat (
        cd %%i 
        call makeConfig.bat        
        echo off        
    )
)
popd
echo.
echo Done!
