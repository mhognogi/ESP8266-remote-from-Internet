# ESP8266-remote-from-Internet

MCU WeMos ESP8266 command open close gate from Internet
MCU is connected to wifi and send HTTP POST request to PHP sever. 
main.cpp send POST an if response is "O" open the gate and if "C" close the gate.
The code is edited in Atom IDE with PlatformIO Package.
For install this IDE see: https://github.com/mhognogi/install_on_ubuntu/blob/master/platformio.sh

In PHP server put  
