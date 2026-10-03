#ifndef SIMULADOR_ESCALONAMENTO_CONFIGPARSER_H
#define SIMULADOR_ESCALONAMENTO_CONFIGPARSER_H
#include <string>

#include "Config.h"


class ConfigParser
{
public:
    Config parse(std::string& filePath);
};


#endif //SIMULADOR_ESCALONAMENTO_CONFIGPARSER_H