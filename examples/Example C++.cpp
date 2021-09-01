#include <Input.hpp>

#include <iostream>

int main() {
    std::vector<Device> devices;

    devices = Input::list<Device>();

    for(int i = 0; i < devices.size(); i++) {
        
        /*
            TODO: Por algum motivo abrir o dispostivo de mouse somente leitura apresenta falhar no GCC, mas não no visual studio
        */
        std::cout << "---------------------------------------------" << std::endl << std::endl;
        if(devices[i].open()) {
            std::cout << devices[i];
            std::cout << std::endl;
            devices[i].close();
        }
    }

    if(devices.size())
        std::cout << "---------------------------------------------" << std::endl;

    return 0;
}