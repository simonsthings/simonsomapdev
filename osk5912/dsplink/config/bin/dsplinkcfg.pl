#!/usr/bin/perl
#   ============================================================================
#   @file   dsplinkcfg.pl
#
#   @path   $(DSPLINK)\config\bin
#
#   @desc   Creates configuration file for DSP/BIOS LINK.
#
#   @ver    01.10.01
#   ============================================================================
#   Copyright (c) Texas Instruments Incorporated 2002-2004
#
#   Use of this software is controlled by the terms and conditions found in the
#   license agreement under which this software has been supplied or provided.
#   ============================================================================


use strict ;

#   ****************************************************************************
#   * USAGE :                                                                  *
#   *                                                                          *
#   * dsplinkcfg.pl                                                            *
#   ****************************************************************************

#   ============================================================================
#   Define constants
#   ============================================================================

my $OS_WINDOWS      = 'WINDOWS' ;
my $OS_LINUX        = 'LINUX' ;

my $TRUE            = 1 ;
my $FALSE           = 0 ;

my $CFGFILE         = "CURRENTCFG.MK" ;

my $MENU_ROOTDIR    = "ROOT-DIR" ;
my $MENU_GPPOS      = "GPPOS" ;
my $MENU_PLATFORM   = "PLATFORM" ;
my $MENU_VARIANT    = "VARIANT" ;
my $MENU_BUILDCFG   = "BUILD-CFG" ;
my $MENU_TRACE      = "TRACE" ;
my $MENU_PROFILE    = "PROFILE" ;

my $DIRSEP_WINDOWS  = "\\" ;
my $DIRSEP_LINUX    = "/" ;

#   ============================================================================
#   Import the configuration options for DSP/BIOS LINK.
#   ============================================================================

require ('dsplinkcfg.opt') ;            # Configuration options

#   ============================================================================
#   Global Variables
#   ============================================================================
my $TheOS           = "" ;

my $Var_ChgRootDir  = 0  ;

my $Var_Date        = 0  ;

my $Var_RootDir     = "" ;
my $Var_GppOS       = "" ;
my $Var_Distrib     = "" ;
my $Var_Platform    = "" ;
my $Var_Variant     = "" ;
my $Var_BuildCfg    = "" ;
my $Var_Trace       = 0  ;
my $Var_Profile     = 0  ;

my $Var_UseProc     = 0  ;
my $Var_UseChnl     = 0  ;
my $Var_UseMsgq     = 0  ;
my $Var_UseDsp      = 0  ;

my $Arr_Variant     = \@DsplinkCfgOpt::OPT_VARIANT_NONE ;

#   ============================================================================
#   Error strings
#   ============================================================================
my $ERR_OS      = "\n  !! ERROR !! Could not identify the native OS!\n" ;
my $ERR_ROOT    = "\n  !! ERROR !! Environment variable DSPLINK is not defined!\n" ;
my $ERR_PATH    = "\n  !! ERROR !! Invalid path assigned to DSPLINK!\n" ;
my $ERR_DIRS    = "\n  Could not find following directories:\n" ;


#   ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#   START PROCESSING
#   ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#   ============================================================================
#   Identify the native operating system
#   ============================================================================
if    ($ENV {'COMSPEC'} ne "")
{
    $TheOS = $OS_WINDOWS ;
}
elsif ($ENV {'SHELL'} ne "")
{
    $TheOS = $OS_LINUX ;
}
else
{
    die $ERR_OS ;
}

#   ============================================================================
#   Check for environment variable DSPLINK
#   ============================================================================
if ($ENV {'DSPLINK'} eq "")
{
    die $ERR_ROOT ;
}

#   ============================================================================
#   Get current date & time
#   ============================================================================
$Var_Date = &GetDateString () ;

#   ============================================================================
#   Get user inputs
#   ============================================================================
&ShowMenu ($MENU_ROOTDIR) ;

