from periphery import I2C
import math

class I2CMessage():
    def __init__(self):
        self._i2c = I2C("/dev/i2c-6")

    def I2CSend(self, dest, data):
        toSend = len(data)
        length = toSend
        header = bytearray(8)
        maxLen = 247

        numPackets = int(math.ceil(float(toSend)/maxLen))
        totalLen = int(numPackets*8+toSend)

        #make a header
        #version
        header[0] = 0x01
        header[1] = (totalLen >> 8) & 0xFF
        header[2] = (totalLen & 0xFF)
        header[3] = 0x00
        header[4] = 0x01
	header[7] = 0x01

        while toSend > 0:
            if toSend > maxLen:
                header[5] = 0x80
	    else:
                header[5] = 0x00

            offset = length-toSend
            header[5] = header[5] | (offset >> 8)
            header[6] = (offset & 0xff)

            dataByte = None
            if toSend > maxLen:
                dataByte = bytearray(data[offset:offset+maxLen])
                toSend = toSend - maxLen
            else:
                dataByte = bytearray(data[offset:offset+toSend])
		toSend = 0

            msg = [I2C.Message(header+dataByte)]
            self._i2c.transfer(dest,msg)


    def I2CRead(self, dest, count):
        maxLen = 247
        numPackets = int(math.ceil(float(count)/maxLen))

        toReceive = count
        dataToReturn = bytearray(count)
        while toReceive > 0:
            if toReceive > maxLen:
                rec = bytearray(255)
                msg = [I2C.Message(rec,read=True)]
                self._i2c.transfer(dest,msg)
                dataToReturn[count-toReceive:count-toReceive+maxLen] = msg[0].data[8:]
                toReceive = toReceive - maxLen
            else:
                rec = bytearray(8+toReceive)
                msg = [I2C.Message(rec,read=True)]
                self._i2c.transfer(dest,msg)
                dataToReturn[count-toReceive:] = msg[0].data[8:]
                toReceive = 0

        return dataToReturn

