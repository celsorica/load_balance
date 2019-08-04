#include "servermanagement.h"

ServerManagement::ServerManagement()
{

}

ServerManagement::~ServerManagement()
{

}

bool ServerManagement::loadFile(std::string fileName)
{

    //std::cout << "Enter the file name : ";
    m_fileName = fileName;

    std::string line;
    std::ifstream myfile (m_fileName.c_str());

    if (myfile.is_open()) {

        if(getline(myfile, line)) {
            m_ttask = static_cast<uint32_t>(std::atoi(line.c_str()));
        } else {
            return false;
        }

        if(getline(myfile, line)) {
            m_umax = static_cast<uint32_t>(atoi(line.c_str()));
        } else {
            return false;
        }

        while (getline (myfile,line)) {
            uint32_t value = static_cast<uint32_t>(atoi(line.c_str()));
            m_numbers.push_back(value);
        }
        myfile.close();
        return true;
    }

    return false;

}

void ServerManagement::process()
{
    for(uint32_t value : m_numbers) {
        //std::cout << value << std::endl;

        //Efetua o efeito de um tick
        tick();

        //verifica se há servidores disponiveis para alocar os clientes e os aloca
        if(value > 0) {

            createServer(value);

        }

        //Imprime o status dos servers
        toString();

    }

    while(waitLastTick()) {
        tick();
        toString();
    }

    std::cout << m_tick_counter << std::endl;
}

void ServerManagement::toString() const
{

    //std::cout << std::setw(4) << n_tick++ << "    " << value << "     [ ";
    std::string result = "";
    std::stringstream ss;
    if(m_listServers.size() > 0) {
        for(Server server : m_listServers) {

            ss << server.getCountClients() << ",";

        }

        result = ss.str().substr(0, ss.str().size() - 1);

    } else {
        ss << 0;
        result = ss.str();
    }

    std::cout << result << std::endl;

}

bool ServerManagement::waitLastTick()
{

    for(Server& s : m_listServers) {
        if(!s.isEmpty()) {

            return true;

        }

    }

    return false;

}

void ServerManagement::createServer(uint32_t numberClients)
{

    //Verifica quais Servers estão disponiveis e aloca alguns Clients neles,
    //diminuindo os Clients que restarao para ser alocados em novos Servers
    for(Server& server : m_listServers) {
        while(!server.isFull() && numberClients > 0) {
            server.addClient();
            --numberClients;
        }
    }

    //Verifica quantos Servers precisarao ser alocados
    uint32_t numServerInt = (numberClients / m_umax);
    uint32_t numServerMod = (numberClients % m_umax);


    //Se precisar criar mais...
    if(numServerInt > 0) {

        //Cria Servers para parte inteira da divisao
        for(uint32_t cont = 0; cont < numServerInt; ++cont) {

            Server s(m_ttask, m_umax, m_umax);
            m_listServers.push_back(s);

        }

    }

    if(numServerMod > 0) {

        //Cria Servers para a parte do resto da divisao caso precise
        if(numberClients > 0) {

            Server s(m_ttask, m_umax, numServerMod);
            m_listServers.push_back(s);

        }

    }

    return;

}

void ServerManagement::tick()
{

    for(Server& server : m_listServers) {
        server.tick();
        ++m_tick_counter;
    }

    m_listServers.erase(std::remove_if(m_listServers.begin(),
                                       m_listServers.end(),
                                       [=](Server s) { return (s.getCountClients() == 0); }),
                        m_listServers.end());

}
