/* autor : celso
 * servermanagement.h
 * Definicao da classe ServerManagement responsável pela gerencia dos servidores
 */

#ifndef SERVERMANAGEMENT_H
#define SERVERMANAGEMENT_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "server.h"

/**
 * Classe responsável pelos testes
 */
class ServerManagementTest;

class ServerManagement {
public:
    /**
     * @brief ServerManagement - Construtor
     */
    ServerManagement();

    /**
     * @brief ~ServerManagement - Destrutor
     */
    virtual ~ServerManagement();

    /**
     * @brief loadFile - Efetua o carregamento de um arquivo .txt passado como parametro
     * @param fileName
     * @return
     */
    bool loadFile(std::string fileName);

    /**
     * @brief process - processa todos os numeros lidos de clientes e cria os seus respectivos servidores
     */
    void process();

    /**
     * @brief toString - Imprime o texto contendo seridores com seus numeros de clientes
     */
    void toString() const;

    /**
     * @brief waitLastTick - Aguarda o ultimo cliente terminar o trabalho no servidor
     * @return
     */
    bool waitLastTick();

private:

    /**
     * Declarando a classe ServerManagementTest como amiga para poder visualizar seus dados privados
     * e poder comparar dados
     */
    friend class ServerManagementTest;

    /**
     * @brief createServer - Cria servidores necessários de acordo com o numero de clientes passado no parametro
     * @param numberClients
     */
    void createServer(uint32_t numberClients);

    /**
     * @brief tick - Decrementa a contagem de ticks servidores / clientes
     */
    void tick();

    /**
     * @brief m_listServers - Lista de servidores criada
     */
    std::vector<Server> m_listServers;

    /**
     * @brief m_numbers - Lista de numeros lidos para serem injetados
     */
    std::vector<uint32_t> m_numbers;

    /**
     * @brief m_fileName - Nome do arquivo a ser lido - Default
     */
    std::string m_fileName = "C:/Users/Celso/Documents/arquivo.txt";

    /**
     * @brief m_ttask - Numero de ticks por servidor
     */
    uint32_t m_ttask = 0;

    /**
     * @brief m_umax - Numero maximo de clientes por servidor
     */
    uint32_t m_umax = 0;

    /**
     * @brief m_tick_counter - Contador de ticks dos servidores
     */
    uint32_t m_tick_counter = 0;

};

#endif // SERVERMANAGEMENT_H
