#!/usr/bin/perl
#   ============================================================================
#   @file   runif.pl
#
#   @path   $(DSPLINK)\gpp\make\bin
#
#   @desc   Perl script to conditionally execute a command.
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
#   cexec <condition> <command if true> <command if false>
#   ============================================================================

$debug = 0 ;                        # Set to 1 to print debug messages

#   ============================================================================
#   Define the TAGs used in configuration file
#   ============================================================================
my $TKN_EXIST    = 'exist'     ;

#  =============================================================================
#  Get the arguments
#  =============================================================================
if ($#ARGV < 0)
{
    die "USAGE : [PERL] cexec.pl <platform> <condition> <command if true> <command if false>\n" ;
}
my $str_cmdline   = '' ;
my $str_condition = '' ;
my $str_cmdtrue   = '' ;
my $str_cmdfalse  = '' ;
my $cmdtoexecute  = 'echo Invalid arguments' ;

# - how to get the full command line
$str_condition = $ARGV [1] ;
$str_cmdtrue   = $ARGV [2] ;
$str_cmdfalse  = $ARGV [3] ;
#  =============================================================================
#  Parse the command
#  =============================================================================
#($str_condition, $str_cmdtrue, $str_cmdfalse) = split (/\s+/ , $str_cmdline) ;
if ($debug == 1)
{
    print "Condition    : $str_condition\n" ;
    print "Cmd If True  : $str_cmdtrue\n"   ;
    print "Cmd If False : $str_cmdfalse\n"  ;
}

#  =============================================================================
#  Check condition
#  =============================================================================
if ($str_condition =~ /^$TKN_EXIST\s+(.*)/)
{
    $filename = $1 ;
    if ( -e $filename )
    {
        $cmdtoexecute = $str_cmdtrue ;
    }
    else
    {
        $cmdtoexecute = $str_cmdfalse ;
    }
}

#  =============================================================================
#  Execute the conditional command
#  =============================================================================
if ($ARGV [0] eq "WINDOWS") {
    $retVal=system ($cmdtoexecute) ;
}
elsif ($ARGV [0] eq "LINUX") {
    $retVal=exec ($cmdtoexecute) ;
}

#  =============================================================================
#  DONE!!
#  =============================================================================
exit ($retVal) ;
