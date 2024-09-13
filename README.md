# GamepadLib
Biblioteca de codigo aberto para mapeamento e uso de controles UBS. Altualmente apenas para Windows, com suporte as linguagens C++ e logo logo em C. Seu uso não depende de bilbiotecas externas, fazendo com que seja possivel usa-la facilmente com qualquer biblioteca de desenvolvimento de games. Se você ainda não sabe desenvolver games para fazer o uso dessa biblioteca em breve terei outros projetos abertos para desenvolvimento de games e game engines.

Para compilar o codigo de exemplo, basta seguir com o terminar até a pasta examples e executar o comando "mingw32-make c++", com o compilador GCC para windows intalado. O codigo em questão ainda não é compativel com Visual Studio, mas será em breve. Executando o exemplo atual, apenas será possivel ver uma lista de controles USB conectados ao seus computador, mas em breve mais atualizações serão lançadas para que ler os dados dos Gamepad's conectados.

Gerar GUID do banco de dados:

https://github.com/mdqinc/SDL_GameControllerDB/tree/master
https://github.com/libsdl-org/SDL/blob/cd9c25e80065dce4dd5a20b26c858d03e2b37eed/src/joystick/SDL_joystick.c#L2709
https://github.com/libsdl-org/SDL/blob/cd9c25e80065dce4dd5a20b26c858d03e2b37eed/src/joystick/android/SDL_sysjoystick.c#L341