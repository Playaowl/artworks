import smbus
from time import sleep
import time
import VL53L0X
#Definieren der Variablen und Konstanten
Busnummer = 1
Adresse_SRF02 = 0x74
Adresse_LIDAR = 0x29
Start = 0x51
High_Byte = 0
Low_Byte = 0
Entfernung = 0

#Instanzieren eines I2C Objektes
i2cBus = smbus.SMBus(Busnummer)
tof = VL53L0X.VL53L0X()
tof.start_ranging(VL53L0X.VL53L0X_BETTER_ACCURACY_MODE)

#Entfernungsmessung
while True:

    i2cBus.write_byte_data(Adresse_SRF02, 0x00, 0x51)
    sleep(0.1)
    High_Byte = i2cBus.read_byte_data(Adresse_SRF02, 0x02)
    Low_Byte = i2cBus.read_byte_data(Adresse_SRF02, 0x03)
    if High_Byte == 0:

        Entfernung = Low_Byte

    else:

        Entfernung = (High_Byte*256)+Low_Byte

    distance = tof.get_distance()
    if (distance > 0):
        print ("%d mm, %d cm, %d" % (distance, (distance/10), count))


    print "Entfernung %d" % Entfernung
