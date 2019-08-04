#include "server.h"

Server::Server(uint32_t max_tick, uint32_t max_users, uint32_t countUsed) :
    m_max_tick(max_tick),
    m_max_users(max_users),
    m_count_users(0)
{

    for(uint32_t count = 0; count < countUsed; ++count) {
        addClient();
    }

}

Server::~Server()
{

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

bool Server::isEmpty() const {

    return (m_count_users == 0);

}

void Server::tick() {

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

size_t Server::getCountClients() const {

    return m_count_users;

}