if ($Var_ChgRootDir == 1)
{
    &ShowAbort () ;
}
else
{
    &ShowMenu ($MENU_GPPOS) ;
    &ShowMenu ($MENU_PLATFORM) ;
    &ShowMenu ($MENU_VARIANT) ;
    &ShowMenu ($MENU_BUILDCFG) ;
    &ShowMenu ($MENU_TRACE) ;
    &ShowMenu ($MENU_PROFILE) ;

    #   ------------------------------------------------------------------------
    #   Generate command file/ shell script to set variables
    #   ------------------------------------------------------------------------
    &WriteCurrentConfig () ;

    &ShowComplete () ;
}


#   ============================================================================
#   Done
#   ============================================================================
exit (0) ;


#   ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#   SUBROUTINES
#   ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#   ============================================================================
#   func    ShowMenu
#
#   desc    Show the menu
#   ============================================================================
sub ShowMenu
{
    my ($menu)  = @_ ;

    my ($done)  = $FALSE  ;
    my ($text)  = "" ;

    do
    {
        &ClearScreen () ;

        &MenuHeader  () ;

        if    ($menu eq $MENU_ROOTDIR)
        {
            &MenuRootDir () ;
        }
        elsif ($menu eq $MENU_GPPOS)
        {
            &MenuGppOS () ;
        }
        elsif ($menu eq $MENU_PLATFORM)
        {
            &MenuPlatform () ;
        }
        elsif ($menu eq $MENU_VARIANT)
        {
            &MenuVariant () ;
        }
        elsif ($menu eq $MENU_BUILDCFG)
        {
            &MenuBuildCfg () ;
        }
        elsif ($menu eq $MENU_TRACE)
        {
            &MenuTrace () ;
        }
        elsif ($menu eq $MENU_PROFILE)
        {
            &MenuProfile () ;
        }

        &MenuFooter  () ;

        $text = &ReadInput  () ;

        $done = &IsValid ($menu, $text) ;
    }
    while ($done == $FALSE) ;
}


#   ============================================================================
#   func    ShowComplete
#
#   desc    Show the completion message
#   ============================================================================
sub ShowComplete
{
    my ($text)  = "" ;

    &ClearScreen () ;

    &MenuHeader  () ;

    $text .= "\n" ;
    $text .= "  Configuration complete.\n" ;
    $text .= "\n" ;
    $text .= "  See: " . &GetCfgFile () . "\n" ;
    $text .= "\n" ;
    $text .= "  ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n" ;

    print $text ;
}


#   ============================================================================
#   func    ShowAbort
#
#   desc    Show the abort message
#   ============================================================================
sub ShowAbort
{
    my ($text)  = "" ;

    &ClearScreen () ;

    &MenuHeader  () ;

    $text .= "\n" ;
    $text .= "  Configuration was aborted.\n" ;
    $text .= "\n" ;
    $text .= "  ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n" ;

    print $text ;
}


