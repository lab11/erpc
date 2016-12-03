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
    class CryptoServiceHandler(test_crypt.interface.ICrypto):
        def sign(self, data):
            #key = RSA.generate(2048)
            #h = SHA.new(data)
            #signer = PKCS1_v1_5.new(key)
            #signature = signer,sign(h)
            for i, val in data:
                data[i] = val+1

            return ret(data)

    transport = erpc.transport.I2CTransport(6,0x19)
    server = erpc.simple_server.SimpleServer(transport, erpc.basic_codec.BasicCodec)
    cryptoHandler = CryptoServiceHandler()
    cryptoService = test_crypt.server.CryptoService(cryptoHandler)
    server.add_service(cryptoService)
    server.run()

if __name__ == "__main__":
    server()

