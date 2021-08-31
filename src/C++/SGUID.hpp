#pragma once

/*
    SGUID é uma estrutura de dados usada para identificar o controle USB. A estrura é composta
    por id do fornecedor, id do produto, versão e barramento.
*/

#include <string>
#include <iostream>
#include <iomanip>
#include <stdint.h>

/* Mapeamento será dividido entre USB e bluetooh */
enum DeviceBus : uint16_t{
    USB = 0x0001,
    /* Mapeamento bluetooth ainda não suportado */
    Bluetooth = 0x0002
};

class SGUID {
    private:
        uint16_t data[4];
    
    public:
        SGUID(): data{} {}
        SGUID(uint16_t bus, uint16_t vendor, uint16_t product, uint16_t version)
        : data {bus, vendor, product, version} {}
        SGUID(const std::string &guid);
        ~SGUID() {}

        std::string toString();

        uint16_t operator [] (unsigned int index) {
            return data[index];
        }
};

/* Operador usado apenas para facilitar a depuração */
inline std::ostream & operator << (std::ostream &os, SGUID &guid) {
    os << guid.toString();

    return os;
}

class SGUIDInfo {
    private:
        unsigned int bus;
        unsigned int vendor;
        unsigned int product;
        unsigned int version;

    public:
        SGUIDInfo() : bus(0), vendor(0), product(0), version(0) {}
        SGUIDInfo(SGUID guid);
        ~SGUIDInfo() {}

        friend std::ostream & operator << (std::ostream &os, SGUIDInfo &guidInfo); 
};

/* Operador usado apenas para facilitar a depuração */
inline std::ostream & operator << (std::ostream &os, SGUIDInfo &guidInfo) {
    os << "Bus:     0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << guidInfo.bus     << "\n";
    os << "Vendor:  0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << guidInfo.vendor  << "\n";
    os << "product: 0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << guidInfo.product << "\n";
    os << "version: 0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << guidInfo.version << "\n";

    return os;
}