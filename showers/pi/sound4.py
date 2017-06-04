import os, sys, time, wave, pygame, pygame.mixer, numpy, pyaudio, pygame.sndarray, scikits.samplerate, struct
import smbus
import VL53L0X
#Definieren der Variablen und Konstanten
Busnummer = 1
Adresse_SRF02 = 0x74
Start = 0x51
High_Byte = 0
Low_Byte = 0
Entfernung = 0

numSensors = 2
limit = 250  # in cm, alles was darueber ist wird ignoriert
minDistanz = 20.0
maxDistanz = 250.0
minOctave = 0.0
maxOctave = 1.0
offset = 100
"""
minOctave, maxOctave: the highest and lowest pitch changes. 0 is no change.
"""

ratio0 = 1.0  # The resampling ratio
pygame.mixer.init(44100, -16, 2, 4096)

sound0 = pygame.mixer.Sound("Downloads/shower.wav")
sound1 = pygame.mixer.Sound("Downloads/om.wav")
sound0.set_volume(1.0)
sound1.set_volume(1.0)
snd_array0 = pygame.sndarray.array(sound0)
snd_array1 = pygame.sndarray.array(sound1)
lastsound0 = sound0
lastsound1 = sound1
#Instanzieren eines I2C Objektes
i2cBus = smbus.SMBus(Busnummer)
tof = VL53L0X.VL53L0X()
tof.start_ranging(VL53L0X.VL53L0X_BETTER_ACCURACY_MODE)

i2cBus.write_byte_data(Adresse_SRF02, 0x00, 0x51)
sleep(0.1)

while True:

    High_Byte = i2cBus.read_byte_data(Adresse_SRF02, 0x02)
    Low_Byte = i2cBus.read_byte_data(Adresse_SRF02, 0x03)
    if High_Byte == 0:

        Entfernung = Low_Byte

    else:

        Entfernung = (High_Byte*256)+Low_Byte

    distance = tof.get_distance()
    if (distance > 0):
        print ("IS %d mm, %d cm" % (distance, (distance/10)))


    print "US Entfernung %d" % Entfernung


    wert0 = Entfernung
    #		print 'wert0: '+str(wert0)
    ratio0 = 0.01
    if (wert0 > 0 and wert0 < limit):
        #			ratio0 = maxOctave - (wert0-offset) / (limit-offset)
        ratio0 = 1.0 - (max(min(wert0, maxDistanz), minDistanz) - minDistanz) / (maxDistanz - minDistanz)
        #			print('max(min(wert0,250.0),100.0)/150'+str( max(min(wert0,250.0),100.0)/150.0))
        #			print 'ratio 0: ' + str(ratio0)
        #			sound0.set_volume(ratio0)
        #		else:
        #			sound0.set_volume(0.0)
        # print 'wert 1: ' + str(wert1 )

    wert1 = distance
    if (wert1 > 0 and wert1 < limit):
        ratio1 = 0.01
        if (wert1 > 0 and wert1 < limit):
            # ratio1 = maxOctave - (wert1 -offset)* (maxOctave - minOctave) / (limit - offset)
            ratio1 = 1.0 - (max(min(wert0, maxDistanz), minDistanz) - minDistanz) / (maxDistanz - minDistanz)
            #			print 'ratio 1: ' + str(ratio1)
            #			sound1.set_volume(ratio1)
            #		else:
            #			sound1.set_volume(0.0)

            #	array = numpy.fromstring(byteString, numpy.int16)
    snd_out0 = lastsound0
    if (ratio0 <= 0.0):
        ratio0 = 0.01
        snd_resample0 = scikits.samplerate.resample(snd_array0, ratio0, "sinc_fastest").astype(snd_array0.dtype)
        snd_out0 = pygame.sndarray.make_sound(snd_resample0)
    # byteString = scikits.samplerate.resample(array, ratio0, "sinc_fastest").astype(numpy.int16).tostring()
    #	stream.write(byteString)
    #	byteString = waveRead.readframes(150) # Read at most 500 samples from the file.
    #	if byteString == '' : # If file is over then rewind.
    #		waveRead.rewind()
    #		byteString = waveRead.readframes(150)
    snd_out1 = lastsound1
    #		array1 = numpy.fromstring(byteString1, numpy.int16)
    if (ratio1 <= 0.0):
        ratio1 = 0.01
    snd_resample1 = scikits.samplerate.resample(snd_array1, ratio1, "sinc_fastest").astype(snd_array1.dtype)
    snd_out1 = pygame.sndarray.make_sound(snd_resample1)

    lastsound0.stop()
    snd_out0.play()
    lastsound0 = snd_out0

    lastsound1.stop()
    snd_out1.play()
    lastsound1 = snd_out1

    i2cBus.write_byte_data(Adresse_SRF02, 0x00, 0x51)
    sleep(0.1)


