#ifndef ONUT_H_INCLUDED
#define ONUT_H_INCLUDED

#include <string>
#include <vector>

namespace onut
{
    void quit();
}

extern std::vector<std::string> OArguments;

#define OQuit onut::quit

#endif
