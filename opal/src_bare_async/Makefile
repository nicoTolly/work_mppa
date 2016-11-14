ifeq ($(ARCH), )
ARCH := k1b
SIM_ARCH := bostan
endif

ifeq ($(ARCH), k1a)
SIM_ARCH := andey
endif

ifeq ($(ARCH), k1b)
SIM_ARCH := bostan
endif

ifeq ($(O), )
O := ${PWD}/output/$(CORE)/$(BOARD)
endif

arch := $(ARCH)
#board := large_memory
board := developer

system-name := bare
COMPILE_OPTI := -O3

################################################################################
# Bare, async only, no DSM
################################################################################
# Cluster rules
cluster-cflags := -g -std=c99 ${COMPILE_OPTI} -mhypervisor -I . -Wall
cluster-lflags := -g -mhypervisor -lm -Wl,--defsym=USER_STACK_SIZE=0x1000 -Wl,--defsym=KSTACK_SIZE=0x1000
cluster-lflags += -lvbsp -lutask -lmppa_remote -lmppa_async -lmppa_request_engine -lmppapower -lmppanoc
cluster-lflags += -lmpparouting -Wl,--defsym=_LIBNOC_DISABLE_FIFO_FULL_CHECK=0

# IO rules
io-cflags := -std=c99 -g ${COMPILE_OPTI} -DMPPA_TRACE_ENABLE -Wall -mhypervisor
io-lflags := -lvbsp -lmppa_remote -lmppa_async -lmppa_request_engine -lpcie_queue -lutask  -lmppapower
io-lflags += -lmppanoc -lmpparouting -mhypervisor -Wl,--defsym=_LIBNOC_DISABLE_FIFO_FULL_CHECK=0

################################################################################
# Variables list
################################################################################

# duration_list  := 10 20 50 100 150 200 400 600 800 1000
# duration_list  := 10 100 200 400 800
duration_list  := 1000

# grid size = (cavity)^3
# cavity_list    := 64 96 128 160 192 224
cavity_list    := 128

# Note : cavity^3 | (nb_clusters * nb_cores)
nb_clusters_list   := 1 2 4 8 16          # nb_clusters, max = 16
nb_cores_list      := 16          # nb_cores within a cluster, max = 16
nb_ddr_list        := 1 2
pipeline_depth_list   := 1 2 4

# monitoring
monitoring ?= 1

################################################################################
define gen-rules-mpk-4

