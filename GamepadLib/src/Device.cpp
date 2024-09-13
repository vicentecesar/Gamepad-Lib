#include "Device.hpp"
#include <windows.h>
#include <hidsdi.h>
#include <hidpi.h>
#include <iostream>

static std::string wstringTostring(const std::wstring& wstr) {
    char *buffer;
    std::string s;

    buffer = (char *) malloc((wstr.length() + 1) * sizeof(char));
    wcstombs(buffer,wstr.c_str(), wstr.length() + 1);
    s = std::string(buffer);
    free(buffer);

    return s;
}

static std::wstring stringTowstring(const std::string& str) {
    wchar_t *buffer;
    std::wstring ws;

    buffer = (wchar_t *) malloc((str.length() + 1) * sizeof(wchar_t));
    mbstowcs(buffer, str.c_str(), str.length() + 1);
    ws = std::wstring(buffer);
    free(buffer);
    
    return ws;
}

bool Device::open() {
	/* Capacidades do dispositivo */
	HIDP_CAPS capabilities;
    /* Atributos do dispositivo */
	HIDD_ATTRIBUTES Attributes;
    /* Buffer para leitura de dados */
    wchar_t *buffer = nullptr;
	/* O tamanho do buffer deve ser menor ou igual a 4093 */
	ULONG size = 1024 * sizeof(wchar_t);

    /* Para leitura sincrona */
    this->handle = CreateFileA(this->path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	
    if (this->handle == INVALID_HANDLE_VALUE) {
		std::cout << "Erro ao abrir dispositivo ..." << GetLastError() << std::endl;
		return false;
	}

    /* Recupera usage e usage page */
	if (!HidD_GetPreparsedData(this->handle, (PHIDP_PREPARSED_DATA*)&this->preparsedData)) {
		std::cout << "Erro ao carregar parser para dispositivo USB..." << std::endl << std::endl;
		return false;
	}

	if (HidP_GetCaps((PHIDP_PREPARSED_DATA)this->preparsedData, &capabilities) == HIDP_STATUS_INVALID_PREPARSED_DATA) {
		std::cout << "Erro ao recuperar CAPS do dispositivo ..." << std::endl << std::endl;
		return false;
	}

	this->usagePage = (Device::UsagePage) capabilities.UsagePage;
	this->usage = (Device::Usage) capabilities.Usage;
	this->capabilities.numberOfButtonsCaps = capabilities.NumberInputButtonCaps;
    this->capabilities.numberOfValueCaps = capabilities.NumberInputValueCaps;
    
    this->dataBufferSize = capabilities.InputReportByteLength;
    this->dataBuffer = new char[this->dataBufferSize];
	
	//std::cout << "Buttons: " << this->capabilities.numberOfButtons << std::endl;

    buffer = (wchar_t *) calloc(size, sizeof(char));

    if(HidD_GetProductString(this->handle, (PVOID)buffer, size) == FALSE) {
        std::cout << "Windows error code: " << GetLastError() << std::endl;
        std::cout << "Erro ao recuperar a string produto do dispositivo ..." << std::endl << std::endl;
		this->product = "Undefined";
    } else {
    	this->product = wstringTostring(buffer);
	}
    
    
    if(HidD_GetManufacturerString(this->handle, (PVOID)buffer, size) == FALSE) {
        std::cout << "Windows error code: " << GetLastError() << std::endl;
        std::cout << "Erro ao recuperar a string fabricante do dispositivo ..." << std::endl << std::endl;
		this->manufacturer = "Undefined";
    } else {
    	this->manufacturer = wstringTostring(buffer);
	}

    
    free(buffer);

    /* Recupera dados para GUID do dispositivo */
    HidD_GetAttributes(this->handle, &Attributes);
	this->vid = Attributes.VendorID;
	this->pid = Attributes.ProductID;
	this->rev = Attributes.VersionNumber;

    return true;
}

void Device::close() {
    CloseHandle(this->handle);
}

bool Device::verify(Device::UsagePage usagePage, Device::Usage usage) {
       /*
    	Se a pagina de uso tem de ser ignorada, retornamos que a propriedade é a desejada, pois qualquer propriedade
    	deve ser valida. Se a pagina de uso for ignorada, não precisamos verificar o uso, pois não pode-se arfirmar
    	nada sobre uso sem saber sua pagina.
    */
    if (usagePage == Device::UsagePage::Ignore) {
    	return true;
    }
    /*
    	Se o uso dever ser ignorado (neste ponto ja sabemos que a pagina não deve), devemos procurar por qualquer
    	dispositivo pertencente a esta pagina
    */
    if (usage == Device::Usage::Ignore) {
    	return this->usagePage == usagePage;
    } else {
    	/*
    		Pequisar por dispositivo em uma pagina e uso pre definidos
    		O uso é extraido apenas dentro desse if, pois os if's anteriores ignoram o uso
    	*/
    	return (this->usagePage == usagePage) && (this->usage == usage);
    }
}

#define DUALSENSE_LED_REPORT_ID 0x31
#define DUALSENSE_LED_BUFFER_SIZE 64

//static int getAnalogicID() {
//    Eixo X : Geralmente mapeado para Usage 0x30 (Eixo X).
//        Eixo Y : Geralmente mapeado para Usage 0x31 (Eixo Y).
//}

void Device::update() {
    DWORD bytesRead;
    if (!ReadFile(this->handle, this->dataBuffer, this->dataBufferSize, &bytesRead, NULL)) {
        std::cerr << "Failed to read from device" << std::endl;
        DWORD error = GetLastError();
        std::cerr << "Error code: " << error << std::endl;
        return;
    }

    for (int i = 0; i < this->dataBufferSize; i++) {
        std::cout << static_cast<int>(((uint8_t *)this->dataBuffer)[i]) << " ";
    }

    std::cout << std::endl;
    return;

    HIDP_BUTTON_CAPS *buttonCaps = new HIDP_BUTTON_CAPS[this->capabilities.numberOfButtonsCaps];
    USHORT buttonCapsLength = this->capabilities.numberOfButtonsCaps;
    if (HidP_GetButtonCaps(HidP_Input, buttonCaps, &buttonCapsLength, (PHIDP_PREPARSED_DATA)this->preparsedData) != HIDP_STATUS_SUCCESS) {
        std::cerr << "Failed to get button caps" << std::endl;
    }

    USAGE usage[64];
    ULONG usageLength;
    for (USHORT i = 0; i < buttonCapsLength; ++i) {
        usageLength = 64;
        if (HidP_GetUsages(HidP_Input, buttonCaps[i].UsagePage, 0, usage, &usageLength, (PHIDP_PREPARSED_DATA)this->preparsedData, (PCHAR)this->dataBuffer, this->dataBufferSize) != HIDP_STATUS_SUCCESS) {
            std::cerr << "Failed to get usages" << std::endl;
        }

        for (ULONG j = 0; j < usageLength; ++j) {
            std::cout << "Button " << usage[j] << " is pressed" << std::endl;
        }
    }

    HIDP_VALUE_CAPS *valueCaps = new HIDP_VALUE_CAPS[this->capabilities.numberOfValueCaps];
    USHORT valueCapsLength = this->capabilities.numberOfValueCaps;
    if (HidP_GetValueCaps(HidP_Input, valueCaps, &valueCapsLength, (PHIDP_PREPARSED_DATA)this->preparsedData) != HIDP_STATUS_SUCCESS) {
        std::cerr << "Failed to get value caps" << std::endl;
    }

    for (USHORT i = 0; i < valueCapsLength; ++i) {
        ULONG value;
        if (HidP_GetUsageValue(HidP_Input, valueCaps[i].UsagePage, 0, valueCaps[i].NotRange.Usage, &value, (PHIDP_PREPARSED_DATA)this->preparsedData, (PCHAR)this->dataBuffer, this->dataBufferSize) != HIDP_STATUS_SUCCESS) {
            std::cerr << "Failed to get usages" << std::endl;
            continue;
        }

        float normalizedValue = static_cast<float>(value) / 65535.0f;
        std::cout << "a" << (valueCaps[i].NotRange.Usage - 0x30) << ": " << (2 * normalizedValue) - 1 << " ";

    }

    std::cout << std::endl;

    delete[] buttonCaps;
}