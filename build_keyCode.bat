arduino-cli.exe compile -b arduino:avr:nano LightControl.ino -v -e  --build-property "build.extra_flags=\"-DAPP_keyCode=1\""
rmdir build\keyCode /q /S
rename build\arduino.avr.nano keyCode
