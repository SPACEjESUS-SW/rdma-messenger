// chat_client.h
#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

/**
 * @brief Macro defining timeout period in ms
 */
#define TIMEOUT_MS 2000

/**
 * @brief Prepares client side connection resources for an RDMA connection
 * 
 * @param s_addr (struct sockaddr_in*) 
 * 
 * @return -1 on failure
 */
static int client_prepare_connection(struct sockaddr_in*);

/**
 * @brief pre-posts a receive buffer before calling connect()
 * 
 * @return -1 on failure
 */
static int client_pre_post_revc_buffer();

/**
 * @brief Connects to RDMA server
 * 
 * @return -1 on failure
 */
static int client_connect_to_server();

/**
 * @brief Exchanges metadata with the server
 * 
 * @return -1 on failure
 */
static int client_xchange_metadata();

/**
 * @brief Handles RDMA buffer related operations
 * 
 * @return -1 on failure
 */
static int client_remote_memory_ops();

/**
 * @brief Disconnects from RDMA server and cleans up
 * 
 * @return -1 on failure
 */
static int client_disconnect();

#endif