#####################################
# Build Host
#####################################
host-bin += host_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)
host_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-srcs   := $(wildcard host/*.c)

host_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-cflags := -I./host -g -Wall ${COMPILE_OPTI}
host_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-lflags := -lpthread -lm -lrt -lmppa_remote -lpcie
host_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-name   := Host_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)

#####################################
# Build cluster
#####################################
cluster-bin += cluster_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)
# cluster_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-srcs   := $(wildcard cluster/*.c)
cluster_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-srcs   := cluster/config.c cluster/data.c cluster/export.c
cluster_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-srcs   += cluster/print.c cluster/results.c cluster/utils.c cluster/main.c
cluster_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-srcs   += cluster/compute_common.c cluster/compute.c

cluster_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-cflags := -I./cluster -DNB_DDR=$(5) -DASYNC_PIPELINE_DEPTH=$(6) -DMONITORING=${monitoring}
cluster_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-cflags += -DDURATION=$(1) -DCAVITY=$(2) -DNB_CLUSTERS=$(3) -DNB_CORES=$(4)
cluster_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-cflags += -D TWO_LATTICE

# In-place update LBM
cluster-bin += cluster_opal_async_inplace_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)
cluster_opal_async_inplace_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-srcs   := cluster/config.c cluster/data.c cluster/export.c
cluster_opal_async_inplace_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-srcs   += cluster/print.c cluster/results.c cluster/utils.c cluster/main.c
cluster_opal_async_inplace_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-srcs   += cluster/compute_common.c cluster/compute_inplace.c

cluster_opal_async_inplace_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-cflags := -I./cluster -DNB_DDR=$(5) -DASYNC_PIPELINE_DEPTH=$(6) -DMONITORING=${monitoring}
cluster_opal_async_inplace_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-cflags += -DDURATION=$(1) -DCAVITY=$(2) -DNB_CLUSTERS=$(3) -DNB_CORES=$(4)
cluster_opal_async_inplace_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-cflags += -D INPLACE

# Shift update LBM
cluster-bin += cluster_opal_async_shift_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)
cluster_opal_async_shift_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-srcs   := cluster/config.c cluster/data.c cluster/export.c
cluster_opal_async_shift_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-srcs   += cluster/print.c cluster/results.c cluster/utils.c cluster/main.c
cluster_opal_async_shift_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-srcs   += cluster/compute_common.c cluster/compute_shift.c

cluster_opal_async_shift_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-cflags := -I./cluster -DNB_DDR=$(5) -DASYNC_PIPELINE_DEPTH=$(6) -DMONITORING=${monitoring}
cluster_opal_async_shift_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-cflags += -DDURATION=$(1) -DCAVITY=$(2) -DNB_CLUSTERS=$(3) -DNB_CORES=$(4)
cluster_opal_async_shift_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-cflags += -D SHIFT

#####################################
# Build IO 0
#####################################
io-bin += io0_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)
io0_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-srcs   := $(wildcard io/main.c)

io0_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-cflags := -I./cluster # -DMPPA_ASYNC_BOOST
io0_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-cflags += -DCLUSTER_EXEC_NAME=\"cluster_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)\"
io0_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-cflags += -DIO_SOUTH_EXEC_NAME=\"io_bin_south_$(3)_clusters\"
io0_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-cflags += -DDURATION=$(1) -DCAVITY=$(2) -DNB_CLUSTERS=$(3) -DNB_CORES=$(4)

# In-place update LBM
io-bin += io0_opal_async_inplace_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)
io0_opal_async_inplace_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-srcs   := $(wildcard io/main.c)

io0_opal_async_inplace_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-cflags := -I./cluster # -DMPPA_ASYNC_BOOST
io0_opal_async_inplace_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-cflags += -DCLUSTER_EXEC_NAME=\"cluster_opal_async_inplace_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)\"
io0_opal_async_inplace_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-cflags += -DIO_SOUTH_EXEC_NAME=\"io_bin_south_$(3)_clusters\"
io0_opal_async_inplace_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-cflags += -DDURATION=$(1) -DCAVITY=$(2) -DNB_CLUSTERS=$(3) -DNB_CORES=$(4)

# Shift update LBM
io-bin += io0_opal_async_shift_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)
io0_opal_async_shift_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-srcs   := $(wildcard io/main.c)

io0_opal_async_shift_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-cflags := -I./cluster # -DMPPA_ASYNC_BOOST
io0_opal_async_shift_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-cflags += -DCLUSTER_EXEC_NAME=\"cluster_opal_async_shift_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)\"
io0_opal_async_shift_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-cflags += -DIO_SOUTH_EXEC_NAME=\"io_bin_south_$(3)_clusters\"
io0_opal_async_shift_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-cflags += -DDURATION=$(1) -DCAVITY=$(2) -DNB_CLUSTERS=$(3) -DNB_CORES=$(4)

#####################################
# Build IO 1
#####################################
io-bin += io_bin_south_$(3)_clusters
io_bin_south_$(3)_clusters-srcs   := $(wildcard io/south.c)
io_bin_south_$(3)_clusters-cflags := -DNB_CLUSTERS=$(3)

#####################################
# Multibinary
#####################################
mppa-bin +=  multibin_ioddr0_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)
multibin_ioddr0_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-objs := cluster_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)
multibin_ioddr0_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-objs += cluster_opal_async_inplace_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)
multibin_ioddr0_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-objs += cluster_opal_async_shift_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)
multibin_ioddr0_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-objs += io0_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)
multibin_ioddr0_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-objs += io0_opal_async_inplace_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)
multibin_ioddr0_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-objs += io0_opal_async_shift_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)
multibin_ioddr0_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-objs += io_bin_south_$(3)_clusters
multibin_ioddr0_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)-flags := -w ".*"

hw_test: hw_test_opal

build: host_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6) multibin_ioddr0_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)

hw_test_opal: multibin_ioddr0_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6) host_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)
	@echo "hw_test: OPAL ASYNC"
	timeout 500 $(O)/bin/Host_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6) $(O)/bin/multibin_ioddr0_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6).mpk io0_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6) 0

# sim_test_opal: multibin_ioddr0_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6) host_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6)
# 	@echo "hw_test: opal"
# 	timeout 500 $(K1_TOOLCHAIN_DIR)/bin/k1-pciesim-runner $(O)/bin/Host_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6) $(O)/bin/multibin_ioddr0_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6).mpk io0_opal_async_$(1)_$(2)_$(3)_$(4)_ddr$(5)_depth$(6) 0

endef

$(foreach duration,$(duration_list),\
	$(foreach cavity,$(cavity_list),\
		$(foreach nb_clusters,$(nb_clusters_list),\
			$(foreach nb_cores,$(nb_cores_list),\
				$(foreach nb_ddr,$(nb_ddr_list),\
					$(foreach pipeline_depth,$(pipeline_depth_list),\
						$(eval $(call gen-rules-mpk-4,$(duration),$(cavity),$(nb_clusters),$(nb_cores),$(nb_ddr),$(pipeline_depth)))\
					)\
				)\
			)\
		)\
	)\
)

include $(K1_TOOLCHAIN_DIR)/share/make/Makefile.kalray
