echo off
rem -----------------------------------------------------------------
rem This file sets up the appropriate tiRoot configuration for TCONF. 
rem This will generate a configuration file for the application.
rem Run the CCS dosrun.bat first to get correct path settings
rem -----------------------------------------------------------------
rem An imports path to the CCS dsk5510 include dirctory has been added
rem due to a bug reported with the EMIF setup in CDB.  See SDSsq29434
rem for more details regarding the bug
rem ------------------------------------------------------------------
echo on

tconf -Dconfig.tiRoot="%TI_DIR%" -Dconfig.platform="Dsk5510" -Dconfig.importPath="../../appConfig;../../../../include/tci;%TI_DIR%/c5500/dsk5510/include;" appcfg.tcf

