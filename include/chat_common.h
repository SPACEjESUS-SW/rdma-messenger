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

/**
 * @brief Main structure used for exchange of information between the server and the client
 *
 * We use the `packed` attribute so the compiler doesn't pad the struct
 */
struct __attribute((packed)) rdma_buffer_attr {
    uint64_t address;
    uint64_t length;

    /**
     * @brief This union is used to name the packed differently based
     * on wheather it is being received or sent
     *
     * @param local_stag (uint64_t): The packet is called local_stag if it is being sent
     * @param remote_stag (uint64_t): The packet is called remote_stag if it is being received
     */
    union stag {
        uint64_t local_stag;
        uint64_t remote_stag;
    } stag;
};

/**
 * @brief Resolves a given destination address
 * 
 * @param dst (const char*) destination address
 * @param addr (struct sockaddr*)
 * 
 * @return -1 on failure, positive number on success
 */
int get_addr(const char*, struct sockaddr*);

/**
 * @brief Prints RDMA buffer information
 * 
 * @param attr (struct rdma_buffer_attr*) 
 */
void show_rdma_buffer_attr(const struct rdma_buffer_attr*);

/**
 * @brief Processes an RDMA connection management (CM) event
 * 
 * @param echannel (struct rdma_event_channel*) CM event channel where the event is expected
 * @param expected_event (enum rdma_cm_event_type) Expected event type
 * @param cm_event (struct rdma_cm_event**) Memory location where the event will be stored
 */
int process_rdma_cm_event(struct rdma_event_channel*, enum rdma_cm_event_type,
                          struct rdma_cm_event**);

/**
 * @brief Allocated a RDMA buffer with size `length` and also registers the buffer
 * 
 * @param pd (struct ibv_pd*) protection domain where the buffer will be allocated
 * @param length (size_t) length of the buffer
 * @param permission (enum ibv_access_flags) OR of permissions as defined fir the flag ibv_access_flags
 * 
 * @return pointer to allocated buffer. NULL on failure
 */
struct ibv_mr* rdma_buffer_alloc(struct ibv_pd*, size_t, enum ibv_access_flag);

/**
 * @brief Frees the allocated buffer
 * 
 * @param mr (struct ibv_mr*)
 */
void rdma_buffer_free(struct ibv_mr*);

/**
 * @brief Registers a previously allocated buffer
 * 
 * @param pd (struct ibv_pd*) protection domain where the buffer will be allocated
 * @param addr (void*) Address of the buffer
 * @param length (size_t) length of the buffer
 * @param permission (enum ibv_access_flags) OR of permissions as defined fir the flag ibv_access_flags
 * 
 * @return pointer to allocated buffer. NULL on failure
 */
struct ibv_mr* rmda_buffer_register(struct ibv_pd*, void*, size_t, enum ibv_access_flag);

/**
 * @brief Deregisters a previously allocated buffer
 * 
 * @param mr (struct ibv_mr*)
 */
void rdma_buffer_deregister(struct ibv_mr*);

/**
 * @brief Processes a work completion event notification
 * 
 * @param comp_channel (struct ubv_comp_channel*) Completion Channel where work completion notifications are
 *                     expected to arrive
 * @param wc (struct ibv_wc) Array holding work completion elements
 * @param max_wc (int) Maximum number of WC elements
 */
int process_work_completion_events(struct ibv_comp_channel*, struct ibv_wc, int);

/**
 * @brief Prints some details of the cm id
 * 
 * @param id (struct rdma_cm_id*)
 */
void show_rdma_cmid(struct rdma_cm_id*);

#endif