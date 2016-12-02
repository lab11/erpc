#include "i2c_master_slave_transport.h"
#include "tock.h"
#include "gpio.h"
#include "i2c_master_slave.h"
#include <cassert>
#include <cstdio>

using namespace erpc;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

#define BUFFER_SIZE 256
static uint8_t master_write_buf[BUFFER_SIZE];
static uint8_t master_read_buf[BUFFER_SIZE];
static uint8_t slave_write_buf[BUFFER_SIZE];
volatile static bool rpc_done;

void i2c_master_slave_callback(int callback_type, int length, int unused, void* callback_args) {
    if(callback_type == CB_SLAVE_WRITE) {
        rpc_done = true;
    } else if(callback_type == CB_MASTER_WRITE) {
        gpio_clear(11);
    }
}

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
    //setup the callbacks and allocate the necessary buffers
    i2c_master_slave_set_callback(i2c_master_slave_callback, NULL);
    i2c_master_slave_set_master_write_buffer(master_write_buf,BUFFER_SIZE);
    i2c_master_slave_set_master_read_buffer(master_read_buf,BUFFER_SIZE);
    i2c_master_slave_set_slave_write_buffer(slave_write_buf,BUFFER_SIZE);
    i2c_master_slave_set_slave_address(m_responseAddress);
    i2c_master_slave_listen();


    return kErpcStatus_Success;
}

erpc_status_t I2CMasterSlaveTransport::underlyingReceive(uint8_t *data, uint32_t size) {

    //listen for the response from the storage master

    /*while(!rpc_done) {
        yield();
    }*/

    if(size == 4) {
        //trying to receive the header
        yield();
        memcpy(data,slave_write_buf,4);
        gpio_clear(11);
    } else {
        //trying to receive the body
        memcpy(data,slave_write_buf+4,size);
    }

    return kErpcStatus_Success;
}

erpc_status_t I2CMasterSlaveTransport::underlyingSend(const uint8_t *data, uint32_t size)
{
    static uint8_t header[4];

    if(size == 4) {
        //this is a header, just store the data and send it in the next call.
        //this is a bit of a hack that fits the i2c model better without
        //changing the interface
        memcpy(header,data,4);
    } else {
        //this must be an actual transmission
        //set the response address
        uint8_t reg = 0x03;
        memcpy(master_write_buf,&reg,1);
        memcpy(master_write_buf+1,&m_responseAddress,1);
        i2c_master_slave_write_sync(m_RPCServerAddress,2);

        //copy the rpc request into the buffer
        memcpy(master_write_buf+1,header,4);
        memcpy(master_write_buf+5,data,size);
        //the register for RPC requests is 0x01
        master_write_buf[0] = 0x01;

        i2c_master_slave_write_sync(m_RPCServerAddress,size+1+4);
        rpc_done = false;
    }

    return kErpcStatus_Success;
}

