#include "chat_common.h"
#include "chat_client.h"

// --- // Global variables // --- //

// --- RDMA connection related resources --- //
static struct rdma_event_channel* cm_event_channel = NULL;
static struct rdma_cm_id* cm_client_id = NULL;
static struct ibv_pd* pd = NULL;
static struct ibv_comp_channel* io_completion_channel = NULL;
static struct ibv_cq* client_cq = NULL;
static struct ibv_qp_init_attr* qp_init_attr;
static struct ibv_qp* client_qp;

// --- Memory Buffer Related Resources --- //
static struct ibv_mr *client_metadata_mr = NULL,
                     *client_src_mr = NULL,
                     *client_dst_mr = NULL,
                     *server_metadata_mr = NULL;

static struct rdma_buffer_attr client_metadata_attr, server_metadata_attr;
static struct ibv_send_wr client_send_wr, *bad_client_send_wr = NULL;
static struct ibv_recv_wr server_recv_wr, *bad_server_recv_wr = NULL;
static struct ibv_sge client_send_sge, server_recv_sge;

// --- Source and Destination address buffers --- //
static char* src = NULL;
static char* dst = NULL;

static int client_prepare_connection(struct sockaddr_in* s_addr) {
    struct rdma_cm_event * cm_event = NULL;
    int ret = -1;

    // Open channel
    cm_event_channel = rdma_create_event_channel()      // From /usr/local/include/rdma/rdma_cma.h
    if (!cm_event_channel) {
        printf("Error in creating event channel. errono: %d\n", -errno);
        return -errno;
    }

    ret = rdma_create_id(cm_event_channel, 
                         &cm_client_id, 
                         NULL, RDMA_PS_TCP);            // From /usr/local/include/rdma/rdma_cma.h

    if (ret) {
        printf("Error in creating cm if. errno: %d\n", -errno);
        return -errno;
    }

	// Resolve destination and optional source addresses from IP addresses  to
	// an RDMA address.  If successful, the specified rdma_cm_id will be bound
	// to a local device.

    ret = rdma_resolve_addr(cm_client_id, NULL,
                            (struct sockaddr*) s_addr,
                            TIMEOUT_MS);                // From /usr/local/include/rdma/rdma_cma.h

    if (ret) {
        printf("Error in resolving address. errno: %d\n", -errno);
        return -errno;
    }

    // Process RDMA event
    ret = process_rdma_cm_event(cm_event_channel,
                                RDMA_CM_EVENT_ADDR_RESOLVED,
                                &cm_event);             // From /usr/local/include/rdma/rdma_cma.h
    
    if (ret) {
        printf("Error in processing RDMA event. errno: %d\n", -errno);
        return -errno;
    }

    // Acknowledge the event
    ret = rdma_ack_cm_event(cm_event);                  // From /usr/local/include/rdma/rdma_cma.h

    if (ret) {
        printf("Error in acknowledging the CM event. errno: %d\n", -errno);
        return -errno;
    }

    // Resolve route to destination in order to establish connection
    ret = rdma_resolve_route(cm_client_id, TIMEOUT_MS); // From /usr/local/include/rdma/rdma_cma.h

    if (ret) {
        printf("Error in resolving the route. errno: %d\n", -errno);
        return -errno;
    }

    // Process the SYN ACK event
    ret = process_rdma_cm_event(cm_event_channel,
                                RDMA_CM_EVENT_ROUTE_RESOLVED,
                                &cm_event);             

    if (ret) {
        printf("Error, did not receive a valid event. errno: %d\n", ret);
        return ret;
    }

	ret = rdma_ack_cm_event(cm_event);
	if (ret) {
		rdma_error("Failed to acknowledge the CM event, errno: %d \n", -errno);
		return -errno;
	}

    printf();

}
