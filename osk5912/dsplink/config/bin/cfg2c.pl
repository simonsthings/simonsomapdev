#   ============================================================================
#   @file   cfg2c.pl
#
#   @path   $(DSPLINK)\gpp\make\bin
#
#   @desc   Creates C source code corresponding to the configuration provided
#           in text format.
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
#   * cfg2c.pl [GPP|DSP] cfgfile cfile                                         *
#   ****************************************************************************

my $debug   = 0 ;

#   ============================================================================
#   Define constants for TRUE, FALSE
#   ============================================================================
my $TRUE    = 1 ;
my $FALSE   = 0 ;

#   ============================================================================
#   Define the tokens used in configuration file
#   ============================================================================

#   DRIVER SECTION -------------------------------------------------------------
my @Tokens_DRIVER       = (
                           ['NAME'          , 'GENERIC'],
                           ['COMPONENTS'    , 'GENERIC'],
                           ['QUEUE'         , 'GENERIC'],
                           ['LINKTABLES'    , 'GENERIC'],
                           ['MMUTABLES'     , 'GENERIC'],
                           ['NUMMQAS'       , 'MSGQ_COMPONENT'],
                           ['NUMMQTS'       , 'MSGQ_COMPONENT'],
                           ['LOCALMQT'      , 'MSGQ_COMPONENT']
                          ) ;

#   GPP SECTION ----------------------------------------------------------------
my @Tokens_GPP          = (
                           ['NAME'          , 'GENERIC'],
                           ['NUMDSPS'       , 'GENERIC']
                          ) ;

#   DSP SECTION ----------------------------------------------------------------
my @Tokens_DSP          = (
                           ['NAME'          , 'GENERIC'],
                           ['ARCHITECTURE'  , 'GENERIC'],
                           ['EXECUTABLE'    , 'GENERIC'],
                           ['LOADER'        , 'GENERIC'],
                           ['LINKTABLE'     , 'GENERIC'],
                           ['NUMLINKS'      , 'GENERIC'],
                           ['AUTOSTART'     , 'GENERIC'],
                           ['RESETVECTOR'   , 'GENERIC'],
                           ['WORDSIZE'      , 'GENERIC'],
                           ['ENDIAN'        , 'GENERIC'],
                           ['MMUFLAG'       , 'GENERIC'],
                           ['MMUTABLE'      , 'GENERIC'],
                           ['MMUENTRIES'    , 'GENERIC'],
                           ['INTERFACE'     , 'GENERIC'],
                           ['MQTID'         , 'MSGQ_COMPONENT']
                          ) ;

#   LINK SECTION ---------------------------------------------------------------
my @Tokens_LINK         = (
                           ['NAME'          , 'CHNL_COMPONENT'],
                           ['ABBR'          , 'CHNL_COMPONENT'],
                           ['NUMCHANNELS'   , 'CHNL_COMPONENT'],
                           ['BASECHANNELID' , 'CHNL_COMPONENT'],
                           ['MAXBUFSIZE'    , 'CHNL_COMPONENT'],
                           ['INTERFACE'     , 'CHNL_COMPONENT'],
                           ['ARGUMENT1'     , 'CHNL_COMPONENT'],
                           ['ARGUMENT2'     , 'CHNL_COMPONENT']
                          ) ;

#   MMU SECTION ----------------------------------------------------------------
my @Tokens_MMU          = (
                           ['ADDRVIRTUAL'   , 'GENERIC'],
                           ['ADDRPHYSICAL'  , 'GENERIC'],
                           ['SIZE'          , 'GENERIC'],
                           ['ACCESS'        , 'GENERIC'],
                           ['PRESERVE'      , 'GENERIC'],
                           ['MAPINGPP'      , 'GENERIC']
                          ) ;

#   MQA SECTION ----------------------------------------------------------------
my @Tokens_MQA          = (
                           ['NAME'          , 'MSGQ_COMPONENT'],
                           ['INTERFACE'     , 'MSGQ_COMPONENT']
                          ) ;

#   MQT SECTION ----------------------------------------------------------------
my @Tokens_MQT          = (
                           ['NAME'          , 'MSGQ_COMPONENT'],
                           ['INTERFACE'     , 'MSGQ_COMPONENT'],
                           ['LINKID'        , 'MSGQ_COMPONENT']
                          ) ;

#   ============================================================================
#   Define the TAGs used in configuration file
#   ============================================================================
my $tkn_driver_begin    = '\[DRIVER\]'   ;
my $tkn_driver_end      = '\[\/DRIVER\]' ;

my $tkn_gpp_begin       = '\[GPP\]'   ;
my $tkn_gpp_end         = '\[\/GPP\]' ;

my $tkn_dsp_begin       = '\[DSP'   ;
my $tkn_dsp_end         = '\[\/DSP' ;

my $tkn_link_begin      = '\[LINKTABLE' ;
my $tkn_link_end        = '\[\/LINKTABLE' ;

my $tkn_mmu_begin       = '\[MMUTABLE'   ;
my $tkn_mmu_end         = '\[\/MMUTABLE' ;

my $tkn_mqa_begin       = '\[MQA'   ;
my $tkn_mqa_end         = '\[\/MQA' ;

my $tkn_mqt_begin       = '\[MQT'   ;
my $tkn_mqt_end         = '\[\/MQT' ;


#   ============================================================================
#   Define the 'C' typedefs defined in the file "cfg.h" (GPP side)
#   ============================================================================
my $GPP_TYPE_DRIVER     = "CFG_Driver"      ;
my $GPP_TYPE_GPP        = "CFG_Gpp"         ;
my $GPP_TYPE_DSP        = "CFG_Dsp"         ;
my $GPP_TYPE_LINK       = "CFG_Link"        ;
my $GPP_TYPE_MMU        = "CFG_MmuEntry"    ;
my $GPP_TYPE_MQA        = "CFG_Mqa"    ;
my $GPP_TYPE_MQT        = "CFG_Mqt"    ;

#   ============================================================================
#   Define the 'C' variables defined in the file "cfg.h" (GPP side)
#   ============================================================================
my $GPP_VAR_DRIVER     = "CFG_DbDriver"     ;
my $GPP_VAR_GPP        = "CFG_DbGpp"        ;
my $GPP_VAR_DSP        = "CFG_DbDsp_"       ;
my $GPP_VAR_LINK       = "CFG_DbLinkTable_" ;
my $GPP_VAR_MMU        = "CFG_DbMmuTable_"  ;
my $GPP_VAR_MQA        = "CFG_DbMqa"  ;
my $GPP_VAR_MQT        = "CFG_DbMqt"  ;

