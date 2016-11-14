#include <stdio.h>
#include <stdlib.h>
#include <pcie.h>

int main(int argc, char **argv)
{
	mppadesc_t fd;
	int mppa_ret;

	if(!(fd = pcie_open_device(atoi(argv[3])))) 
		exit(-1);

	if (argc >= 3) {
		if ( pcie_load_io_exec_mb(fd, argv[1], argv[2], PCIE_LOAD_FULL ) ) {
			printf ("Boot of MPPA failed\n");
			exit(1);
		}
	}
	printf("# [HOST] pcie queue init\n");
	pcie_queue_init(fd);
	/* pcie_queue init needs to be called to enable pcie communication via queues */ 
	printf("# [HOST] init queue ok\n");	
	pcie_register_console(fd, stdin, stdout);
	printf("# [HOST] pcie_register_console ok\n");
	int status;
	int local_status = 0;
	printf("# [HOST] waits\n");	
	pcie_queue_barrier(fd, local_status, &status);
	pcie_queue_exit(fd, 0, &mppa_ret);
	printf("# [HOST] MPPA exited with status %d\n", status);
	pcie_close(fd);
	printf("# [HOST] Goodbye\n");
 	return 0;
}
