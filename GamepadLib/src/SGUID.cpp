#include "SGUID.hpp"
#include <sstream>

/* Converte um caracter hexadecimal em um valor de 4 bits */
static uint8_t nibble(char c) {
    if ((c >= '0') && (c <= '9')) {
        return (uint8_t)(c - '0');
    }

    if ((c >= 'A') && (c <= 'F')) {
        return (uint8_t)(c - 'A' + 0x0a);
    }

    if ((c >= 'a') && (c <= 'f')) {
        return (uint8_t)(c - 'a' + 0x0a);
    }

    /* Retornamos zero se ouver um caracter invalido, o que nunca deve acontecer em uma versão final */
    return 0;
}

SGUID::SGUID(const std::string &guid) {
    const char *str = guid.c_str();
    size_t len = guid.length()/4;
    uint8_t *p;
    size_t i;

    for (i = 0; i < len; i++) {
        /*
            Pega 2 bytes, formados por 4 nibles e preenche as variaveis
            Por padrão os dados estão armazenados no arquivo no formato big endain
        */

        this->data[i] = (nibble(str[(4*i)]) << 4) | (nibble(str[(4*i) + 1])) | (nibble(str[(4*i) + 2]) << 12) | (nibble(str[(4*i) + 3]) << 8);
    }
}


SGUIDInfo::SGUIDInfo(SGUID guid) {
    /* 
        CPU's intel são little endian, assim como a base de dados, logo não precisamos
        inverter o endain dos dados, porem, caso seja postado para outras plataformas
        sera necessario inverter a ordem dos bytes antes de atribuilos
    */

    this->bus = guid[0];
    this->vendor = guid[1];
    this->product = guid[2];
    this->version = guid[3];
}