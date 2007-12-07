
The Reference Framework Level 6 code package is delivered with the
 following files:

- rf_v<VERSION>.zip: DSP-side source code for RF Level 6 application and
  Framework modules. Also includes RF Levels 1, 3, 5.
- rf_gpp_v<VERSION>.tar.gz: GPP-side minimal source code for RF Level 6
  application and associated modules. 
- relnotes_rf_v<VERSION>.html: release notes with useful information 
  such as bug fixes. 
- filediff_rf_v<VERSION>.txt: file showing the differences between this 
  version and the previous version.

Why is the source code package split into two files? The primary reason is 
 that the home for each of these deliverables may be quite different. The 
 DSP-side content typically sits on a PC running Windows while the 
 GPP-side code is on an NFS mount of a Linux host - the two file systems 
 may be entirely separate.

To install RF6, 

1.	Unzip the DSP-side code. The TI_DIR\myprojects (where TI_DIR stands
 for CCS installation directory, e.g. c:\ti) folder is a suggested location
 for these files. Make sure to use directory names when you unzip the file.
 You may need to enable an option called something similar to "Use folder
 names" in your zip utility.

On the Linux workstation,

2.	Make a directory with name of your choice
[>] cd ~/montavista
[>] mkdir -p xyzdir

3.	Go into the directory 
[>] cd xyzdir

4.	Untar the GPP-side code in a directory of your choice:
[>] tar xvzf rf_gpp_v<VERSION>.tar.gz  

Please refer to the chapter 4, Installing, Building, and Running the RF6 
  Base Application in spra796 for details on how to build and run the 
  application.


