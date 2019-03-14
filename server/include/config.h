#ifndef CONFIG_H_
#define CONFIG_H_

#include <iostream>
#include <fstream>

class Config
{
  public:
    // Error: Return -1
    // Success: Return 0;
    int Load(std::string config_path = "config");
    std::string GetIP();
    unsigned int GetPort();

  private:
    std::string ip;
    unsigned int port;
};

#endif // CONFIG_H_