#   ============================================================================
#   Define the context corresopnding to sections within configuration file
#   ============================================================================
my $SECT_NONE           = 0 ;
my $SECT_DRIVER         = 1 ;
my $SECT_GPP            = 2 ;
my $SECT_DSP            = 3 ;
my $SECT_LINK           = 4 ;
my $SECT_MMU            = 5 ;
my $SECT_MQA            = 6 ;
my $SECT_MQT            = 7 ;

my $section_context     = $SECT_NONE ;

#   ============================================================================
#   Define indent levels
#   ============================================================================
my $INDENT_LEVEL_1      = 1 ;
my $INDENT_LEVEL_2      = 2 ;
my $INDENT_LEVEL_3      = 3 ;

#   ============================================================================
#   Tokens defined in build configuration
#   ============================================================================
my $TKN_CFG_DATE        = 'export\s+CFGDATE'  ;

my $TKN_USE_DSP         = 'export\s+USE_DSP'  ;
my $TKN_USE_PROC        = 'export\s+USE_PROC' ;
my $TKN_USE_CHNL        = 'export\s+USE_CHNL' ;
my $TKN_USE_MSGQ        = 'export\s+USE_MSGQ' ;

my $TKN_GPPOS           = 'export\s+GPPOS'        ;
my $TKN_DISTRIBUTION    = 'export\s+DISTRIBUTION' ;
my $TKN_PLATFORM        = 'export\s+PLATFORM'     ;
my $TKN_VARIANT         = 'export\s+VARIANT'      ;

#   ============================================================================
#   Configuration option chosen.
#   ============================================================================
my $CFG_DATE            = "" ;

my $USE_DSP             = 0 ;
my $USE_PROC            = 0 ;
my $USE_CHNL            = 0 ;
my $USE_MSGQ            = 0 ;

my $VAL_GPPOS           = "" ;
my $VAL_DISTRIBUTION    = "" ;
my $VAL_PLATFORM        = "" ;
my $VAL_VARIANT         = "" ;

#   ============================================================================
#   Define the storage for different sections as they are read
#   ============================================================================
my @arr_driver          = () ;          # Simple array
my @arr_link            = () ;          # Simple array
my @arr_gpp             = () ;          # Simple array
my @arr_dsp             = () ;          # Array of arrays
my @arr_mmu             = () ;          # Array of arrays

my @arr_mqa             = () ;          # Array of arrays
my @arr_mqt             = () ;          # Array of arrays

my @lines               = () ;          # Array for lines a file

#   ============================================================================
#   Define storage for IDs attached to sections that can appear more than once
#   ============================================================================
my $id_dsp              = "" ;
my $id_link             = "" ;
my $id_mmu              = "" ;
my $id_mqa              = "" ;
my $id_mqt              = "" ;

#   ============================================================================
#   Define globals
#   ============================================================================

my $STR_CAUTION1        = "Caution ! This is a generated file." ;
my $STR_CAUTION2        = "It defines the run-time configuration." ;
my $STR_CAUTION3        = "Any change made in this file will be lost." ;

my $errstr              = "" ;
my $currentcfg          = "" ;

my $filehandle ;


#   ############################################################################
#   START PROCESSING
#   ############################################################################

#   ============================================================================
#   Parse & Validate command line parameters
#   ============================================================================

if (scalar (@ARGV) < 3)
{
    die "USAGE : [PERL] cfg2c.pl GPP|DSP cfgfile outfile\n" ;
}

my $target  =  uc ($ARGV [0]) ;
my $cfgfile =  $ARGV [1] ;
my $outfile =  $ARGV [2] ;

if (($target ne "GPP") && ($target ne "DSP"))
{
    $errstr = "!! ERROR: Invalid target specified.\n" ;
}

if (!(-f $cfgfile))
{
    $errstr .= "!! ERROR: Configuration file does not exist: $cfgfile\n"
}

die $errstr unless ($errstr eq "") ;


#   ============================================================================
#   Get the current configuration
#   ============================================================================
$currentcfg = &GetCurrentCfg () ;

if ($ENV {'DSPLINK'} eq "")
{
    $errstr .= "!! ERROR: Environment variable DSPLINK is not defined.\n"
}
elsif (!(-f $currentcfg))
{
    $errstr .= "!! ERROR: Configuration file does not exist: $currentcfg\n"
}

die $errstr unless ($errstr eq "") ;


open (CURRENTCFG, "$currentcfg") || die "!! Couldn't open $currentcfg.\n" ;

@lines = <CURRENTCFG> ;

close (CURRENTCFG) ;


