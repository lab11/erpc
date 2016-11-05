include ../../../mk/erpc_common.mk

CXXFLAGS += -DSERIAL

APP_NAME = test_arithmetic
INCLUDES += ./ \
			$(ERPC_ROOT)/erpc_c/config \
			$(ERPC_ROOT)/erpc_c/infra \
			$(ERPC_ROOT)/erpc_c/port \
			$(ERPC_ROOT)/erpc_c/setup \
			$(ERPC_ROOT)/erpc_c/transports \
			$(ERPC_ROOT)/erpcgen/src \

SOURCES += test_arithmetic_client_serial.c \
		   test_arithmetic_client.cpp \
		   $(ERPC_ROOT)/erpc_c/setup/erpc_client_setup.cpp \
		   $(ERPC_ROOT)/erpc_c/setup/erpc_setup_serial.cpp \
		   $(ERPC_ROOT)/erpc_c/infra/basic_codec.cpp \
		   $(ERPC_ROOT)/erpc_c/infra/client_manager.cpp \
		   $(ERPC_ROOT)/erpc_c/infra/framed_transport.cpp \
		   $(ERPC_ROOT)/erpc_c/infra/message_buffer.cpp \
		   $(ERPC_ROOT)/erpc_c/transports/serial_transport.cpp \
		   $(ERPC_ROOT)/erpc_c/port/erpc_port_stdlib.cpp \
		   $(ERPC_ROOT)/erpc_c/port/erpc_threading_pthreads.cpp \
		   $(ERPC_ROOT)/erpc_c/port/serial.cpp

LIBRARIES += -lpthread -lrt
LDFLAGS += -L$(ERPC_ROOT)/Debug/Linux/test/lib

include $(ERPC_ROOT)/mk/targets.mk
