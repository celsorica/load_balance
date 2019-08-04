/* autor : celso
 * server.h
 * Definicao da classe Server responsável pela alocacao dos clientes
 */

#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

typedef struct {
    uint32_t tick_restantes = 0;
} Clients;

/**
 * Classe responsável pelos testes
 */
class ServerTest;

class Server {
public:
    /**
     * @brief Server - Construtor
     * @param max_tick - Numero maximo de ticks
     * @param max_users - Numero maximo de clientes
     * @param countUsed - Quantidade de clientes já utilizando o servidor na criação
     */
    explicit Server(uint32_t max_tick, uint32_t max_users, uint32_t countUsed = 0);

    /**
     * @brief ~Server - Destrutor
     */
    virtual ~Server();

    /**
     * @brief addClient - Adiciona um novo cliente no servidor
     */
    void addClient();

    /**
     * @brief isFull - Retorna true caso o servidor esteja na capacidade máxima caso contrario false
     * @return
     */
    bool isFull() const;

    /**
     * @brief isEmpty - Retorna true caso o servidor esteja vazio caso contrario false
     * @return
     */
    bool isEmpty() const;

    /**
     * @brief tick - Varre todos os clientes adicionados e decrementa o contador de ticks de cada um
     */
    void tick();

    /**
     * @brief getCountClients - Retorna a quantidade de clientes conectada no servidor atualmente
     * @return
     */
    size_t getCountClients() const;

private:

    /**
     * Declarando a classe ServerManagementTest como amiga para poder visualizar seus dados privados
     * e poder comparar dados
     */
    friend class ServerTest;

    /**
     * @brief m_clients - Lista dos clientes do servidor
     */
    std::vector<Clients> m_clients;

    /**
     * @brief m_max_tick - Numero de ticks do servidor
     */
    uint32_t m_max_tick = 0;

    /**
     * @brief m_max_users - Numero maximo de usuarios do servidor
     */
    uint32_t m_max_users = 0;

    /**
     * @brief m_count_users - Numero de usuarios conectados no servidor
     */
    uint32_t m_count_users = 0;
};

#endif // SERVER_H
