#include <Input.hpp>

#include <iostream>
#include <cstdio>
#include <cstdint>

static uint16_t crc16_for_byte(uint8_t r)
{
    uint16_t crc = 0;
    int i;
    for (i = 0; i < 8; ++i) {
        crc = ((crc ^ r) & 1 ? 0xA001 : 0) ^ crc >> 1;
        r >>= 1;
    }
    return crc;
}

uint16_t SDL_crc16(uint16_t crc, const void *data, size_t len)
{
    /* As an optimization we can precalculate a 256 entry table for each byte */
    size_t i;
    for (i = 0; i < len; ++i) {
        crc = crc16_for_byte((uint8_t)crc ^ ((const uint8_t *)data)[i]) ^ crc >> 8;
    }
    return crc;
}

int main() {
    std::vector<Device> devices;

    devices = Input::list<Device>();

    for(int i = 0; i < devices.size(); i++) {
        
        /*
            TODO: Por algum motivo abrir o dispostivo de mouse somente leitura apresenta falhar no GCC, mas não no visual studio
        */
        if(devices[i].open()) {
            if(
                devices[i].verify(Device::UsagePage::GenericDesktopControls, Device::Usage::Joystick) ||
                devices[i].verify(Device::UsagePage::GenericDesktopControls, Device::Usage::GamePad)) {
                std::cout << "---------------------------------------------" << std::endl << std::endl;
                std::cout << devices[i];
                std::cout << std::endl;
                std::cout << "---------------------------------------------" << std::endl;

                while (true) {
                    devices[i].update();
                }
            }
            devices[i].close();
        }
    }

    uint16_t crc = 0;
    printf("%.8X", SDL_crc16(crc, "Controller (XBOX 360 For Windows)", strlen("Controller (XBOX 360 For Windows)")));

    return 0;
}