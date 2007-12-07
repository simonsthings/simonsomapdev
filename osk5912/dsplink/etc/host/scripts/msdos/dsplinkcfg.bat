@echo off
rem ============================================================================
rem @file   dsplinkcfg.bat
rem
rem @path   %DSPLINK%\etc\host\scripts\msdos
rem
rem @desc   Calls perl script to create build configuration for
rem         DSP/BIOS(TM) Link
rem
rem @ver    01.10.01
rem ============================================================================
rem Copyright (c) Texas Instruments Incorporated 2002-2004
rem
rem Use of this software is controlled by the terms and conditions found in the
rem license agreement under which this software has been supplied or provided.
rem ============================================================================

pushd %DSPLINK%\config\bin
perl dsplinkcfg.pl
popd
