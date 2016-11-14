/**
 * Copyright (C) 2015 Kalray SA.
 *
 * All rights reserved.
 */
#include <mppa_power.h>
#include <mppa_routing.h>
#include <stdio.h>
#include <mppa_remote.h>
#include <mppa_noc.h>
#include <mppa_async.h>
#include <assert.h>
#include <utask.h>

#include "common_io.h"
 
int main() {

	//printf("# [IODDR1] Hello\n");
	
	#ifdef MPPA_ASYNC_BOOST
	mppa_rpc_server_set_nb_rm(NB_RM);	
	#endif // MPPA_ASYNC_BOOST

	mppa_rpc_server_init(0, 0, NB_CLUSTERS);
	mppa_async_server_init();
	mppa_remote_server_init(0, NB_CLUSTERS);

	//printf("# [IODDR1] start mppa_rpc_server_start\n");

	mppa_cnoc_header_t cnoc_header_tx = { 0 };
	mppa_cnoc_config_t cnoc_config_tx = { 0 };
	cnoc_header_tx._.tag = 127;
	unsigned tx_id = 0;

	if(mppa_noc_cnoc_tx_alloc_auto(1, &tx_id, MPPA_NOC_BLOCKING) != MPPA_NOC_RET_SUCCESS)
		printf("# Fail alloc cnoc tx\n");
	mppa_routing_get_cnoc_unicast_route(__k1_get_cluster_id()+1, 128, &cnoc_config_tx, &cnoc_header_tx);
	mppa_noc_cnoc_tx_configure(1, tx_id, cnoc_config_tx, cnoc_header_tx);	
	mppa_noc_cnoc_tx_push_eot(1, tx_id, 1);
	mppa_noc_cnoc_tx_free(1, tx_id);

	#ifdef MPPA_ASYNC_BOOST
	utask_t t[NB_RM-1];
	for (unsigend int = 0; i < NB_RM-1; i++){
		utask_create(&t[i], NULL, mppa_rpc_server_start, NULL);	
	}
	#endif // MPPA_ASYNC_BOOST
	
	mppa_rpc_server_start();

	//printf("# [IODDR1] Goodbye\n");
	return 0;
}
