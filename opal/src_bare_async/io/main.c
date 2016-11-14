/**
 * Copyright (C) 2015 Kalray SA.
 *
 * All rights reserved.
 */
#include <mppa_power.h>
#include <mppa_routing.h>
#include <stdio.h>
#include <mppa_noc.h>
#include <mppa_remote.h>
#include <mppa_async.h>
#include <assert.h>
#include <utask.h>

#include <HAL/hal/hal_ext.h>
#include <HAL/hal/board/boot_args.h>

#include "common_io.h"

int main() {

	mppadesc_t pcie_fd = 0;
	if (__k1_spawn_type() == __MPPA_PCI_SPAWN) {
		pcie_fd = pcie_open(0);
		pcie_queue_init(pcie_fd);
		pcie_register_console(pcie_fd, stdin, stdout);
	}

	#ifdef MPPA_ASYNC_BOOST
	mppa_rpc_server_set_nb_rm(NB_RM);
	#endif // MPPA_ASYNC_BOOST

	mppa_rpc_server_init(0, 0, NB_CLUSTERS);
	mppa_async_server_init();
	mppa_remote_server_init(pcie_fd, NB_CLUSTERS);

	for(unsigned i=0;i<NB_CLUSTERS;i++){
		if (mppa_power_base_spawn(i, CLUSTER_EXEC_NAME, NULL, NULL, MPPA_POWER_SHUFFLING_ENABLED) == -1)
			printf("# [IODDR0] Fail to Spawn cluster %d\n", i);
	}

	// start and sync with IO South
	mppa_power_base_spawn(192, IO_SOUTH_EXEC_NAME, NULL, NULL, MPPA_POWER_SHUFFLING_ENABLED);
	mppa_cnoc_mailbox_notif_t notif = { .reg = 0 };
	notif._.evt_en = 1;
	notif._.rm = 1U<< __k1_get_cpu_id();
	mppa_noc_cnoc_rx_configuration_t cnoc_config_rx = {0};
	cnoc_config_rx.mode = MPPA_NOC_CNOC_RX_MAILBOX;
	mppa_noc_cnoc_rx_alloc(0, 127);
	mppa_noc_cnoc_rx_configure(0, 127, cnoc_config_rx, &notif);
	mppa_noc_wait_clear_event(0, MPPA_NOC_INTERRUPT_LINE_CNOC_RX, 127);
	mppa_noc_cnoc_rx_free(0, 127);

	printf("# [IODDR0] start mppa_rpc_server_start\n");

	#ifdef MPPA_ASYNC_BOOST
	utask_t t[NB_RM-1];
	for (unsigend int = 0; i < NB_RM-1; i++){
		utask_create(&t[i], NULL, mppa_rpc_server_start, NULL);
	}
	#endif // MPPA_ASYNC_BOOST

	mppa_rpc_server_start();

	for(unsigned i=0;i<NB_CLUSTERS;i++){
		int status;
		if (mppa_power_base_waitpid(i, &status, 0) < 0) {
			printf("# [IODDR0] Waitpid failed on cluster %d\n", i);
	   	}
	}

	/*mppa_rpc_request_t mppa_async_request = {
		.lib_id = 0,
		.cmd = (int)MPPA_RPC_DISPLAY_STATS
	};
	mppa_rpc_server_process(&mppa_async_request, NULL);*/

	if(__k1_spawn_type() == __MPPA_PCI_SPAWN){
		mppa_remote_server_exit();
	}

	/* Send an exit message on pcie interface */
	if (__k1_spawn_type() == __MPPA_PCI_SPAWN) {
		int remote_status;
		pcie_queue_barrier(pcie_fd, 0, &remote_status);
		pcie_unregister_console(pcie_fd);
		pcie_queue_exit(pcie_fd, 0, NULL);
	}
	printf("# [IODDR0] Goodbye\n");
	return 0;
}
