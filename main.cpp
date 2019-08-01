#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <iomanip>
#include <sstream>

typedef struct {
    uint32_t tick_restantes = 0;
} Clients;

class Server {
public:
    explicit Server(uint32_t max_tick, uint32_t max_users, uint32_t countUsed = 0);
    void addClient();
    bool isFull() const;
    bool isEmpty() const;
    void tick();
    size_t getCountClients() const;
    std::string toString() const;

private:

    std::vector<Clients> m_clients;
    uint32_t m_max_tick = 0;        //ttask
    uint32_t m_max_users = 0;       //umax
    uint32_t m_count_users = 0;     //total users connected
    uint32_t m_tick_counter_vm = 0; //total ticks VM
};


Server::Server(uint32_t max_tick, uint32_t max_users, uint32_t countUsed) :
    m_max_tick(max_tick),
    m_max_users(max_users),
    m_count_users(0)
{
    for(uint32_t count = 0; count < countUsed; ++count) {
        addClient();
    }
}

void Server::addClient() {
    Clients cl;
    cl.tick_restantes = m_max_tick;
    m_clients.push_back(cl);
    ++m_count_users;
}

bool Server::isFull() const {
    return (m_count_users == m_max_users);
}

bool Server::isEmpty() const
{
    return (m_count_users == 0);
}

void Server::tick()
{

    ++m_tick_counter_vm;

    //Processa o "tick" no servidor
    for(Clients& client : m_clients) {
        --client.tick_restantes;
    }

    m_clients.erase(std::remove_if(m_clients.begin(),
                                   m_clients.end(),
                                   [=](Clients c) {
                                                    if(c.tick_restantes == 0) {
                                                         m_count_users--;
                                                         return true;
                                                     } else {
                                                         return false;
                                                     }
                                                  }
                                  ),
                                  m_clients.end());

}

size_t Server::getCountClients() const
{
    return m_count_users;//m_clients.size();
}

std::string Server::toString() const
{
    std::stringstream ss;
    for(const Clients& c : m_clients) {
        ss << c.tick_restantes;
    }
    return ss.str();
}

class ServerBox {
public:
    ServerBox();

    bool loadFile(std::string fileName);
    void process();
    void toString() const;
    bool waitLastTick();

private:

    void createServer(uint32_t numberClients);
    void tick();

    std::vector<Server> m_listServers;
    std::vector<uint32_t> m_numbers;
    std::string m_fileName = "C:/Users/Celso/Documents/arquivo.txt";
    uint32_t m_ttask = 0;
    uint32_t m_umax = 0;
    uint32_t m_tick_counter = 0;
};

ServerBox::ServerBox() {

}

void ServerBox::createServer(uint32_t numberClients) {

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

void ServerBox::tick() {

    for(Server& server : m_listServers) {
        server.tick();
        ++m_tick_counter;
    }

    m_listServers.erase(std::remove_if(m_listServers.begin(),
                                       m_listServers.end(),
                                       [=](Server s) { return (s.getCountClients() == 0); }),
                        m_listServers.end());

}

bool ServerBox::loadFile(std::string fileName) {

    //std::cout << "Enter the file name : ";
    //m_fileName = fileName;

    std::string line;
    std::cout << m_fileName.c_str() << std::endl;
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

void ServerBox::process() {

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

void ServerBox::toString() const
{
    //static int n_tick = 1;

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
    //std::cout << " ]" << std::endl;
}

bool ServerBox::waitLastTick()
{

    for(Server& s : m_listServers) {
        if(!s.isEmpty()) {

            return true;

        }

    }

    return false;

}

int main(int argc, char* argv[])
{

    if(argc == 1) {

        std::cout << "Use : load_balancer <input_file.txt>" << std::endl;
        return 1;

    }

    std::string fileName = argv[1];
    ServerBox sb;
    if(sb.loadFile(fileName)) {

        sb.process();

    } else {

        std::cout << "Falha ao processar arquivo" << std::endl;

    }

    return 0;

}
