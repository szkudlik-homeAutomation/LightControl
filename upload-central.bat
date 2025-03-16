set PORT=com6
arduino-cli.exe upload -b arduino:avr:mega:cpu=atmega2560 -v -p %PORT% -i build/central/LightControl.ino.with_bootloader.hex
arduino-cli.exe monitor -c baudrate=115200 -p %PORT%
