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

#include <windows.h>
#include <string>

class Device {
    public:
        enum class Type : uint32_t {
			All = 0x00,
			Joystick = 0x01,
			Mouse = 0x02,
			Keyboard = 0x04,
			Gamepad = 0x08
		};

		/* Outras paginas de uso não são listadas aqui pois não são usadas pelas engine */
		enum class UsagePage : int32_t {
			Ignore = -1,
			GenericDesktopControls = 1
		};

		/* Apenas Usage para GenericDesktopControls são listados aqui, ja que são os unicos usados */
		enum class Usage {
			Ignore = -1,
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

		/* Variaveis independente de sistema */
		std::string path;
		std::string product;
		std::string  manufacturer;
		unsigned int vid;
		unsigned int pid;
		unsigned int rev;
		Device::UsagePage usagePage;
		Device::Usage usage;

    public:
        Device(const char *path) : path(path) {}
        /* Abre o dispositivo, preenchendo os campos necessarios */
        bool open();
        /* Fecha o dispositivo ao terminar seu uso */
        void close();
        /* Verifica se o dispositivo tem uma determinada categoria */
        bool verify(Device::UsagePage usagePage, Device::Usage usage);
};