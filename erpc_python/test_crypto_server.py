#! /usr/bin/env python

from __future__ import print_function

import sys
import math
import random
import erpc
#from Crypto.Signature import PKCS1_v1_5
#from Crypto.Hash import SHA
#from Crypto.PublicKey import RSA

import test_crypt

def server():
    transport = erpc.transport.I2CTransport(6,0x19)
    server = erpc.simple_server.SimpleServer(transport, erpc.basic_codec.BasicCodec)
    services = test_crypt.get_services()
    for service in services:
    	server.add_service(service)
    server.run()

if __name__ == "__main__":
    server()