#   ============================================================================
#   func    IsValid
#
#   desc    Check if the input provided by the user is valid for specific menu
#   ============================================================================
sub IsValid
{
    my ($menu)  = shift (@_) ;
    my ($text)  = @_ ;

    my ($valid) = $FALSE ;

    #   ------------------------------------------------------------------------
    #   Remove extra spaces before and after the text entered on command-line.
    #   ------------------------------------------------------------------------
    $text  =~ s/^\s*(.*?)\s*$/$1/ ;

    #   ------------------------------------------------------------------------
    #   Menu : Root directory
    #   ------------------------------------------------------------------------
    if   ($menu eq $MENU_ROOTDIR)
    {
        if    (   ($text =~ m/\d/)
               && ($text >  0)
               && ($text <= 2))
        {
            $Var_RootDir = $ENV {'DSPLINK'} ;

            if ($text == 2)
            {
                $Var_ChgRootDir = 1 ;
            }

            $valid = $TRUE ;
        }
    }
    #   ------------------------------------------------------------------------
    #   Menu : Host OS
    #   ------------------------------------------------------------------------
    elsif ($menu eq $MENU_GPPOS)
    {
        if    (   ($text =~ m/\d/)
               && ($text >  0)
               && ($text <= scalar (@DsplinkCfgOpt::OPT_GPPOS)))
        {
            $valid = $TRUE ;

            $Var_GppOS   = $DsplinkCfgOpt::OPT_GPPOS [$text][0] ;
            $Var_Distrib = $DsplinkCfgOpt::OPT_GPPOS [$text][1] ;
        }
    }
    #   ------------------------------------------------------------------------
    #   Menu : PLATFORM
    #   ------------------------------------------------------------------------
    elsif ($menu eq $MENU_PLATFORM)
    {
        if    (   ($text =~ m/\d/)
               && ($text >  0)
               && ($text <= scalar (@DsplinkCfgOpt::OPT_PLATFORM)))
        {
            $valid = $TRUE ;

            $Var_Platform = $DsplinkCfgOpt::OPT_PLATFORM [$text][0] ;
            $Arr_Variant  = $DsplinkCfgOpt::OPT_PLATFORM [$text][2] ;
        }
    }
    #   ------------------------------------------------------------------------
    #   Menu : VARIANT
    #   ------------------------------------------------------------------------
    elsif ($menu eq $MENU_VARIANT)
    {
        if    (   ($text eq "")
               && (scalar (@$Arr_Variant) == 0))
        {
            $valid = $TRUE ;
        }
        elsif (   ($text =~ m/\d/)
               && ($text >  0)
               && ($text <= scalar (@$Arr_Variant)))
        {
            $Var_Variant = $$Arr_Variant [$text][0] ;

            $valid = $TRUE ;
        }
    }
    #   ------------------------------------------------------------------------
    #   Menu : BUILD CONFIGURATION
    #   ------------------------------------------------------------------------
    elsif ($menu eq $MENU_BUILDCFG)
    {
        if    (   ($text =~ m/\d/)
               && ($text >  0)
               && ($text <= 4))
        {
            if ($text == 1)
            {
                $Var_UseProc     = 1  ;
                $Var_UseChnl     = 1  ;
                $Var_UseMsgq     = 1  ;
                $Var_UseDsp      = 1  ;
            }
            elsif ($text == 2)
            {
                $Var_UseProc     = 1  ;
                $Var_UseChnl     = 1  ;
                $Var_UseDsp      = 1  ;
            }
            elsif ($text == 3)
            {
                $Var_UseProc     = 1  ;
                $Var_UseDsp      = 1  ;
            }
            else
            {
                $Var_UseDsp      = 1  ;
            }

            $valid = $TRUE ;
        }
    }
    #   ------------------------------------------------------------------------
    #   Menu : TRACE
    #   ------------------------------------------------------------------------
    elsif ($menu eq $MENU_TRACE)
    {
        if    (($text =~ m/\d/) && ($text <= 1))
        {
            $Var_Trace = $text ;

            $valid = $TRUE ;
        }
    }
    #   ------------------------------------------------------------------------
    #   Menu : PROFILE LEVELS
    #   ------------------------------------------------------------------------
    elsif ($menu eq $MENU_PROFILE)
    {
        if    (($text =~ m/\d/) && ($text <= 2))
        {
            $Var_Profile  = $text ;

            $valid = $TRUE ;
        }
    }

    return $valid ;
}


