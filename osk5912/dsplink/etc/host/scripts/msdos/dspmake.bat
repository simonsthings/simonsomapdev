@echo off
rem ============================================================================
rem @file   dspmake.bat
rem
rem @path   $(DSPLINK)/etc/host/scripts/msdos
rem
rem @desc   Builds the dsp side testsuite.
rem
rem @ver    00.08
rem ============================================================================
rem Copyright (c) Texas Instruments Incorporated 2002-2004
rem
rem Use of this software is controlled by the terms and conditions found in the
rem license agreement under which this software has been supplied or provided.
rem ============================================================================

pushd .

rem ============================================================================
rem Check for command line argument(s)
rem ============================================================================

if "%1"==""   goto help
if "%1"=="/h" goto help
if "%1"=="/H" goto help
if "%1"=="/?" goto help

if "%1"=="omap"  set DSP_PLATFORM=OMAP
if "%1"=="OMAP"  set DSP_PLATFORM=OMAP
if "%1"=="dm642" set DSP_PLATFORM=DM642
if "%1"=="DM642" set DSP_PLATFORM=DM642

if "%2"=="debug" set DSP_BUILD=debug
if "%2"=="Debug" set DSP_BUILD=debug

if "%2"=="release" set DSP_BUILD=release
if "%2"=="Release" set DSP_BUILD=release

set TIMAKE=C:\CCStudio_v3.1\cc\bin\timake

rem ============================================================================
rem Build dsplink library on which the testsuite depends.
rem ============================================================================
@echo on
:debug
pushd %DSPLINK%\dsp\make\%DSP_PLATFORM%
%TIMAKE% dsplink.pjt %DSP_BUILD% -a
popd
@echo off

rem ============================================================================
rem Build dsplink messaging library on which the testsuite depends.
rem ============================================================================
@echo on
:debug
pushd %DSPLINK%\dsp\make\%DSP_PLATFORM%
%TIMAKE% dsplinkmsg.pjt %DSP_BUILD% -a
popd
@echo off

rem ============================================================================
rem Build the loop sample application.
rem ============================================================================
@echo on
pushd %DSPLINK%\dsp\src\samples\loop\%DSP_PLATFORM%
%TIMAKE% loop.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% loop_bios_text.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% loop_bios.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% loop_text.pjt %DSP_BUILD% -a
popd
@echo off
rem ============================================================================
rem Build the message sample application.
rem ============================================================================
@echo on
pushd %DSPLINK%\dsp\src\samples\message\%DSP_PLATFORM%
%TIMAKE% message.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% message_bios_text.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% message_bios.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% message_text.pjt %DSP_BUILD% -a
popd
@echo off
rem ============================================================================
rem Build the scale sample application.
rem ============================================================================
@echo on
pushd %DSPLINK%\dsp\src\samples\scale\%DSP_PLATFORM%
%TIMAKE% scale.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% scale_bios_text.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% scale_bios.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% scale_text.pjt %DSP_BUILD% -a
popd
@echo off
rem ============================================================================
rem Build all the dsp side test projects.
rem ============================================================================
@echo on
pushd %DSPLINK%\dsp\src\test\endofstream\%DSP_PLATFORM%
%TIMAKE% endofstream.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% endofstream_bios_text.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% endofstream_bios.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% endofstream_text.pjt %DSP_BUILD% -a
popd

pushd %DSPLINK%\dsp\src\test\multichnlstress\%DSP_PLATFORM%
%TIMAKE% multichnlstress.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% multichnlstress_bios_text.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% multichnlstress_bios.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% multichnlstress_text.pjt %DSP_BUILD% -a
popd

pushd %DSPLINK%\dsp\src\test\multitaskchnlstress\%DSP_PLATFORM%
%TIMAKE% multitaskchnlstress.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% multitaskchnlstress_bios_text.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% multitaskchnlstress_bios.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% multitaskchnlstress_text.pjt %DSP_BUILD% -a
popd

pushd %DSPLINK%\dsp\src\test\receivebuf\%DSP_PLATFORM%
%TIMAKE% receivebuf.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% receivebuf_bios_text.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% receivebuf_bios.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% receivebuf_text.pjt %DSP_BUILD% -a
popd

