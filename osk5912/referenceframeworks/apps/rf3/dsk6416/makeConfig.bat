echo off
rem -----------------------------------------------------------------
rem This file sets up the appropriate tiRoot configuration for TCONF. 
rem This will generate a configuration file for the application.
rem Run the CCS dosrun.bat first to get correct path settings
rem -----------------------------------------------------------------
echo on

tconf -Dconfig.tiRoot="%TI_DIR%" -Dconfig.platform="Dsk6416" -Dconfig.importPath="../appConfig;../../../include/tci;" appcfg.tcf