#   ============================================================================
#   func    WriteCurrentConfig
#
#   desc    Write the current build configuration
#   ============================================================================
sub WriteCurrentConfig
{
    my ($cfgfile)   = &GetCfgFile () ;
    my ($text)      = "" ;

    my ($filehandle) ;

    open ($filehandle, ">$cfgfile") || die "!! Couldn't open file: $cfgfile\n" ;

    $text .= "#   =========================================================\n" ;
    $text .= "#   DSP/BIOS LINK Configuration file.\n" ;
    $text .= "#\n" ;
    $text .= "#   CAUTION! This is a generated file.\n" ;
    $text .= "#            All changes will be lost.\n" ;
    $text .= "#\n" ;
    $text .= "#   This file was generated on " . $Var_Date . "\n" ;
    $text .= "#   =========================================================\n" ;
    $text .= "\n" ;
    $text .= "\n" ;

    $text .= "#   =========================================================\n" ;
    $text .= "#   When this file was created.\n" ;
    $text .= "#   =========================================================\n" ;
    $text .= "export  CFGDATE       := " . $Var_Date . "\n" ;
    $text .= "\n" ;
    $text .= "\n" ;

    $text .= "#   =========================================================\n" ;
    $text .= "#   DSP/BIOS LINK GPP side root directory.\n" ;
    $text .= "#   =========================================================\n" ;
    $text .= "export GPPROOT        := " . &GetRootDir ("gpp") . "\n" ;
    $text .= "export DSPROOT        := " . &GetRootDir ("dsp") . "\n" ;
    $text .= "\n" ;
    $text .= "\n" ;

    $text .= "#   =========================================================\n" ;
    $text .= "#   GPP OS for which DSP/BIOS LINK is being built\n" ;
    $text .= "#   =========================================================\n" ;
    $text .= "export GPPOS          := " . $Var_GppOS   . "\n" ;
    $text .= "\n" ;
    $text .= "\n" ;

    $text .= "#   =========================================================\n" ;
    $text .= "#   Specific distribution of GPP OS (if any)\n" ;
    $text .= "#   =========================================================\n" ;
    $text .= "export DISTRIBUTION   := " . $Var_Distrib . "\n" ;
    $text .= "\n" ;
    $text .= "\n" ;

    $text .= "#   =========================================================\n" ;
    $text .= "#   Target platform for DSP/BIOS LINK\n" ;
    $text .= "#   =========================================================\n" ;
    $text .= "export PLATFORM       := " . $Var_Platform . "\n" ;
    $text .= "\n" ;
    $text .= "\n" ;

    $text .= "#   =========================================================\n" ;
    $text .= "#   Variant of the target platform\n" ;
    $text .= "#   =========================================================\n" ;
    $text .= "export VARIANT        := " . $Var_Variant . "\n" ;
    $text .= "\n" ;
    $text .= "\n" ;

    $text .= "#   =========================================================\n" ;
    $text .= "#   Compile time scalability options for DSP/BIOS LINK\n" ;
    $text .= "#   =========================================================\n" ;
    $text .= "export USE_PROC       := " . $Var_UseProc . "\n" ;
    $text .= "export USE_CHNL       := " . $Var_UseChnl . "\n" ;
    $text .= "export USE_MSGQ       := " . $Var_UseMsgq . "\n" ;
    $text .= "export USE_DSP        := " . $Var_UseDsp  . "\n" ;
    $text .= "\n" ;
    $text .= "\n" ;

    $text .= "#   =========================================================\n" ;
    $text .= "#   Enable/ Disable trace\n" ;
    $text .= "#   =========================================================\n" ;
    $text .= "export TRACE          := " . $Var_Trace . "\n" ;
    $text .= "\n" ;
    $text .= "\n" ;

    $text .= "#   =========================================================\n" ;
    $text .= "#   Enable/ Disable profiling\n" ;
    $text .= "#   =========================================================\n" ;
    $text .= "export PROFILE        := " . $Var_Profile  . "\n" ;

    print $filehandle $text ;

    close ($filehandle) ;
}


