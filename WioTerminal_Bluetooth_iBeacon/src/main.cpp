#include "rpcBLEDevice.h"
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    char uuid[60];
    BLEAddress addr = advertisedDevice.getAddress();
    int rssi = advertisedDevice.getRSSI();
    uint8_t* data = advertisedDevice.getManufacturerData();

    Serial.printf("data=%d,%d\n",data[0],data[1]);


    if( (data[0] == 0x02) && (data[1] == 0x15)) {
      sprintf(uuid,"%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X"
        ,data[2],data[3],data[4],data[5],data[6],data[7],data[8],data[9],data[10],data[11]
        ,data[12],data[13],data[14],data[15],data[16],data[17]);
      int major = (int)(((data[18]&0xff) << 8) + (data[19] & 0xff));
      int minor = (int)(((data[20]&0xff) << 8) + (data[21] & 0xff));
      signed char power = (signed char)(data[22]&0xff);
      Serial.printf("uuid=%s\n",uuid);
      Serial.printf("addr=%s rssi=%d uuid=%s\n",addr.toString().c_str(),rssi,uuid);
      Serial.printf("major=%d,minor=%d,power=%d\n",major,minor,power);
    }
  }
};


void setup() {
  Serial.begin(115200);
  while(!Serial){};
  delay(2000);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(), true);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5);
} // End of setup.

void loop(){
}