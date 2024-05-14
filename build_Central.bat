arduino-cli.exe compile -b arduino:avr:mega:cpu=atmega2560 LightControl.ino -v -e  --build-property "build.extra_flags=\"-DAPP_Central=1\""
rmdir build\central /q /S
rename build\arduino.avr.mega central