pushd %DSPLINK%\dsp\src\test\receivesendbuf\%DSP_PLATFORM%
%TIMAKE% receivesendbuf.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% receivesendbuf_bios_text.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% receivesendbuf_bios.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% receivesendbuf_text.pjt %DSP_BUILD% -a
popd

pushd %DSPLINK%\dsp\src\test\sendbuf\%DSP_PLATFORM%
%TIMAKE% sendbuf.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% sendbuf_bios_text.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% sendbuf_bios.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% sendbuf_text.pjt %DSP_BUILD% -a
popd

pushd %DSPLINK%\dsp\src\test\idletest\%DSP_PLATFORM%
%TIMAKE% idletest.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% idletest_bios_text.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% idletest_bios.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% idletest_text.pjt %DSP_BUILD% -a
popd

pushd %DSPLINK%\dsp\src\test\flushtest\%DSP_PLATFORM%
%TIMAKE% flushtest.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% flushtest_bios_text.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% flushtest_bios.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% flushtest_text.pjt %DSP_BUILD% -a
popd

pushd %DSPLINK%\dsp\src\test\siodeletetest\%DSP_PLATFORM%
%TIMAKE% siodeletetest.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% siodeletetest_bios_text.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% siodeletetest_bios.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% siodeletetest_text.pjt %DSP_BUILD% -a
popd

pushd %DSPLINK%\dsp\src\test\reclaimtimeout\%DSP_PLATFORM%
%TIMAKE% reclaimtimeout.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% reclaimtimeout_bios_text.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% reclaimtimeout_bios.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% reclaimtimeout_text.pjt %DSP_BUILD% -a
popd

pushd %DSPLINK%\dsp\src\test\EchoMsg\%DSP_PLATFORM%
%TIMAKE% EchoMsg.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% EchoMsg_text.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% EchoMsg_bios.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% EchoMsg_bios_text.pjt %DSP_BUILD% -a
popd

pushd %DSPLINK%\dsp\src\test\LocateTimeoutMsg\%DSP_PLATFORM%
%TIMAKE% LocateTimeoutMsg.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% LocateTimeoutMsg_bios_text.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% LocateTimeoutMsg_bios.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% LocateTimeoutMsg_text.pjt %DSP_BUILD% -a
popd

pushd %DSPLINK%\dsp\src\test\ReceiveMsg\%DSP_PLATFORM%
%TIMAKE% ReceiveMsg.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% ReceiveMsg_bios_text.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% ReceiveMsg_bios.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% ReceiveMsg_text.pjt %DSP_BUILD% -a
popd

pushd %DSPLINK%\dsp\src\test\SendMsg\%DSP_PLATFORM%
%TIMAKE% SendMsg.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% SendMsg_bios_text.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% SendMsg_bios.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% SendMsg_text.pjt %DSP_BUILD% -a
popd

pushd %DSPLINK%\dsp\src\test\SupplierMsg\%DSP_PLATFORM%
%TIMAKE% SupplierMsg.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% SupplierMsg_bios_text.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% SupplierMsg_bios.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% SupplierMsg_text.pjt %DSP_BUILD% -a
popd
@echo off

pushd %DSPLINK%\dsp\src\test\getputmsg\%DSP_PLATFORM%
%TIMAKE% getputmsg.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% getputmsg_bios_text.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% getputmsg_bios.pjt %DSP_BUILD% -a
if %DSP_PLATFORM%==OMAP %TIMAKE% getputmsg_text.pjt %DSP_BUILD% -a
popd

goto done

:help
rem ----------------------------------------------------------------------------
rem HELP
rem ----------------------------------------------------------------------------
@echo.
@echo USAGE : dspmake [OMAP/DM642] [Debug/Release]
@echo.

:done

popd
rem ----------------------------------------------------------------------------
rem Unset environent variables.
rem ----------------------------------------------------------------------------
set TIMAKE=
set DSP_PLATFORM=
set DSP_BUILD=

@echo on
