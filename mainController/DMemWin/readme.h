/* (Had to make this README file .h to get it exportable in mbed)


	There is a better formatted version of this guide with better explanations:

	http://developer.mbed.org/teams/Embedded-Artists/wiki/LPC4088DM-Guide-to-Working-with-emWin


    emWin Graphical Library
    -----------------------
    According to http://www.lpcware.com/content/project/emwin-graphics-library:
    
    "The high performance emWin embedded graphics library developed by SEGGER 
     Microcontroller is now offered by NXP Semiconductors in library form for 
     free commercial use with NXP microcontrollers."
    
    Follow the link above to read more about the graphical library and available
    features.
    
    NOTE: The emWin library cannot be used in the online compiler for legal
          reasons. You have to export the project and then follow the instructions
          below to download emWin and then modify your local project.
    
    How to get it
    -------------
    The library must be downloaded from the page above and at the time this
    document was written the latest version was emWin 5.24h with the direct
    link: http://www.lpcware.com/system/files/NXP_emWin524h_libraries.exe
    
    Download the executable, run it and have it extract the files to the emwin/
    folder. After extracting the archive, the emwin/ folder should contain the
    following:
    
        emWin_Config\
        emWin_header\
        emWin_library\
        readme.txt
    
	Delete the emWin_Config\ folder as it is not needed
	
    The the 5.24h version (and possibly other later versions as well) is missing
    PNG support but the manual (available on the linked page above) has instructions
    on how to add it. Download www.segger.com/link/emwin_png.zip and extract it
    into the png/ folder. After extracting the folder should contain one or more
    version folders. This is what the png/ folder may look like:
    
        emWin_V5.18/
        emWin_V5.28/
        
    Delete the version that you don't need (probably the oldest one).
    
    Updating the project
    --------------------
    When this project was exported it did not have the correct paths and files
    needed by emWin (for licence reasons). This is what you have to do in your
    IDE to get it to work:
    
        Keil uVision 4:
        ==============
        1) Open the project
        2) Select the Project->Manage menu
        3) Add a new Group "emLib"
        4) Add all DMemWin/png/emWin_V5.28/GUI/PNG/*.c files "emLib" group
        5) Add DMemWinGUI/emwin/emWin_library/Keil/emWin_M4.lib to the "emLib" group
        6) Close the dialog
        
        NXP LPCXpresso IDE:
        ==================
        1) Open the workspace
        2) Import the project from the exported zip file
        3) Right-click on the DMemWin folder in the Project Explorer and select Refresh...
		4) Right-click on the project name and select Properties...
			a) "C/C++ General" -> "Paths and Symbols" and select the "Library Paths" tab. 
			b) Click the Add button 
			c) Copy-paste the following into the dialog box: 
			
				/${ProjName}/DMemWin/emwin/emWin_library/LPCXpresso
			
			d) Make sure that the "Is a workspace path" option is selected and click OK 
			e) Switch to the Libraries tab 
			f) Click the Add button 
			g) Copy-paste the following into the dialog box: 
			
				emWin_M4
			
			h) Click the OK button until all dialogs are closed. 
			
        Other IDEs:
        ==========
        Do the corresponding actions for your IDE but remember to use the correct
        library file.
        
        If the IDE doesn't get the paths correct by the exporter then you might have
        to add the following to the include path:
        
            DMemWin/emwin/emWin_header/

*/
