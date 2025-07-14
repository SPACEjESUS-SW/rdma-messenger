// chat_common.h
#ifndef CHAT_COMMON_H
#define CHAT_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>

#include <netdb.h>
#include <netinet/in.h>	
#include <arpa/inet.h>
#include <sys/socket.h>

#include <rdma/rdma_cma.h>
#include <infiniband/verbs.h>

/**
 * @brief Macro defining the size of Completion Queue (CP)
 */
#define CQ_CAPACITY 16

/**
 * @brief Macro defining maximum Scatter Gather Elements
 */
#define MAX_SGE 2

/**
 * @brief Macro defining the maximum work requests
 */
#define MAX_WR 8

/**
 * @brief Macro defining the default RDMA port
 */
#define DEFAULT_PORT 6969

#endif