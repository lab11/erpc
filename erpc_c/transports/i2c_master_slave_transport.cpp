#include "i2c_master_slave_transport.h"
#include "tock.h"
#include "gpio.h"
#include "message.h"
#include <cassert>
#include <cstdio>

using namespace erpc;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

I2CMasterSlaveTransport::I2CMasterSlaveTransport(uint8_t RPCServerAddress, uint8_t responseAddress)
: m_RPCServerAddress(RPCServerAddress)
, m_responseAddress(responseAddress)
{
}

I2CMasterSlaveTransport::~I2CMasterSlaveTransport()
{
    //I don't know what to do here
}

erpc_status_t I2CMasterSlaveTransport::init()
{
    message_init(m_responseAddress);

    return kErpcStatus_Success;
}

erpc_status_t I2CMasterSlaveTransport::underlyingReceive(uint8_t *data, uint32_t size) {

    uint32_t len;
    uint8_t data_buffer[1024];

    if(size == 4) {
        uint8_t src = 0x00;
        while(src != m_RPCServerAddress) {
            len = message_recv(data_buffer,700, &src);
        }
        memcpy(data,data_buffer,4);
    } else {
        //trying to receive the body
        memcpy(data,data_buffer+4,size);
    }

    return kErpcStatus_Success;
}

erpc_status_t I2CMasterSlaveTransport::underlyingSend(const uint8_t *data, uint32_t size)
{
    static uint8_t header[4];
    uint8_t data_buffer[1024];

    if(size == 4) {
        //this is a header, just store the data and send it in the next call.
        //this is a bit of a hack that fits the i2c model better without
        //changing the interface
        memcpy(header,data,4);
    } else {
        data_buffer[0] = 0x01;
        memcpy(data_buffer+1,header,4);
        memcpy(data_buffer+5,data,size);
        message_send(m_RPCServerAddress,data_buffer,size+1+4);
    }

    return kErpcStatus_Success;
}

