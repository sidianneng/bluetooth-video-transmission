# ble video transmission

## 1 Introduction
In real applications, wireless video transmission is generally achieved using WIFI with a relatively large bandwidth. So is it possible to transmit video using Bluetooth with a very limited bandwidth? In fact, the insta 360 go3 launched in 2023 has given us the answer: this idea is feasible.
Inspired by the insta 360 go3, I started this project. This project is based on ESP32-S3. The resolution of the transmitted image is 240x240.

## 2 Feature 
### 2.1 Sender
* After the system starts, initialize the camera and start the ble gatt server
* After pressing the button, the indicator light starts to show the working status
* The indicator light has three colors: red, green and blue. Green means high power, blue means medium power, and red means low power.
* The indicator light flashes slowly when the Bluetooth transmitter is not connected, and flashes quickly when it is connected.
* After pressing the button once, the indicator light turns off
* When the transmitter is placed in the charging compartment, the indicator will light up to indicate that it is charging (it cannot be turned off by pressing a button at this time)

### 2.2 Receiver
* After the system starts, it will initialize the screen and automatically search for the ble gatt server
* After searching the server, it will automatically connect and receive image data
* Pressing the button will turn on the screen backlight and display the picture
* There is a battery icon in the upper right corner of the screen to show the power level
* After connecting the charging cable, the battery icon will show that it is charging.

## 3 Principle Analysis
* Bluetooth transmission is based on GATT's NOTIFY implementation, with a maximum transmission speed of about 90KB/s
* The camera uses OV2640, the image format is configured as JPEG, and the resolution is 240x240
* JPEG decoding library is esp_jpeg

### If you have any questions about this project, please feel free to discuss with me in the issue.