#   ============================================================================
#   func    MenuRootDir
#
#   desc    Show menu : RootDir
#   ============================================================================
sub MenuRootDir
{
    my ($str) = "" ;
    my ($err) = "" ;

    my $dir = $ENV {'DSPLINK'} ;

    $str .= "  DSPLINK is currently defined as:\n" ;
    $str .= "\n" ;
    $str .= "  " . $dir . "\n" ;
    $str .= "\n" ;

    #   ------------------------------------------------------------------------
    #   Check if the directory referred by environment variable DSPLINK
    #   actually exists.
    #   ------------------------------------------------------------------------
    if (-d $dir)
    {
        my @subdirs = () ;

        #   --------------------------------------------------------------------
        #   Check if ncessary sub-directories actually exist.
        #   --------------------------------------------------------------------
        if ($TheOS eq $OS_WINDOWS)
        {
            @subdirs = (
                        "\\config",
                        "\\config\\all",
                        "\\config\\bin"
                       ) ;
        }
        else
        {
            @subdirs = (
                        "/config",
                        "/config/all",
                        "/config/bin"
                       ) ;
        }

        foreach my $subdir (@subdirs)
        {
            my ($path) = $dir . $subdir ;

            if (!(-d $path))
            {
                $err .= "  $path\n" ;
            }
        }
    }
    else
    {
        $err .= "  $dir\n" ;
    }

    if ($err ne "")
    {
        $str .= $ERR_PATH ;
        $str .= $ERR_DIRS ;
        $str .= $err ;
        $str .= "\n" ;
    }

    $str .= "\n" ;
    $str .= "  1.   Continue.\n" ;
    $str .= "\n" ;
    $str .= "  2.   Quit to change.\n" ;
    $str .= "\n" ;

    print $str ;
}


#   ============================================================================
#   func    MenuGppOS
#
#   desc    Show menu : GPP OS
#   ============================================================================
sub MenuGppOS
{
    my ($str) = "" ;

    $str .= "  Choose the OS running on GPP\n" ;
    $str .= "\n" ;

    for (my $i = 1 ; $i < scalar (@DsplinkCfgOpt::OPT_GPPOS) ; $i++)
    {
        $str .= sprintf "  %d.   %s\n", $i, $DsplinkCfgOpt::OPT_GPPOS [$i][2] ;
        $str .= "\n" ;
    }

    print $str ;
}


#   ============================================================================
#   func    MenuPlatform
#
#   desc    Show menu : Platform
#   ============================================================================
sub MenuPlatform
{
    my ($str) = "" ;

    $str .= "  Choose the target platform\n" ;
    $str .= "\n" ;

    for (my $i = 1 ; $i < scalar (@DsplinkCfgOpt::OPT_PLATFORM) ; $i++)
    {
        $str .= sprintf "  %d.   %s\n", $i, $DsplinkCfgOpt::OPT_PLATFORM [$i][1] ;
        $str .= "\n" ;
    }

    print $str ;
}


#   ============================================================================
#   func    MenuVariant
#
#   desc    Show menu : Variant
#   ============================================================================
sub MenuVariant
{
    my ($str) = "" ;

    if (scalar (@$Arr_Variant) == 0)
    {
        $str .= "  No variant is supported for this platform.\n" ;
        $str .= "\n" ;
        $str .= "  Press <ENTER> to continue...\n" ;
        $str .= "\n" ;
    }
    else
    {
        $str .= "  Choose the variant of the platform\n" ;
        $str .= "\n" ;

        for (my $i = 1 ; $i < scalar (@$Arr_Variant) ; $i++)
        {
            $str .= sprintf "  %d.   %s\n", $i, $$Arr_Variant [$i][1] ;
            $str .= "\n" ;
        }
    }

    print $str ;
}


#   ============================================================================
#   func    MenuBuildCfg
#
#   desc    Show menu : Build Configuration
#   ============================================================================
sub MenuBuildCfg
{
    my ($str) = "" ;

    $str .= "  Choose the target platform\n" ;
    $str .= "\n" ;

    for (my $i = 1 ; $i < scalar (@DsplinkCfgOpt::OPT_BUILDCFG) ; $i++)
    {
        $str .= sprintf "  %d.   %s\n", $i, $DsplinkCfgOpt::OPT_BUILDCFG [$i] ;
        $str .= "\n" ;
    }

    print $str ;
}


