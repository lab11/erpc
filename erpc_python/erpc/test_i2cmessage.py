import i2cmessage

i2c = i2cmessage.I2CMessage()
i2c.I2CSend(0x01,[0x01, 0x02, 0x03])
