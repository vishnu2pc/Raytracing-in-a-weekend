@echo off
@pushd bin
main.exe > image.ppm
Start ImageGlass image.ppm
@popd
