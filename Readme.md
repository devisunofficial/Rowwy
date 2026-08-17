# Rowwy
Rowwy is a hand-movement control car which uses Mecanum wheels.

### CAD Model:
It has two seperate bodies.

Controller : ![Complete Controller](Asset/Controller.png)

Rowwy : ![Complete Rowwy](Asset/Rowwy.png)

### PCB:
Here's my PCB! It was made in KiCad. 

Schematic : ![Complete Rowwy](Asset/Schematic.png)
PCB Footprint : ![Complete Rowwy](Asset/PCB.png)

### Firmware Overview:
Current version of Firmware is underdevelopment as it has not be implemeted in real life.
This car and controller uses C++ firmware and Arduino IDE for Flashing.

### Assembly Instructions :
1. Flash Firmware in XIAO ESP32 and Wroom Dev-board
2. Solder Motors, Devboard, Battery of car 
3. Solder Gyro-sensor, Flex-Sensor, Battery of controller
4. Check If Everything Works or not, also update the code depending on soldered pins
5. Glue togetther the controller enclosure and bracelet. Also Glue together battery and all circuits on Rowwy's base plate
6. Screw motor holder to base plate and enjoy the ride.

### BOM Table
|Name|Use|Quantity|Distributer|Total Cost(USD)|Link|
|-----|---|-------|-----------|-----|---|
|18650 battery |Power Source for Car|1|Robu|3|13.2|[Link](https://robu.in/product/samsung-inr18650-25r-2500mah-li-ion-battery/)|
|PCB|Main-Circuit|1|JLCPCB|32|--|
|XIAO ESP32|Controller|1|Robu|7.2|[Link](https://robu.in/product/seeed-studio-xiao-esp32c3-tiny-mcu-board-with-wi-fi-and-ble-battery-charge-supported-power-efficiency-and-rich-interface/)|
|Wroom ESP32 Dev Board|Reciever for Car|1|Robu|7.4|[Link](https://robu.in/product/smartelex-esp-wroom-32-wifi-bluetooth-networking-development-board-1-pcs/)|
|Mecanum Wheels|For Motion|1|Robu|5.3|[Link](https://robu.in/product/80mm-a-mecanum-wheel-compatible-with-6-7mm-coupling-pack-of-4-yellow/)|
|N20 geared Motor|Powering Motion|4|Robu|6.7|[Link](https://robu.in/product/n20-12v-600-rpm-micro-metal-gear-motor/)|
|Battery(1)|For Controller|1|Robu|2|[Link](https://robu.in/product/nova-303040-400mah-3-7v-micro-lipo-battery-pack/)|
|Flex Sensor|Handbreak Functionality|1|Robu|7.3|[Link](https://robu.in/product/flex-sensor-2-2-bend-sensor-hand-gesture-recognition/)|
|MPU6050|Calculating Handmovement|1|Robu|1.5|[Link](https://robu.in/product/mpu6500-gyroscope-accelerometer-digital-motion-processor-dmp-6-axis-motion-sensor-with-i2c-spi-interface/)|
|battery holder|To hold 18650 battery|1|Robu|0.3|[Link](https://robu.in/product/black-plastic-storage-box-case-holder-battery-3-x-18650-cell-box-without-cover/)|
|18650 charger|To charge 18650 battery|1|Amazon|2.1|[Link](https://www.amazon.in/TESTIN-ELECTRONICS-Battery-Charger-Charger-1Slot/dp/B0G4JR7FVW?source=ps-sl-shoppingads-lpcontext&psc=1&smid=A1LULBHI7A07S5)|