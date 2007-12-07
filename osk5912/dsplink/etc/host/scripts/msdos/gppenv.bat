@echo off
rem ============================================================================
rem @file   gppenv.bat
rem
rem @path   \dsplink\etc\host\scripts\msdos
rem
rem @desc   Sets the necessary environment for GPP side development for
rem         DSP/BIOS(TM) Link
rem
rem @ver    01.10.01
rem ============================================================================
rem Copyright (c) Texas Instruments Incorporated 2002-2004
rem
rem Use of this software is controlled by the terms and conditions found in the
rem license agreement under which this software has been supplied or provided.
rem ============================================================================


rem ----------------------------------------------------------------------------
rem @name   DSPLINK
rem
rem @desc   Root directory of DSP/BIOS LINK.
rem ----------------------------------------------------------------------------
set DSPLINK=L:\dsplink

rem ----------------------------------------------------------------------------
rem @name   PATH
rem
rem @desc   Appends the path to the host scripts
rem ----------------------------------------------------------------------------
set PATH=%PATH%;%DSPLINK%\etc\host\scripts\msdos
