#include <iostream>
#include <stdlib.h>

#include "servermanagement.h"

int main(int argc, char* argv[])
{

    if(argc == 1) {

        std::cout << "Use : load_balancer <input_file.txt>" << std::endl;
        return 1;

    }

    std::string fileName = argv[1];
    ServerManagement sm;
    if(sm.loadFile(fileName)) {

        sm.process();

    } else {

        std::cout << "Falha ao processar arquivo" << std::endl;

    }

    return 0;

}
