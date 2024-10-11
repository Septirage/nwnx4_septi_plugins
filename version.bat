set "rootPath=%~dp0"
set "buildPath=%~1"
set "pluginName=%~2"

set "pluginPath=%buildPath%/%pluginName%"
set "versionPath=%buildPath%/Version.rc"
set "resPath=%buildPath%/Version.res"


"%rootPath%tool\ResourceHacker.exe" -open "%versionPath%" -save "%resPath%" -action compile
"%rootPath%tool\ResourceHacker.exe" -open "%pluginPath%" -save "%pluginPath%" -action addoverwrite -resource "%resPath%"
