import i2cmessage
import time

i2c = i2cmessage.I2CMessage()
data = []
for i in range(0,254):
	data.append(i+1)
i2c.I2CSend(0x19,data)
time.sleep(0.001)
result = i2c.I2CRead(0x19,254)