foreach my $line (@lines)
{
    chomp ($line) ;

    unless (($line  =~ /^#/) || ($line eq ""))
    {
        if    ($line =~ /^$TKN_GPPOS\s+:=\s+(\w+)$/)
        {
            $VAL_GPPOS = $1 ;
        }
        elsif ($line =~ /^$TKN_DISTRIBUTION\s+:=\s+(\w+)$/)
        {
            $VAL_DISTRIBUTION = $1 ;
        }
        elsif ($line =~ /^$TKN_PLATFORM\s+:=\s+(\w+)$/)
        {
            $VAL_PLATFORM = $1 ;
        }
        elsif ($line =~ /^$TKN_VARIANT\s+:=\s+(\w+)$/)
        {
            $VAL_VARIANT = $1 ;
        }
        elsif ($line =~ /^$TKN_USE_DSP\s+:=\s+(\d+)$/)
        {
            $USE_DSP = $1 ;
        }
        elsif ($line =~ /^$TKN_USE_PROC\s+:=\s+(\d+)$/)
        {
            $USE_PROC = $1 ;
        }
        elsif ($line =~ /^$TKN_USE_CHNL\s+:=\s+(\d+)$/)
        {
            $USE_CHNL = $1 ;
        }
        elsif ($line =~ /^$TKN_USE_MSGQ\s+:=\s+(\d+)$/)
        {
            $USE_MSGQ = $1 ;
        }
        elsif ($line =~ /^$TKN_CFG_DATE\s+:=\s+(.*)$/)
        {
            $CFG_DATE = $1 ;
        }
    }
}


&ShowConfiguration () ;

#   ============================================================================
#   Read the configuration file.
#   ============================================================================
&Debug_1 ("Configuration file [%s]\n\n", $cfgfile) ;

open (CFGFILE, "$cfgfile") || die "!! Couldn't open $cfgfile for input.\n" ;

@lines = <CFGFILE> ;

close (CFGFILE) ;


#   ============================================================================
#   Create output file and add required header files.
#   ============================================================================
&Debug_1 ("Generating C file [%s]\n\n", $outfile) ;

open ($filehandle, ">$outfile") || die "!! Couldn't open file: $outfile\n" ;


#   ============================================================================
#   Write the file header.
#   ===========================================================================
&WriteFileHeader () ;


#   ============================================================================
#   Populate arrays corresponding to each section.
#   Print corresponding code at end of each section.
#   ===========================================================================
foreach my $line (@lines)
{
    chomp ($line) ;

    #   ------------------------------------------------------------------------
    #   Remove leading and trailing spaces
    #   ------------------------------------------------------------------------
    $line =~ s/^\s*(.*?)\s*$/$1/ ;

    #   ------------------------------------------------------------------------
    #   Ignore comments & empty lines
    #   ------------------------------------------------------------------------
    if (($line  =~ /^#/) || ($line eq ""))
    {
    }
    elsif ($section_context == $SECT_NONE)
    {
        #   --------------------------------------------------------------------
        #   Beginning of a new section
        #   --------------------------------------------------------------------
        if    ($line  =~ /^$tkn_driver_begin/)                  # [DRIVER]
        {
            $section_context = $SECT_DRIVER ;
        }
        elsif ($line  =~ /^$tkn_gpp_begin/)                     # [GPP]
        {
            $section_context = $SECT_GPP ;
        }
        elsif ($line  =~ /^$tkn_dsp_begin(\d+)/)                # [DSP]
        {
            $section_context = $SECT_DSP ;

            $id_dsp = &GetTagId ($1) ;
        }
        elsif ($line  =~ /^$tkn_link_begin(\d+)/)               # [LINK]
        {
            $section_context = $SECT_LINK ;

            $id_link = &GetTagId ($1) ;
        }
        elsif ($line  =~ /^$tkn_mmu_begin(\d+)/)                # [MMU]
        {
            $section_context = $SECT_MMU ;

            $id_mmu = &GetTagId ($1) ;
        }
        elsif ($line  =~ /^$tkn_mqa_begin(\d+)/)                # [MQA]
        {
            $section_context = $SECT_MQA ;

            $id_mqa = &GetTagId ($1) ;
        }
        elsif ($line  =~ /^$tkn_mqt_begin(\d+)/)                # [MQT]
        {
            $section_context = $SECT_MQT ;

            $id_mqt = &GetTagId ($1) ;
        }
    }
    else
    {
        #   --------------------------------------------------------------------
        #   Is it end of a section?
        #   Write section information to selected file.
        #   --------------------------------------------------------------------

        if    ($line  =~ /^$tkn_driver_end/)
        {
            &WriteSectDriver () ;

            @arr_driver     = () ;
            $section_context   = $SECT_NONE ;
        }
        elsif ($line  =~ /^$tkn_gpp_end/)
        {
            &WriteSectGpp () ;

            @arr_gpp        = () ;
            $section_context   = $SECT_NONE ;
        }
        elsif ($line  =~ /^$tkn_dsp_end/)
        {
            &WriteSectDsp () ;

            @arr_dsp        = () ;
            $section_context   = $SECT_NONE ;
        }
        elsif ($line  =~ /^$tkn_link_end/)
        {
            &WriteSectLink () ;

            @arr_link       = () ;
            $section_context   = $SECT_NONE ;
        }
        elsif ($line  =~ /^$tkn_mmu_end/)
        {
            &WriteSectMmu () ;

            @arr_mmu        = () ;
            $section_context   = $SECT_NONE ;
        }
        elsif ($line  =~ /^$tkn_mqa_end/)
        {
            &WriteSectMqa () ;

            @arr_mqa        = () ;
            $section_context   = $SECT_NONE ;
        }
        elsif ($line  =~ /^$tkn_mqt_end/)
        {
            &WriteSectMqt () ;

            @arr_mqt        = () ;
            $section_context   = $SECT_NONE ;
        }
        else
        {
            #   ----------------------------------------------------------------
            #   Valid configuration item.
            #   Update corresponding array.
            #   ----------------------------------------------------------------
            if    ($section_context == $SECT_DRIVER)
            {
                push (@arr_driver, $line) ;
            }
            elsif ($section_context == $SECT_GPP)
            {
                push (@arr_gpp, $line) ;
            }
            elsif ($section_context == $SECT_DSP)
            {
                push (@arr_dsp, $line) ;
            }
            elsif ($section_context == $SECT_LINK)
            {
                push (@arr_link, $line) ;
            }
            elsif ($section_context == $SECT_MMU)
            {
                push (@arr_mmu, $line) ;
            }
            elsif ($section_context == $SECT_MQA)
            {
                push (@arr_mqa, $line) ;
            }
            elsif ($section_context == $SECT_MQT)
            {
                push (@arr_mqt, $line) ;
            }
        }
    }
}

#   ============================================================================
#   Close output file
#   ============================================================================
&Debug_0 ("\nClose output file\n") ;

close ($filehandle) ;

#   ============================================================================
#   Done !!!
#   ============================================================================
&Debug_1 ("\nDone !!!\n") ;

exit (0) ;


#   ############################################################################
#   SUBROUTINES
#   ############################################################################

#   ============================================================================
#   func    ShowConfiguration
#
#   desc    Print the information regarding the configuration currently
#           being used for building DSP/BIOS LINK.
#           The generated code depends upon this configuration.
#   ============================================================================
sub ShowConfiguration ()
{
    my ($str) = "" ;

    $str .= "================================================================\n" ;
    $str .= "Current Configuration : " . $currentcfg . "\n",  ;
    $str .= "Created on            : " . $CFG_DATE . "\n",  ;
    $str .= "================================================================\n" ;
    $str .= "GPP OS         = " . $VAL_GPPOS        . "\n" ;
    $str .= "DISTRIBUTION   = " . $VAL_DISTRIBUTION . "\n" ;
    $str .= "\n" ;
    $str .= "PLATFORM       = " . $VAL_PLATFORM     . "\n" ;
    $str .= "VARIANT        = " . $VAL_VARIANT      . "\n" ;
    $str .= "\n" ;
    $str .= "USE_MSGQ       = " . $USE_MSGQ         . "\n" ;
    $str .= "USE_CHNL       = " . $USE_CHNL         . "\n" ;
    $str .= "USE_PROC       = " . $USE_PROC         . "\n" ;
    $str .= "USE_DSP        = " . $USE_DSP          . "\n" ;
    $str .= "================================================================\n" ;

    print $str ;
}


#   ============================================================================
#   func    WriteFileHeader
#
#   desc    Write file header for generated file.
#   ============================================================================
sub WriteFileHeader ()
{
    &Debug_0 ("\nEntered WriteFileHeader\n") ;

    if ($target eq "GPP")
    {
        &GPP_WriteFileHeader () ;
    }
    else
    {
        &DSP_WriteFileHeader () ;
    }
}


#   ============================================================================
#   func    WriteSectDriver
#
#   desc    Write section: DRIVER
#   ============================================================================
sub WriteSectDriver ()
{
    &Debug_0 ("\nEntered WriteSectDriver\n") ;

    if ($target eq "GPP")
    {
        &GPP_WriteSectDriver () ;
    }
    else
    {
        &DSP_WriteSectDriver () ;
    }
}


#   ============================================================================
#   func    WriteSectGpp
#
#   desc    Write section: GPP
#   ============================================================================
sub WriteSectGpp ()
{
    &Debug_0 ("\nEntered WriteSectGpp\n") ;

    if ($target eq "GPP")
    {
        &GPP_WriteSectGpp () ;
    }
    else
    {
        &DSP_WriteSectGpp () ;
    }
}


#   ============================================================================
#   func    WriteSectDsp
#
#   desc    Write section: DSP
#   ============================================================================
sub WriteSectDsp ()
{
    &Debug_0 ("\nEntered WriteSectDsp\n") ;

    if ($target eq "GPP")
    {
        &GPP_WriteSectDsp () ;
    }
    else
    {
        &DSP_WriteSectDsp () ;
    }
}


#   ============================================================================
#   func    WriteSectLink
#
#   desc    Write section: LINK
#   ============================================================================
sub WriteSectLink ()
{
    &Debug_0 ("\nEntered WriteSectLink\n") ;

    if ($target eq "GPP")
    {
        &GPP_WriteSectLink () ;
    }
    else
    {
        &DSP_WriteSectLink () ;
    }
}


#   ============================================================================
#   func    WriteSectMmu
#
#   desc    Write section: MMU
#   ============================================================================
sub WriteSectMmu ()
{
    &Debug_0 ("\nEntered WriteSectMmu\n") ;

    if ($target eq "GPP")
    {
        &GPP_WriteSectMmu () ;
    }
    else
    {
        &DSP_WriteSectMmu () ;
    }
}


#   ============================================================================
#   func    WriteSectMqa
#
#   desc    Write section: MQA
#   ============================================================================
sub WriteSectMqa ()
{
    &Debug_0 ("\nEntered WriteSectMqa\n") ;

    if ($target eq "GPP")
    {
        &GPP_WriteSectMqa () ;
    }
    else
    {
        &DSP_WriteSectMqa () ;
    }
}


#   ============================================================================
#   func    WriteSectMqt
#
#   desc    Write section: MQT
#   ============================================================================
sub WriteSectMqt ()
{
    &Debug_0 ("\nEntered WriteSectMqt\n") ;

    if ($target eq "GPP")
    {
        &GPP_WriteSectMqt () ;
    }
    else
    {
        &DSP_WriteSectMqt () ;
    }
}


#   ############################################################################
#   FUNCTIONS TO GENERATE CODE FOR GPP SIDE
#   ############################################################################

#   ============================================================================
#   func    GPP_WriteFileHeader
#
#   desc    Write file header
#   ============================================================================
sub GPP_WriteFileHeader
{
    my ($outstr)  = "\n\n" ;
    my ($datestr) = "" ;
    my (@comment) = () ;

    &Debug_0 ("    [GPP] Write file header\n") ;

    $datestr .= &GetDateString () ;

    push (@comment, $STR_CAUTION1) ;
    push (@comment, $STR_CAUTION2) ;
    push (@comment, $STR_CAUTION3) ;
    push (@comment, "") ;

    $datestr  = "The configuration was created on : " . $CFG_DATE ;

    push (@comment, $datestr) ;

    $datestr  = "This file was generated on       : " . &GetDateString () ;

    push (@comment, $datestr) ;

    &WriteComment (\@comment) ;

    $outstr  = "#include <gpptypes.h>\n" ;
    $outstr .= "#include <dsplink.h>\n" ;
    $outstr .= "#include <intobject.h>\n" ;
    $outstr .= "#include <errbase.h>\n\n" ;
    $outstr .= "#include <cfgdefs.h>\n" ;
    $outstr .= "#include <dspdefs.h>\n" ;
    $outstr .= "#include <linkdefs.h>\n\n" ;
    $outstr .= "#if defined (MSGQ_COMPONENT)\n" ;
    $outstr .= "#include <msgqdefs.h>\n" ;
    $outstr .= "#endif  /* defined (MSGQ_COMPONENT) */\n" ;
    $outstr .= "\n\n" ;

    print $filehandle $outstr ;
}


#   ============================================================================
#   func    GPP_WriteSectDriver
#
#   desc    [GPP] Write section: DRIVER
#   ============================================================================
sub GPP_WriteSectDriver ()
{
    my (@code)      = () ;
    my ($str)       = "" ;
    my (@comment)   = () ;

    &Debug_0 ("    [GPP] Write Section: DRIVER\n") ;

    push (@comment, "DRIVER") ;

    #   ------------------------------------------------------------------------
    #   Parse each line in the section
    #   ------------------------------------------------------------------------
    foreach my $item (@arr_driver)
    {
        my ($name, $type, $value)   = &ParseCfgEntry ($item) ;
        my ($found)                 = &GetTokenInfo (\@Tokens_DRIVER, $name) ;

        if ($found)
        {
            my ($tmpstr) = &GetCodeString ($INDENT_LEVEL_1, $name, $type, $value) ;
            push (@code, $tmpstr) ;
        }
    }

    #   ------------------------------------------------------------------------
    #   Write the structure
    #   ------------------------------------------------------------------------
    &WriteComment (\@comment) ;

    $str = &GPP_GetSectionString ($GPP_TYPE_DRIVER,
                                  $GPP_VAR_DRIVER,
                                  \@code,
                                  $FALSE) ;


    print $filehandle "$str\n" ;
}


#   ============================================================================
#   func    GPP_WriteSectGpp
#
#   desc    [GPP] Write section: GPP
#   ============================================================================
sub GPP_WriteSectGpp ()
{
    my (@code)      = () ;
    my ($str)       = "" ;
    my (@comment)   = () ;

    &Debug_0 ("    [GPP] Write Section: GPP\n") ;

    push (@comment, "GPP") ;

    #   ------------------------------------------------------------------------
    #   Parse each line in the section
    #   ------------------------------------------------------------------------
    foreach my $item (@arr_gpp)
    {
        my ($name, $type, $value)   = &ParseCfgEntry ($item) ;
        my ($found)                 = &GetTokenInfo (\@Tokens_GPP, $name) ;

        if ($found)
        {
            my ($tmpstr) = &GetCodeString ($INDENT_LEVEL_1, $name, $type, $value) ;
            push (@code, $tmpstr) ;
        }
    }

    #   ------------------------------------------------------------------------
    #   Write the structure
    #   ------------------------------------------------------------------------
    &WriteComment (\@comment) ;

    $str = &GPP_GetSectionString ($GPP_TYPE_GPP,
                                  $GPP_VAR_GPP,
                                  \@code,
                                  $FALSE) ;

    print $filehandle "$str\n" ;
}


#   ============================================================================
#   func    GPP_WriteSectDsp
#
#   desc    [GPP] Write section: DSP
#   ============================================================================
sub GPP_WriteSectDsp ()
{
    my (@code)      = () ;
    my ($str)       = "" ;
    my (@comment)   = () ;
    my ($intf_ldr)  = "" ;
    my ($intf_dsp)  = "" ;

    &Debug_0 ("    [GPP] Write Section: DSP\n") ;

    push (@comment, "DSP") ;

    #   ------------------------------------------------------------------------
    #   Parse each line in the section
    #   ------------------------------------------------------------------------
    foreach my $item (@arr_dsp)
    {
        my ($name, $type, $value)   = &ParseCfgEntry ($item) ;
        my ($found)                 = &GetTokenInfo (\@Tokens_DSP, $name) ;

        if ($found)
        {
            my ($tmpstr) = &GetCodeString ($INDENT_LEVEL_1, $name, $type, $value) ;
            push (@code, $tmpstr) ;

            if    ($name eq 'LOADER')
            {
                $intf_ldr = $value ;
            }
            elsif ($name eq 'INTERFACE')
            {
                $intf_dsp   = $value ;
            }
        }
    }

    #   ------------------------------------------------------------------------
    #   Write the structure
    #   ------------------------------------------------------------------------
    &WriteComment (\@comment) ;

    print $filehandle "EXTERN LoaderInterface  " . $intf_ldr . " ;\n" ;
    print $filehandle "EXTERN DspInterface     " . $intf_dsp . " ;\n\n" ;

    $str = &GPP_GetSectionString ($GPP_TYPE_DSP,
                                  $GPP_VAR_DSP . $id_dsp,
                                  \@code,
                                  $FALSE) ;

    print $filehandle "$str\n" ;
}


#   ============================================================================
#   func    GPP_WriteSectLink
#
#   desc    [GPP] Write section: LINK
#   ============================================================================
sub GPP_WriteSectLink ()
{
    my (@code)      = () ;
    my ($str)       = "" ;
    my (@intf)      = () ;
    my (@comment)   = () ;

    &Debug_0 ("    [GPP] Write Section: LINK\n") ;

    push (@comment, "LINKTABLE") ;

    #   ------------------------------------------------------------------------
    #   Parse each line in the section
    #   ------------------------------------------------------------------------
    foreach my $item (@arr_link)
    {
        if    ($item =~ /^\[(\d+)\]$/)
        {
            push (@code, &GetIndentString ($INDENT_LEVEL_1) . "{") ;
        }
        elsif ($item =~ /^\[\/(\d+)\]$/)
        {
            push (@code, &GetIndentString ($INDENT_LEVEL_1) . "}") ;
        }
        else
        {
            my ($name, $type, $value)   = &ParseCfgEntry ($item) ;
            my ($found)                 = &GetTokenInfo (\@Tokens_LINK, $name) ;

            if ($found)
            {
                my ($tmpstr) = &GetCodeString ($INDENT_LEVEL_2, $name, $type, $value) ;
                push (@code, $tmpstr) ;

                if ($type eq 'A')
                {
                    push (@intf, $value) ;
                }
            }
        }
    }

    #   ------------------------------------------------------------------------
    #   Write the structure
    #   ------------------------------------------------------------------------
    &WriteComment (\@comment) ;

    foreach my $item (@intf)
    {
        print $filehandle "EXTERN LinkInterface " . $item . " ;\n" ;
    }
    print $filehandle "\n" ;

    $str = &GPP_GetSectionString ($GPP_TYPE_LINK,
                                  $GPP_VAR_LINK . $id_link,
                                  \@code,
                                  $TRUE) ;

    print $filehandle "$str\n" ;
}


#   ============================================================================
#   func    GPP_WriteSectMmu
#
#   desc    [GPP] Write section: MMU
#   ============================================================================
sub GPP_WriteSectMmu ()
{
    my (@code)      = () ;
    my ($str)       = "" ;
    my (@intf)      = () ;
    my (@comment)   = () ;

    &Debug_0 ("    [GPP] Write Section: MMU\n") ;

    push (@comment, "MMU TABLE") ;

    #   ------------------------------------------------------------------------
    #   Parse each line in the section
    #   ------------------------------------------------------------------------
    foreach my $item (@arr_mmu)
    {
        if    ($item =~ /^\[(\d+)\]$/)
        {
            push (@code, &GetIndentString ($INDENT_LEVEL_1) . "{") ;

            my ($tmpstr) = &GetCodeString ($INDENT_LEVEL_2, 'ENTRY', 'N', $1) ;
            push (@code, $tmpstr) ;
        }
        elsif ($item =~ /^\[\/(\d+)\]$/)
        {
            push (@code, &GetIndentString ($INDENT_LEVEL_1) . "}") ;
        }
        else
        {
            my ($name, $type, $value)   = &ParseCfgEntry ($item) ;
            my ($found)                 = &GetTokenInfo (\@Tokens_MMU, $name) ;

            if ($found)
            {
                my ($tmpstr) = &GetCodeString ($INDENT_LEVEL_2, $name, $type, $value) ;
                push (@code, $tmpstr) ;

                if ($type eq 'A')
                {
                    push (@intf, $value) ;
                }
            }
        }
    }

    #   ------------------------------------------------------------------------
    #   Write the structure
    #   ------------------------------------------------------------------------
    &WriteComment (\@comment) ;

    $str = &GPP_GetSectionString ($GPP_TYPE_MMU,
                                  $GPP_VAR_MMU . $id_mmu,
                                  \@code,
                                  $TRUE) ;

    print $filehandle "$str\n" ;
}


#   ============================================================================
#   func    GPP_WriteSectMqa
#
#   desc    [GPP] Write section: MQA
#   ============================================================================
sub GPP_WriteSectMqa ()
{
    my (@code)      = () ;
    my ($str)       = "" ;
    my (@intf)      = () ;
    my (@comment)   = () ;

    &Debug_0 ("    [GPP] Write Section: MQA\n") ;

    push (@comment, "MQA") ;

    #   ------------------------------------------------------------------------
    #   Parse each line in the section
    #   ------------------------------------------------------------------------
    foreach my $item (@arr_mqa)
    {
        my ($name, $type, $value)   = &ParseCfgEntry ($item) ;
        my ($found)                 = &GetTokenInfo (\@Tokens_MQA, $name) ;

        if ($found)
        {
            my ($tmpstr) = &GetCodeString ($INDENT_LEVEL_1, $name, $type, $value) ;
            push (@code, $tmpstr) ;

            if ($type eq 'A')
            {
                push (@intf, $value) ;
            }
        }
    }

    #   ------------------------------------------------------------------------
    #   Write the structure
    #   ------------------------------------------------------------------------
    &WriteComment (\@comment) ;

    foreach my $item (@intf)
    {
        print $filehandle "EXTERN MqaInterface " . $item . " ;\n" ;
    }
    print $filehandle "\n" ;

    $str = &GPP_GetSectionString ($GPP_TYPE_MQA,
                                  $GPP_VAR_MQA . $id_mqa,
                                  \@code,
                                  $FALSE) ;

    print $filehandle "$str\n" ;
}


#   ============================================================================
#   func    GPP_WriteSectMqt
#
#   desc    [GPP] Write section: MQT
#   ============================================================================
sub GPP_WriteSectMqt ()
{
    my (@code)      = () ;
    my ($str)       = "" ;
    my (@intf)      = () ;
    my (@comment)   = () ;

    &Debug_0 ("    [GPP] Write Section: MQT\n") ;

    push (@comment, "MQT") ;

    #   ------------------------------------------------------------------------
    #   Parse each line in the section
    #   ------------------------------------------------------------------------
    foreach my $item (@arr_mqt)
    {
        my ($name, $type, $value)   = &ParseCfgEntry ($item) ;
        my ($found)                 = &GetTokenInfo (\@Tokens_MQT, $name) ;

        if ($found)
        {
            my ($tmpstr) = &GetCodeString ($INDENT_LEVEL_1, $name, $type, $value) ;
            push (@code, $tmpstr) ;

            if ($type eq 'A')
            {
                push (@intf, $value) ;
            }
        }
    }

    #   ------------------------------------------------------------------------
    #   Write the structure
    #   ------------------------------------------------------------------------
    &WriteComment (\@comment) ;

    foreach my $item (@intf)
    {
        print $filehandle "EXTERN MqtInterface " . $item . " ;\n" ;
    }
    print $filehandle "\n" ;

    $str = &GPP_GetSectionString ($GPP_TYPE_MQT,
                                  $GPP_VAR_MQT . $id_mqt,
                                  \@code,
                                  $FALSE) ;

    print $filehandle "$str\n" ;
}


#   ============================================================================
#   func    ParseCfgEntry
#
#   desc    Get name, type and value from a given entry
#   ============================================================================
sub ParseCfgEntry
{
    my ($line) = @_ ;
    my ($name, $type, $value) = split (/\s*\|\s*/, $line) ;

    if (   ($type ne 'N')   # Decimal value
        && ($type ne 'H')   # Hexadecimal value
        && ($type ne 'S')   # String
        && ($type ne 'A')   # Address
        && ($type ne 'B')   # Boolean value
        && ($type ne 'E'))  # Enumerated value
    {
        print "!! Error. Invalid type specified\n" ;
        print "=> ", $line, "\n" ;
    }

    return $name, $type, $value ;
}


#   ############################################################################
#   FUNCTIONS TO GENERATE CODE FOR DSP SIDE
#   ############################################################################

#   ============================================================================
#   func    DSP_WriteFileHeader
#
#   desc    [DSP] Write file header
#   ============================================================================
sub DSP_WriteFileHeader
{
    my ($outstr)  = "" ;
    my ($datestr) = "" ;
    my (@comment) = () ;

    &Debug_0 ("    [DSP] Write file header\n") ;

    push (@comment, $STR_CAUTION1) ;
    push (@comment, $STR_CAUTION2) ;
    push (@comment, $STR_CAUTION3) ;
    push (@comment, "") ;

    $datestr  = "The configuration was created on : " . $CFG_DATE ;

    push (@comment, $datestr) ;

    $datestr  = "This file was generated on       : " . &GetDateString () ;

    push (@comment, $datestr) ;

    &WriteComment (\@comment) ;

    $outstr = "\n\n#include <std.h>\n\n" ;

    print $filehandle $outstr ;
}


#   ============================================================================
#   func    DSP_WriteSectDriver
#
#   desc    [DSP] Write section: DRIVER
#   ============================================================================
sub DSP_WriteSectDriver ()
{
    &Debug_0 ("    [DSP] Write Section: DRIVER\n") ;
}


#   ============================================================================
#   func    DSP_WriteSectGpp
#
#   desc    [DSP] Write section: GPP
#   ============================================================================
sub DSP_WriteSectGpp ()
{
    &Debug_0 ("    [DSP] Write Section: GPP\n") ;
}


#   ============================================================================
#   func    DSP_WriteSectDsp
#
#   desc    [DSP] Write section: DSP
#   ============================================================================
sub DSP_WriteSectDsp ()
{
    my ($linktable) = "" ;

    &Debug_0 ("    [DSP] Write Section: DSP\n") ;

}


#   ============================================================================
#   func    DSP_WriteSectLink
#
#   desc    [DSP] Write section: LINK
#   ============================================================================
sub DSP_WriteSectLink ()
{
    my (@code)          = () ;
    my ($str)           = "" ;
    my (@comment)       = () ;

    my ($LINK_NONE)     = 0 ;
    my ($LINK_START)    = 1 ;
    my ($LINK_END)      = 2 ;

    my ($context)       = $LINK_NONE ;

    my ($chnl_abbr)     = "" ;      # Abbreviation used for given link
    my ($chnl_base)     = 0 ;       # Number of channels in given link
    my ($chnl_link)     = 0 ;       # Number of channels in given link
    my ($chnl_total)    = 0 ;       # Number of channels in specific DSP

    &Debug_0 ("    [DSP] Write Section: LINK\n") ;

    #   ------------------------------------------------------------------------
    #   Parse each line in the section
    #   ------------------------------------------------------------------------
    foreach my $item (@arr_link)
    {
        if    ($item =~ /^\[(\d+)\]$/)
        {
            $context = $LINK_START ;
        }
        elsif ($item =~ /^\[\/(\d+)\]$/)
        {
            $context = $LINK_END ;

            push (@code, $chnl_abbr . "," . $chnl_base . "," . $chnl_link) ;
        }
        elsif ($context == $LINK_START)
        {
            my ($name, $type, $value)   = &ParseCfgEntry ($item) ;
            my ($found)                 = &GetTokenInfo (\@Tokens_LINK, $name) ;

            if ($found)
            {
                if    ($name eq 'BASECHANNELID')
                {
                    $chnl_base = $value ;
                }
                elsif ($name eq 'NUMCHANNELS')
                {
                    $chnl_link   = $value ;
                    $chnl_total += $value ;
                }
                elsif ($name eq 'ABBR')
                {
                    $chnl_abbr   = $value ;
                }
            }
        }
    }

    #   ------------------------------------------------------------------------
    #   Write the code
    #   ------------------------------------------------------------------------
    push (@comment, "Maximum number of channels within the complete system.") ;
    &WriteComment (\@comment) ;

    printf $filehandle "#define %-30s %d\n\n", "MAX_CHANNELS", $chnl_total ;

    @comment = () ;

    #   ------------------------------------------------------------------------
    #   Write the code : Base & number of channels
    #   ------------------------------------------------------------------------
    push (@comment, "Base & Number of channels in each configured link.") ;
    &WriteComment (\@comment) ;

    foreach my $line (@code)
    {
        my ($name, $base, $num) = split (/,/, $line) ;

        $str = "BASE_" . $name . "_CHANNELS" ;
        printf $filehandle "#define %-30s %d\n", $str, $base ;

        $str = "NUM_" . $name . "_CHANNELS" ;
        printf $filehandle "#define %-30s %d\n\n", $str, $num ;
    }
}


#   ============================================================================
#   func    DSP_WriteSectMmu
#
#   desc    [DSP] Write section: MMU
#   ============================================================================
sub DSP_WriteSectMmu ()
{
    &Debug_0 ("    [DSP] Write Section: MMU\n") ;
}


#   ============================================================================
#   func    DSP_WriteSectMqa
#
#   desc    [DSP] Write section: MQA
#   ============================================================================
sub DSP_WriteSectMqa ()
{
    &Debug_0 ("    [DSP] Write Section: MQA\n") ;
}


#   ============================================================================
#   func    DSP_WriteSectMqt
#
#   desc    [DSP] Write section: MQT
#   ============================================================================
sub DSP_WriteSectMqt ()
{
    &Debug_0 ("    [DSP] Write Section: MQT\n") ;
}


#   ############################################################################
#   GENERIC HELPER FUNCTIONS
#   ############################################################################

#   ============================================================================
#   func    GetTagId
#
#   desc    Get ID from a given tag.
#   ============================================================================
sub GetTagId
{
    my ($tag) = @_ ;

    if ($tag < 10)
    {
        $tag = "0" . $tag
    }

    return $tag ;
}


#   ============================================================================
#   func    GetCodeString
#
#   desc    Get the code string corresponding to specifed name, value and
#           indent level.
#   ============================================================================
sub GetCodeString
{
    my ($level) = shift (@_) ;
    my ($name)  = shift (@_) ;
    my ($type)  = shift (@_) ;
    my ($value) = @_ ;

    my ($str_lhs) = '' ;            # Indent
    my ($str_mid) = '' ;            # Comment
    my ($str_rhs) = '' ;            # Actual code

    $str_lhs = &GetIndentString  ($level) ;
    $str_mid = &GetCommentString ($name) ;
    $str_rhs = &GetValueString   ($type, $value) ;

    return $str_lhs . $str_mid . $str_rhs ;
}


#   ============================================================================
#   func    GetIndentString
#
#   desc    Get the string corresponding to specified indent level.
#   ============================================================================
sub GetIndentString
{
    my ($level)  = @_ ;
    my ($string) = '' ;

    do
    {
        $string .= '    ' ;
        $level-- ;
    } while ($level > 0) ;

    return $string ;
}


#   ============================================================================
#   func    GetCommentString
#
#   desc    Get the comment string for specified token
#   ============================================================================
sub GetCommentString
{
    my ($name)   = @_ ;
    my ($string) = '' ;

    $string = sprintf "/* %-16s */  ", $name ;

    return $string ;
}


#   ============================================================================
#   func    GetCurrentCfg
#
#   desc    Get the name of file containing current configuration.
#   ============================================================================
sub GetCurrentCfg
{
    my ($file)   = "" ;

    if ($ENV {'COMSPEC'} ne "" )
    {
        $file = $ENV {'DSPLINK'} . '\config\CURRENTCFG.MK' ;
    }
    else
    {
        $file = $ENV {'DSPLINK'} . '/config/CURRENTCFG.MK' ;
    }

    return $file ;
}


#   ============================================================================
#   func    GetValueString
#
#   desc    Get the value string for specified value & type.
#   ============================================================================
sub GetValueString
{
    my ($type)   = shift (@_) ;
    my ($value)  = @_ ;

    my ($string) = '' ;

    #   ------------------------------------------------------------------------
    #   Type: String
    #   ------------------------------------------------------------------------
    if ($type eq 'S')
    {
        my ($len) = length ($value) ;

        if ($len > 32)
        {
            print "!!! Error. String is longer than 32 characters\n" ;
            print "=> ", $value, "\n" ;
        }
        else
        {
            for (my $i = $len ; $i < 32 ; $i++)
            {
                $value = $value . '\\0' ;
            }

            $string = $string . "\"" . $value . "\"" ;
        }
    }
    #   ------------------------------------------------------------------------
    #   Type: Hexadecimal value
    #   ------------------------------------------------------------------------
    elsif ($type eq 'H')
    {
        $string = $string . $value ;
    }
    #   ------------------------------------------------------------------------
    #   Type: Address
    #   ------------------------------------------------------------------------
    elsif ($type eq 'A')
    {
        $string = $string . '&' . $value ;
    }
    #   ------------------------------------------------------------------------
    #   Type: Boolean
    #   ------------------------------------------------------------------------
    elsif ($type eq 'B')
    {
        $string = $string . "(Uint32) " . $value ;
    }
    #   ------------------------------------------------------------------------
    #   Type: Enumeration
    #   ------------------------------------------------------------------------
    elsif ($type eq 'E')
    {
        $string = $string . "(Uint32) " . $value ;
    }
    #   ------------------------------------------------------------------------
    #   Type: Decimal value
    #   ------------------------------------------------------------------------
    else
    {
        $string = $string . $value ;
    }

    return $string ;
}


#   ============================================================================
#   func    StartDeclaration
#
#   desc    Starts the declaration.
#
#   arg 1   qualifier       CONST/ EXTERN
#   arg 2   type            type definition
#   arg 3   name            name of variable
#   arg 4   flag            Is is an array declaration
#   ============================================================================
sub StartDeclaration ()
{
    my ($qualifier) = shift (@_) ;
    my ($type)      = shift (@_) ;
    my ($name)      = shift (@_) ;
    my ($flag)      = @_ ;

    my ($str)       = "" ;

    if ($flag == $TRUE)
    {
        $str = "$qualifier $type $name [] = {\n" ;
    }
    else
    {
        $str = "$qualifier $type $name = {\n" ;
    }

    return $str ;
}

#   ============================================================================
#   func    EndDeclaration
#
#   desc    Ends the declaration.
#   ============================================================================
sub EndDeclaration ()
{
    return "} ;\n" ;
}

#   ============================================================================
#   func    GPP_GetSectionString
#
#   desc    Get the string corresponding to given section
#   ============================================================================
sub GPP_GetSectionString ()
{
    my ($type)      = shift (@_) ;
    my ($var)       = shift (@_) ;
    my ($arr_code)  = shift (@_) ;
    my ($isArray)   = @_ ;

    my ($str_start) = &StartDeclaration ("CONST", $type, $var, $isArray) ;
    my ($str_mid)   = "" ;
    my ($str_end)   = &EndDeclaration   () ;

    my ($strcode)   = "" ;

    foreach my $str (@$arr_code)
    {
        $str_mid .= $str . ",\n" ;
    }

    if ($str_mid ne "")
    {
        $str_mid =~ s/{,/{/g   ;                    # Remove comma after "{"
        $str_mid =~ s/,\n(\s+)}/\n$1}/g ;           # Remove comma before "}"
        $str_mid =~ s/,\n$/\n/ ;                    # Remove trailing comma

        $strcode = $str_start . $str_mid . $str_end ;
    }

    return $strcode ;
}


#   ============================================================================
#   func    GetTokenInfo
#
#   desc    Searches the token in specified array & returns the corresponding
#           information.
#   ============================================================================
sub GetTokenInfo
{
    my ($list)   = shift @_ ;
    my ($item)   = @_ ;

    my ($index)  = 0  ;
    my ($found)  = 0  ;

    #   ------------------------------------------------------------------------
    #   Search for the token in corresponding array
    #   ------------------------------------------------------------------------
    do
    {
        if ($$list [$index][0] eq $item)
        {
            $found  = 1 ;
        }
        $index++ ;

    } while (($found == 0) && ($index < scalar (@$list))) ;

    $index-- ;

    if ($found == 0)
    {
        if (!($item =~ /^\[.?\]$/))
        {
            print "!!! Illegal keyword: ", $item, "\n" ;
        }
    }

    #   ------------------------------------------------------------------------
    #   Include the token based on build configuration.
    #   ------------------------------------------------------------------------
    elsif ($$list [$index][1] ne 'GENERIC')
    {
        if    (($USE_MSGQ == 0) && ($$list [$index][1] eq 'MSGQ_COMPONENT'))
        {
            $found =  0 ;
        }
        elsif (($USE_CHNL == 0) && ($$list [$index][1] eq 'CHNL_COMPONENT'))
        {
            $found =  0 ;
        }
    }

    return $found ;
}


#   ============================================================================
#   func    WriteComment
#
#   desc    Write the "C" style comments with given text.
#   ============================================================================
sub WriteComment
{
    my ($list)  = @_ ;
    my ($str)   = "" ;

    $str .= "/*  ------------------------------------" ;
    $str .= "----------------------------------------\n" ;

    foreach my $line (@$list)
    {
        $str .= " *  " . $line . "\n" ;
    }

    $str .= " *  ------------------------------------" ;
    $str .= "----------------------------------------\n" ;
    $str .= " */\n" ;

    print $filehandle $str ;
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


#   ############################################################################
#   DEBUG FUNCTIONS
#   ############################################################################

#   ============================================================================
#   func    Debug_0
#
#   desc    Print a string if debug level matches.
#   ============================================================================
sub Debug_0
{
    my ($string) = @_ ;

    if ($debug == 1)
    {
        print $string ;
    }
}


#   ============================================================================
#   func    Debug_1
#
#   desc    Print a format string and an argument if debug level matches.
#   ============================================================================
sub Debug_1
{
    my ($format)   = shift (@_) ;
    my ($argument) = @_ ;

    if ($debug == 1)
    {
        printf $format, $argument ;
    }
}
