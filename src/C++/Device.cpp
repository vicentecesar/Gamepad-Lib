#include "Device.hpp"

#include <ddk/hidpi.h>
#include <ddk/hidsdi.h>
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
    /* Dados do dispositivo para analize */
	PHIDP_PREPARSED_DATA preparsedData = NULL;
	/* Capacidades do dispositivo */
	HIDP_CAPS capabilities;
    /* Atributos do dispositivo */
	HIDD_ATTRIBUTES Attributes;
    /* Buffer para leitura de dados */
    wchar_t *buffer = nullptr;
	/* O tamanho do buffer deve ser menor ou igual a 4093 */
	ULONG size = 1024 * sizeof(wchar_t);

    /* Para leitura sincrona */
	this->handle = CreateFileA(this->path.c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, NULL);
	
    if (this->handle == INVALID_HANDLE_VALUE) {
		std::cout << "Erro ao abrir dispositivo ..." << std::endl << std::endl;
		return false;
	}

    /* Recupera usage e usage page */
	if (!HidD_GetPreparsedData(this->handle, &preparsedData)) {
		std::cout << "Erro ao carregar parser para dispositivo USB..." << std::endl << std::endl;
		return false;
	}

	if (HidP_GetCaps(preparsedData, &capabilities) == HIDP_STATUS_INVALID_PREPARSED_DATA) {
		std::cout << "Erro ao recuperar CAPS do dispositivo ..." << std::endl << std::endl;
		return false;
	}

	this->usagePage = (Device::UsagePage) capabilities.UsagePage;
	this->usage = (Device::Usage) capabilities.Usage;

    buffer = (wchar_t *) calloc(size, sizeof(char));

    if(HidD_GetProductString(this->handle, (PVOID)buffer, size) == FALSE) {
        std::cout << "Windows error code: " << GetLastError() << std::endl;
        std::cout << "Erro ao recuperar a string produto do dispositivo ..." << std::endl << std::endl;
		return false;
    }
    
    this->product = wstringTostring(buffer);
    
    if(HidD_GetManufacturerString(this->handle, (PVOID)buffer, size) == FALSE) {
        std::cout << "Windows error code: " << GetLastError() << std::endl;
        std::cout << "Erro ao recuperar a string fabricante do dispositivo ..." << std::endl << std::endl;
		return false;
    }

    this->manufacturer = wstringTostring(buffer);
    
    free(buffer);

    /* Recupera dados para GUID do dispositivo */
    HidD_GetAttributes(this->handle, &Attributes);
	this->vid = Attributes.VendorID;
	this->pid = Attributes.ProductID;
	this->rev = Attributes.VersionNumber;
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