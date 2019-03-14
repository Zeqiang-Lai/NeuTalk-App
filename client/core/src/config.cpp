#include "config.h"

int Config::Load(std::string config_path)
{
    std::ifstream fin;
    fin.open(config_path.c_str());
    if(!fin.is_open())
        return -1;
    fin >> ip;
    fin >> port;
    fin.close();
    return 0;
}

std::string Config::GetIP()
{
    return ip;
}

unsigned int Config::GetPort()
{
    return port;
}