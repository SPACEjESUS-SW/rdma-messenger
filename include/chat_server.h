// chat_server.h
#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

/**
 * @brief Prepares client connection and accept it
 * 
 * @return -1 on failure
 */
static int setup_client_connection();

/**
 * @brief starts an RDMA server by allocating basic connections
 * 
 * @param server_addr (struct sockaddr_in*)
 * 
 * @returns -1 on failure
 */
static int start_rdma_server(struct sockaddr_int*);

/**
 * @brief accepts a RDMA client connection
 * 
 * @return -1 on failure
 */
static int accept_client_connection();

/**
 * @brief Sends server side metadata to client
 * 
 * @return -1 on failure
 */
static int send_server_metadata_to_client();

/**
 * @brief Services rdma_disconnect() from client and cleans up it's resource
 * 
 * @return -1 on failure
 */
static int disconnect_and_cleanup();

#endif