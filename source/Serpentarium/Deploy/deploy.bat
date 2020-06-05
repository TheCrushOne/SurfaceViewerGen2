@echo off

set ROOT=%~dp0..
set SOURCES_DIR=%ROOT%\Source

call :check_mklink_permissions

set PIP_CONFIG_FILE=%ROOT%\Deploy\pip.ini

call :deploy_api TaskManager
if errorlevel 1 exit 1

echo Deploy finished
exit 0


:check_mklink_permissions
echo Checking make link permissions...
if exist "%SOURCES_DIR%\mklink_test" rmdir /s /q "%SOURCES_DIR%\mklink_test"
mklink /d "%SOURCES_DIR%\mklink_test" "%SOURCES_DIR%\Modules"
if errorlevel 1 (
   echo Running as Administrator is required for directory link creating!
   pause
   exit 1
)
rmdir /s /q "%SOURCES_DIR%\mklink_test"
exit /b


:deploy_api
echo Deploying %1...
pushd "%SOURCES_DIR%\%1"

echo Environment creation...
py -m venv env
call env\Scripts\activate
py -m pip install --upgrade pip
:: API's own modules
py -m pip install -r requirements.txt --pre --no-cache-dir
py manage.py migrate
call deactivate

:: Open VS Code
call code .

popd
exit /b
