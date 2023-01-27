<h3 align="center">
  Virtual game input device that gives the user control via their stationary bike
</h3>
<p align="center">
  Supports connectivity over Bluetooth and Wifi
</p>

<p>&nbsp;</p>

<p align="center">
  <a>
    <img src="https://i.imgur.com/KbC97vj.png"/>
  </a>
</p>

## **app**
PC App, GoLang/JavaScript
  - Connects to and reads sensor data from peripheral
  - Virtualizes Xbox 360 and keyboard inputs
  - Runs webserver that displays sensor data<br><br>
  
## **device**
Arduino device, C++
  - Advertises itself over Bluetooth Low Energy
  - Reads and stores data from various sensors
  - Provides subscribable BLE services for reading sensor data on update
