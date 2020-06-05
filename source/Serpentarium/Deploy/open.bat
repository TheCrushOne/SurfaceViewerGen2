set ROOT=%~dp0..
set SOURCES_DIR=%ROOT%\Source

pushd "%SOURCES_DIR%\TaskManager" & call code . & popd

exit 0