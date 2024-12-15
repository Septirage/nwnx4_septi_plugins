set "rootPath=%~dp0"
set "buildPath=%~1"
set "pluginPath=%~2"

set "versionPath=%buildPath%/Version.rc"
set "resPath=%buildPath%/Version.res"


"%rootPath%tool\ResourceHacker.exe" -open "%versionPath%" -save "%resPath%" -action compile
"%rootPath%tool\ResourceHacker.exe" -open "%pluginPath%" -save "%pluginPath%" -action addoverwrite -resource "%resPath%"
