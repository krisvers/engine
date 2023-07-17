.\build.bat
if ($?) {
	.\copy.bat
	if ($?) {
		.\run.bat
	}
}