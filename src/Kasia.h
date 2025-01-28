
#ifndef Kasia_H_INCLUDED
#define Kasia_H_INCLUDED

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <stdint.h>

#define V1_SERVICE_ID "94619026-7220-4669-9d2a-c9f1ab0b8875"
#define V1_CHARAC_ID "c97da6f9-90aa-4f48-abee-6daec01ae2ca"

#define KASIA_DEVICE_PREFIX "Kasia"
#define DEFAULT_KASIA_DEVICE_NAME "KasiaDevice"

#define KASIA_DEVICE_TYPE_GENERIC 0
#define KASIA_DEVICE_TYPE_ESP32_DEVKIT_V1 1

class Kasia 
{
public:
    Kasia();
    void start(uint8_t deviceType);
    void start(const char *deviceName);
    void start(uint8_t deviceType, const char *deviceName);
    void start();

protected:
    static uint8_t _deviceType;
    static const char *_deviceName;
    void startServer();
};

extern Kasia kasia;

#endif /* Kasia_H_INCLUDED */