#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.

#Include tf.ahk

Array_orig := [92,93,88,89,87,91,90,86,85,84,81,83,82]
Array_replace := [22,23,18,19,17,21,20,16,15,14,11,13,12]
FileLoc := "E:\Flight Sim\Community\A32NX\SimObjects\AirPlanes\Asobo_A320_NEO\model\A320_NEO_INTERIOR.xml"


while (!FileExist(FileLoc)) 
{
	MsgBox, 1, Please locate A32NX mod folder, Could not find  A32NX mod install folder. Please specify location of "A32NX" folder.
	IfmsgBox Cancel
		Exit
	FileSelectFolder, FileLoc
	FileLoc := FileLoc . "\SimObjects\AirPlanes\Asobo_A320_NEO\model\A320_NEO_INTERIOR.xml"

}


for index, element in Array_orig ; Enumeration is the recommended approach in most cases.
{
	orig := "<POTENTIOMETER>" . Array_orig[index]  . "</POTENTIOMETER>"
	replace := "<POTENTIOMETER>" . Array_replace[index]  . "</POTENTIOMETER>"
	file := "!" . FileLoc 
	TF_Replace(file,orig,replace)
}

MsgBox A32NX potentiometer indices successfully updated!
Exit
