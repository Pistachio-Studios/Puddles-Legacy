@echo off

cd %UserProfile%

git clone https://github.com/microsoft/vcpkg.git

cd vcpkg && call ./bootstrap-vcpkg.bat

echo momento exportar archivo
cd %~dp0
echo %cd%
echo %UserProfile%\vcpkg > vcpkg-path.txt