#   ============================================================================
#   func    MenuTrace
#
#   desc    Show menu : Trace options
#   ============================================================================
sub MenuTrace
{
    my ($str) = "" ;

    $str .= "  Enable debug trace?\n" ;
    $str .= "\n" ;
    $str .= "  0.   No\n" ;
    $str .= "\n" ;
    $str .= "  1.   Yes\n" ;
    $str .= "\n" ;

    print $str ;
}


#   ============================================================================
#   func    MenuProfile
#
#   desc    Show menu : Profile options
#   ============================================================================
sub MenuProfile
{
    my ($str) = "" ;

    $str .= "  Enable profiling?\n" ;
    $str .= "\n" ;
    $str .= "  0.   No\n" ;
    $str .= "\n" ;
    $str .= "  1.   Yes. Basic only.\n" ;
    $str .= "\n" ;
    $str .= "  2.   Yes. Detailed.\n" ;
    $str .= "\n" ;

    print $str ;
}


#   ============================================================================
#   func    MenuHeader
#
#   desc    Show header of the menu
#   ============================================================================
sub MenuHeader
{
    my ($str) = "" ;

    $str .= "\n" ;
    $str .= "  ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n" ;
    $str .= "             DSP/BIOS(TM) LINK  Configuration Tool\n" ;
    $str .= "  ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n" ;
    $str .= "\n" ;

    print $str ;
}


#   ============================================================================
#   func    MenuFooter
#
#   desc    Show footer of the menu
#   ============================================================================
sub MenuFooter
{
    my ($str) = "" ;

    $str .= "  ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n" ;
    $str .= "\n" ;
    $str .= "  YOUR CHOICE : " ;

    print $str ;
}


#   ============================================================================
#   func    ReadInput
#
#   desc    Reads the user inout from standard input
#   ============================================================================
sub ReadInput
{
    my ($str) ;

    $str = <STDIN> ;

    chomp $str ;

    return $str ;
}


#   ============================================================================
#   func    GetDateString
#
#   desc    Returns the date string
#   ============================================================================
sub GetDateString
{
    my (@date)      = (localtime)[0..5] ;

    my (@month)     = ("JAN", "FEB", "MAR", "APR", "MAY", "JUN",
                       "JUL", "AUG", "SEP", "OCT", "NOV", "DEC") ;

    my ($datestr)   = $month [$date [4]] . " "  .
                      $date [3]          . ", " .
                      ($date [5] + 1900) . "  " .
                      $date [2]          . ":"  .
                      $date [1]          . ":"  ;

    if ($date [0] < 10)
    {
        $datestr .= "0" ;
    }

    $datestr .= $date [0] ;

    return $datestr ;
}


#   ============================================================================
#   func    GetCfgFile
#
#   desc    Returns the full path to configuration file
#   ============================================================================
sub GetCfgFile
{
    my ($str) ;

    if    ($TheOS eq $OS_WINDOWS)
    {
        $str = $Var_RootDir . "\\config\\" . $CFGFILE ;
    }
    else
    {
        $str = $Var_RootDir . "/config/" . $CFGFILE ;
    }

    return $str ;
}


#   ============================================================================
#   func    GetRootDir
#
#   desc    Returns the full path to specified root directory
#   ============================================================================
sub GetRootDir
{
    my ($dir) = @_ ;
    my ($str) ;

    if    ($TheOS eq $OS_WINDOWS)
    {
        $str = $Var_RootDir . "\\" . $dir ;
    }
    else
    {
        $str = $Var_RootDir . "/" . $dir ;
    }

    return $str ;
}


#   ============================================================================
#   func    ClearScreen
#
#   desc    Clears the screen
#   ============================================================================
sub ClearScreen
{
    if    ($TheOS eq $OS_WINDOWS)
    {
        system ("cls") ;
    }
    else
    {
        system ("clear") ;
    }
}
