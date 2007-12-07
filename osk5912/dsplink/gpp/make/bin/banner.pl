#!/usr/bin/perl
#   ============================================================================
#   @file   banner.pl
#
#   @path   $(DSPLINK)\gpp\make\bin
#
#   @desc   Perl script to print formatted banners during execution of
#           make scripts.
#
#   @ver    01.10.01
#   ============================================================================
#   Copyright (c) Texas Instruments Incorporated 2002-2004
#
#   Use of this software is controlled by the terms and conditions found in the
#   license agreement under which this software has been supplied or provided.
#   ============================================================================


#   ============================================================================
#   USAGE:
#   banner.pl level component stage variant
#
#   level     = Level of print
#   component = Name of the component
#   stage     = Stage of build process [DIRS/INCLUDE/OBJECTS/...]
#   variant   = Build variant [DEBUG/RELEASE]
#
#   ============================================================================

$debug = 0 ;                        # Set to 1 to print debug messages

#  =============================================================================
#  Parse command line parameters
#  =============================================================================

$level      = $ARGV [0] ;   # Level
$component  = $ARGV [1] ;   # Module name
$stage      = $ARGV [2] ;   # Stage of build

if ($#ARGV >= 3)
{
    $variant = $ARGV [3] ;  # Build variant
}
else
{
    $variant = '' ;
}


if ($debug)
{
    print "level     : $level\n"  ;
    print "component : $component\n"  ;
    print "stage     : $stage\n"  ;
    print "variant   : $variant\n"  ;
}


#  =============================================================================
#  Print for style - BANNER
#  =============================================================================

$fmt_arg2   = "--------%-10s----------------------------------" ;
$fmt_arg3   = "--------%-10s--------------%-10s----------" ;

$fmt_l1     = "\n[%-9s] %s\n" ;
$fmt_l2     = "[%-9s] %s\n" ;

$outstr = '' ;
$outfmt = '' ;

if ($variant eq '')
{
    $outstr = sprintf "$fmt_arg2", $stage ;
}
else
{
    $outstr = sprintf "$fmt_arg3", $stage, $variant ;
}

$outstr =~ s/ /\-/g ;

$outstr =~ s/(\w)\-/$1 /g ;
$outstr =~ s/\-(\w)/ $1/g ;


if ($level == 1)
{
    $outstr =~ s/\-/\=/g ;

    $outfmt = $fmt_l1 ;
}
else
{
    $outfmt = $fmt_l2 ;
}

printf $outfmt, $component, $outstr ;


#  =============================================================================
#  DONE !!
#  =============================================================================
exit (0) ;
