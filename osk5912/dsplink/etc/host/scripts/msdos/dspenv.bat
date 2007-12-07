@echo off
rem ===========================================================================
rem @file   dspenv.bat
rem
rem @path   $(DSPLINK)\etc\host\scripts\msdos
rem
rem @desc   Sets the necessary PATH settings for DSP side development for
rem         DSP/BIOS(TM) Link
rem
rem @ver    01.10.01
rem ===========================================================================
rem Copyright (c) Texas Instruments Incorporated 2002-2004
rem
rem Use of this software is controlled by the terms and conditions found in the
rem license agreement under which this software has been supplied or provided.
rem ============================================================================


rem  ===========================================================================
rem  Define project name
rem  ===========================================================================
set THE_PROJECT=DSP/BIOS LINK

rem  ===========================================================================
rem  Set PATH to various tools
rem  ===========================================================================

rem  ---------------------------------------------------------------------------
rem  Search for PERL installation
rem  ---------------------------------------------------------------------------
set THE_PATH_PERL=Perl

for %%i in (C, D, E) do if exist "%%i:\%THE_PATH_PERL%" set THE_LOCATION_PERL=%%i:\%THE_PATH_PERL%

rem  ---------------------------------------------------------------------------
rem  Search for CCS installation
rem  ---------------------------------------------------------------------------
set THE_PATH_CCS=CCStudio_v3.1

for %%i in (C, D, E) do if exist "%%i:\%THE_PATH_CCS%" set THE_LOCATION_CCS=%%i:\%THE_PATH_CCS%

rem  ---------------------------------------------------------------------------
rem  Set title of command window
rem  ---------------------------------------------------------------------------
title %THE_PROJECT%

rem  ---------------------------------------------------------------------------
rem  Set the PATH for current session
rem  ---------------------------------------------------------------------------
set PATH=C:\WINNT\system32;C:\WINNT

set PATH=%PATH%;%THE_LOCATION_PERL%\bin

set PATH=%PATH%;%THE_LOCATION_CCS%\bin
set PATH=%PATH%;%THE_LOCATION_CCS%\cc\bin
set PATH=%PATH%;%THE_LOCATION_CCS%\c5500\cgtools\bin
set PATH=%PATH%;%THE_LOCATION_CCS%\bin\utilities\tconf


set DSPLINK=C:\CCStudio_v3.1\boards\osk5912\dsplink

rem  ---------------------------------------------------------------------------
rem  Unset local variables
rem  ---------------------------------------------------------------------------
set THE_PROJECT=

set THE_PATH_PERL=
set THE_PATH_CCS=

set THE_LOCATION_PERL=
set THE_LOCATION_CCS=

rem  ---------------------------------------------------------------------------
rem  DONE
rem  ---------------------------------------------------------------------------
echo DSP/BIOS(TM) LINK
echo The environment for DSP side development is now set.
echo on


