SET pf=%ProgramFiles%
if DEFINED ProgramFiles(x86) SET pf=%ProgramFiles(x86)%

set appcmd="%pf%\iis express\appcmd.exe"
set iisexpress="%pf%\iis express\iisexpress.exe"

cd > tmpFile
set /p currentdir= < tmpFile
del tmpFile

where php-cgi.exe > tmpFile
set /p phprt= < tmpFile
del tmpFile
if DEFINED phprt goto setup_iis
SET phprt=%pf%\PHP\v5.3\php-cgi.exe

:setup_iis

copy "%pf%\iis express\config\templates\PersonalWebServer\applicationhost.config" "%currentdir%\apphost.config"

%appcmd% delete site "WebSite1" /apphostconfig:"%currentdir%\apphost.config"
%appcmd% add site /name:WebSite1 /physicalPath:"%currentdir%" /bindings:http/*:8080: /apphostconfig:"%currentdir%\apphost.config"
%appcmd% set config /section:system.webServer/fastCgi "/+[fullPath='%phprt%']" "/apphostconfig:%currentdir%\apphost.config"
%appcmd% set config /section:system.webServer/handlers "/+[name='PHP-FastCGI',path='*.php',modules='FastCgiModule',verb='*', scriptProcessor='%phprt%',resourceType='Either']" "/apphostconfig:%currentdir%\apphost.config"
%appcmd% set config /section:system.webServer/defaultDocument "/+files.[value='index.php']" "/apphostconfig:%currentdir%\apphost.config"

%iisexpress% /site:WebSite1 /config:"%currentdir%\apphost.config"
