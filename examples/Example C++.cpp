#include <SGUID.hpp>
#include <Device.hpp>

#include <iostream>

int main() {
    SGUID guid(DeviceBus::USB, 0x0079, 0x0006, 0x0107);
    SGUID mguid("0100790006000701");
    SGUIDInfo guidInfo(guid);
    SGUIDInfo mguidInfo(mguid);
    Device device("\\\\?\\hid#vid_0079&pid_0006#7&2fb07e08&1&0000#{4d1e55b2-f16f-11cf-88cb-001111000030}");

    device.open();

    std::cout << guidInfo;
    std::cout << "SGUID: " << guid.toString() << std::endl << std::endl;
    std::cout << mguidInfo;
    std::cout << "SGUID: " << mguid.toString() << std::endl;

    device.close();

    return 0;
}