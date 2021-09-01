#include "Input.hpp"

#include <ddk/hidsdi.h>
#include <setupapi.h>
#include <iostream>
#include <stdio.h>

template <>
std::vector<Device> Input::list<Device>() {
    std::vector<Device> devices;
    GUID guidForHIDDevices;
    HANDLE processHeap;
    /* Manipulador para as informações do dispositivo */
    HDEVINFO hDevInfo;
    /* Interface de comunicação com um dispositivo listado */
    SP_DEVICE_INTERFACE_DATA diviceInterfaceData;
    /* Buffer para informações detalhadas da interface de comunicação com o dispositivo */
    PSP_DEVICE_INTERFACE_DETAIL_DATA_A diviceInterfaceDetailData;
    /* informações sobre o dispositivos recuperado */
    SP_DEVINFO_DATA deviceInfoData;
    /* Indice do dispositivo analizado na lista */
    int diviceIndex = 0;

    processHeap = GetProcessHeap();
    if (processHeap == nullptr) {
        return devices;
    }

    HidD_GetHidGuid(&guidForHIDDevices);

    /* Realiza listagem dos dispositivos do PC */
    hDevInfo = SetupDiGetClassDevs(&guidForHIDDevices, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

    /* Prenche o membro cbSize das estruturas como indicado pela documentação da API */
    diviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    while (SetupDiEnumDeviceInterfaces(hDevInfo, NULL, &guidForHIDDevices, diviceIndex++, &diviceInterfaceData)) {
        /* Armazena tamanho requerido pelo buffer para a informação */
        DWORD required = 0;

        SetupDiGetDeviceInterfaceDetail(hDevInfo, &diviceInterfaceData, NULL, 0, &required, &deviceInfoData);

        if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
            std::cout << "Erro ao obter tamnho do buffer para interface detalhada de dispositivos ...";
            /* Vai para o proximo dispositivo da lista em casa do erro */
            continue;
        }

        /* Aloca memoria para a interface do dispositivo */
        diviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)HeapAlloc(processHeap, 0, required);
        if (diviceInterfaceDetailData == nullptr) {
            std::cout << "Erro ao alocar buffer para interface detalhada de comunicação com dispositivos ...";
            /* Vai para o proximo dispositivo da lista em casa do erro */
            continue;
        }

        /* Preenche o campo cbSize como solicitado na documentação */
        diviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

        /* Recupera informações detalhadas da interface de dispositivo */
        if (!SetupDiGetDeviceInterfaceDetailA(hDevInfo, &diviceInterfaceData, diviceInterfaceDetailData, required, NULL, &deviceInfoData)) {
            std::cout << "Erro ao recuperar informações detalhadas do dispositivo ...";
            /* Limpa a memoria se pular para proxima iteração */
            HeapFree(processHeap, 0, diviceInterfaceDetailData);
            /* Vai para o proximo dispositivo da lista em casa do erro */
            continue;
        }

        devices.push_back(Device(Device::Bus::USB, diviceInterfaceDetailData->DevicePath));

        HeapFree(processHeap, 0, diviceInterfaceDetailData);
    }

    return devices;
}