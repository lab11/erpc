from __future__ import print_function

import sys
import math
import random
import erpc
from Crypto.Signature import PKCS1_v1_5
from Crypto.Hash import SHA
from Crypto.PublicKey import RSA

import test_crypt

def get_services():
    class CryptoServiceHandler(test_crypt.interface.ICrypto):
        def sign(self, data):
            key = RSA.generate(2048)
            h = SHA.new(str(data))
            signer = PKCS1_v1_5.new(key)
            signature = signer.sign(h)
	    bytes = bytearray(signature)
	    print(bytes[16])
	    data2 = test_crypt.common.ret(bytearray(signature))
            return data2

    cryptoHandler = CryptoServiceHandler()
    cryptoService = test_crypt.server.CryptoService(cryptoHandler)
    return [cryptoService]

