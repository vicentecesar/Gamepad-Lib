#pragma once

/*
	Para sistemas windows será usada a API RawInput em conjunto com a API HID do windows para capturar entradas de
	usuario. Devido essa API ser recente (hoje em dia não é muito recente, mas continua com uma documentação ruim)
    a documentação de seu uso é mais escaça do que as API's legado do windows.
	Para ajudar os desenvolvedores a implementar suporte a novos dispositivos de jogos, seguiremos com alguma
	documentção.

	Primeiramente, qualquer dispositivo usado deve ser cadastrado usando um RAWINPUTDEVICE. Seus atributos são:

	usUsagePage e usUsage: Tipo do dispositivo usado. De acordo com a documentação microsoft podem ser:
	https://docs.microsoft.com/en-us/windows-hardware/drivers/hid/hid-clients-supported-in-windows

	Os conseitos de pagina usada e uso não são exclusivos da microsoft, fazem parte de um protocolo USB para HID.
	Isso pode ser encontrado em:
	https://www.usb.org/document-library/hid-usage-tables-112 (Tabela de paginas e usos)
	https://www.usb.org/hid (Pagina inicial da documentação completa sobre HID)
*/

#include "SGUID.hpp"

#include <windows.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

class Capabilities {
	public:
		int numberOfButtonsCaps;
		int numberOfValueCaps;
};

class Mapping {
	private:
		std::vector<std::string> buttonName;
		std::vector<bool> buttonState;
		std::vector<std::string> analogicName;
		std::vector<float> analogicState;
		std::unordered_map<std::string, bool> buttonStateMap;
		std::unordered_map<std::string, float> abalogicStateMap;
};

class Device {
    public:
		/* Mapeamento será dividido entre USB e bluetooh */
		enum Bus : uint16_t{
			USB = 0x0003,
			/* Mapeamento bluetooth ainda não suportado */
			Bluetooth = 0x0005
		};

        enum class Type : uint32_t {
			All = 0x00,
			Joystick = 0x01,
			Mouse = 0x02,
			Keyboard = 0x04,
			Gamepad = 0x08
		};

		/* Outras paginas de uso não são listadas aqui pois não são usadas pelas engine */
		enum class UsagePage : uint16_t {
			Ignore = 0xFFFF,
			GenericDesktopControls = 1
		};

		/* Apenas Usage para GenericDesktopControls são listados aqui, ja que são os unicos usados */
		enum class Usage : uint16_t {
			Ignore = 0xFFFF,
			Mouse = 2,
			Joystick = 4,
			GamePad = 5,
			Keyboard = 6
		};

		/* Special Purpouse hardware ID */
		enum class SPHID {
			All = 0,
			SystemGame = 1,
			SystemMouse = 2,
			SystemKeyboard = 3
		};

    public:
        /* Variaveis dependentes de sistema */
		HANDLE handle;
		/* Dados do dispositivo para analize */
		void *preparsedData;

		/* Variaveis independente de sistema */
		std::string path;
		std::string product;
		std::string  manufacturer;
		uint16_t vid;
		uint16_t pid;
		uint16_t rev;
		uint16_t bus;
		Device::UsagePage usagePage;
		Device::Usage usage;

		void *dataBuffer;
		int dataBufferSize;

		Capabilities capabilities;
		Mapping mapping;

    public:
        Device(uint16_t bus, const char *path) : bus(bus), path(path), capabilities({}), preparsedData(NULL) {
		}
        /* Abre o dispositivo, preenchendo os campos necessarios */
        bool open();
        /* Fecha o dispositivo ao terminar seu uso */
        void close();
        /* Verifica se o dispositivo tem uma determinada categoria */
        bool verify(Device::UsagePage usagePage, Device::Usage usage);
		/* Atualiza informações do dispositivo */
		void update();

        friend std::ostream & operator << (std::ostream &os, Device &device); 
};

/* Operador usado apenas para facilitar a depuração */
inline std::ostream & operator << (std::ostream &os, Device &device) {
	os << "Path:         " << device.path << std::endl;
	os << "Bus:         " << device.bus << std::endl;
	os << "Product:      " << device.product << std::endl;
	os << "Manufacturer: " << device.manufacturer << std::endl;
	os << "Usage Page:   " << (uint16_t) device.usagePage << std::endl;
	os << "Usage:        " << (uint16_t) device.usage << std::endl;
	os << "SGUID:         " << SGUID(device.bus, device.vid, device.pid, device.rev) << std::endl;

	return os;
}