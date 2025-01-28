#include "Kasia.h"
#include <string>

class BLEServerCB : public BLEServerCallbacks
{
    void onConnect(BLEServer *server, esp_ble_gatts_cb_param_t *param) override
    {
        // Something connected, restarting advertising
        BLEDevice::startAdvertising();
    }
} serverCallbacks;

class KasiaCharacteristicCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        String value = pCharacteristic->getValue();

        if (value.length() == 1)
        {
            uint8_t request = value.charAt(0);
            uint8_t toggleValue = request & (1 << 7);

            if (toggleValue == 128) // 0x80 aka (1 << 7)
            {
                request ^= (1 << 7);
                toggleValue = 1;
            }

            pinMode(request, OUTPUT);
            digitalWrite(request, toggleValue);
        }
        else
        {
            Serial.println("Kasia: Unexpected data length");
        }
    }
} kasiaCharacCallbacks;

const char *Kasia::_deviceName = DEFAULT_KASIA_DEVICE_NAME;
uint8_t Kasia::_deviceType = KASIA_DEVICE_TYPE_GENERIC;

Kasia::Kasia()
{
}

/**
 * Start Kasia framework while specifying deviceType
 * @param deviceType
 * Device type
 */
void Kasia::start(uint8_t deviceType)
{
    _deviceType = deviceType;
    startServer();
}

/**
 * Start Kasia framework while specifying deviceName
 * 'Kasia' prefix is added to every device name automatically
 * @param deviceName
 * Unique Id or name of the device
 */
void Kasia::start(const char *deviceName)
{
    std::string name = KASIA_DEVICE_PREFIX;
    name += deviceName;
    _deviceName = name.c_str();
    startServer();
}

/**
 * Start Kasia framework while specifying deviceType and deviceName
 * 'Kasia' prefix is added to every device name automatically
 * @param deviceType
 * Device type
 * @param deviceName
 * Unique Id or name of the device
 *
 */
void Kasia::start(uint8_t deviceType, const char *deviceName)
{
    _deviceType = deviceType;
    std::string name = KASIA_DEVICE_PREFIX;
    name += deviceName;
    _deviceName = name.c_str();
    startServer();
}

/**
 * Start Kasia framework without specifying any configuration
 * This will use the default device name and generic device type
 */
void Kasia::start()
{
    startServer();
}

/**
 * Start ble server
 */
void Kasia::startServer()
{
    BLEDevice::init(_deviceName);
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(&serverCallbacks);
    BLEService *pService = pServer->createService(V1_SERVICE_ID);
    BLECharacteristic *pCharacteristic = pService->createCharacteristic(
        V1_CHARAC_ID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);
    pCharacteristic->setCallbacks(&kasiaCharacCallbacks);
    pCharacteristic->setValue(&_deviceType, (size_t)1);
    pService->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(V1_SERVICE_ID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
    Serial.println("Kasia Device is active :)");
}

Kasia kasia;