# CycleSense
Virtual game input device that gives the user control via their stationary bike.<br><br>


## **ble_central**
PC App, GoLang/JavaScript, the "client"
  - Connects to and reads sensor data from peripheral
  - Virtualizes Xbox 360 and keyboard inputs
  - Runs webserver that displays sensor data<br><br>
  
  
## **ble_peripheral**
Arduino device, C++, the "server"
  - Advertises itself over Bluetooth Low Energy
  - Reads and stores data from various sensors
  - Provides subscribable BLE services for reading sensor data on update
