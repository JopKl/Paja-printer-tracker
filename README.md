# Paja-printer-tracker
3D printer monitoring software for Vilhonpaja

# Dependencies
[Arduino_JSON](https://github.com/arduino-libraries/Arduino_JSON)\
[HttpClient](https://reference.arduino.cc/reference/en/libraries/httpclient/)\
[WiFi](https://reference.arduino.cc/reference/en/libraries/wifi/)\
These libraries can all be downloaded from Arduino IDE library search

# Instructions

1. When deploy set DEBUG flag to 0
2. When booting up, the device will try to ping the REST API:
  >2.1 The device expects 201 response\
  >2.2 The blue LED will blink 3 times if it receceives a 201 response\
  >2.3 The device will retry every 3 second if it doesn't receive a 201 response\
3. The three buttons will report state messages to the server with POST request
  >3.1 The device expects 201 response\
  >3.2 The device sends a ping POST request to the server every 1 minute\
  >3.3 If it does not receive a 201, it will retry in 1 minute\
