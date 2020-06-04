#include <iostream>
#include "BTLightConfig.h"


int main(int argc, char **argv)
{
    if (argc < 2)
    {
        // report version
        std::cout << argv[0] << "Hello Sayer Version "
                  << BTLight_VERSION_MAJOR << "."
                  << BTLight_VERSION_MINOR << std::endl;
        std::cout << "Usage: " << argv[0] << " name" << std::endl;
        return 1;
    }

    std::cout << "Hello " << argv[1] << "!!!" << std::endl;
    return 0;
}