import os, sys, time, wave, pygame, pygame.mixer, numpy, pyaudio, scikits.samplerate, serial, struct

numSensors=4
limit = 250 #in cm, alles was darueber ist wird ignoriert
minOctave= 0.0
maxOctave= 1.0
offset=100
"""
minOctave, maxOctave: the highest and lowest pitch changes. 0 is no change.
"""

ratio0 = 1.0 # The resampling ratio
port="/dev/ttyACM0"
serialFromArduino = serial.Serial(port, 9600)
serialFromArduino.flushInput()
pygame.mixer.init(48000, -16,1,4096)
sound0= pygame.mixer.Sound("Downloads/103591__timbre__103539-timbre-15362-djgriffin-tibetan-chant-1-stretchedx3-bassy-tail-3.wav")
sound1= pygame.mixer.Sound("Downloads/64620__falsalama__om-gate-gate-paragate-parasamgate-bodhi-soha.wav")
soundChannel0= pygame.mixer.Channel(0)
soundChannel1= pygame.mixer.Channel(1)
sound0.set_volume(0.0)
sound1.set_volume(0.0)
soundChannel0.play(sound0, -1)
soundChannel1.play(sound1, -1)
inputString=serialFromArduino.readline();


while True:
	inputString=serialFromArduino.readline();
	serialFromArduino.write('\r')
	print 'inputString: '+inputString
	inputString=inputString.strip();
	sensors=inputString.split(',')
	if ( len(sensors) ==2):
		wert0= int(sensors[0])
		wert1= int(sensors[1])
		# wert2= int(sensors[2])
		# wert3= int(sensors[3])
		if (wert0 > 1000):
			#print 'skipping wert 0: ' + str(wert0 )+' wert 1: ' + str(wert1 )
			continue;
		#print 'wert 0: ' + str(wert0 )+' wert 1: ' + str(wert1 )
#		if (wert0<limit or wert1<limit or wert2<limit or wert3<limit ):
		if (wert0<limit or wert1<limit ):
			sound0.set_volume(1.0)
			sound1.set_volume(1.0)
			print 'sound ' 
		else:
			sound0.set_volume(0.0)
			sound1.set_volume(0.0)
	print 'next'
	
#        if (wert0 >0 and wert0<limit):
#            #ratio0 = maxOctave - (wert0-offset) / (limit-offset)
#	    ratio0 = 1.0-(max(min(wert0,250.0),100.0)-100.0)/150.0
#	    #print('max(min(wert0,250.0),100.0)/150'+str( max(min(wert0,250.0),100.0)/150.0))
#            #print 'ratio 0: ' + str(ratio0)
#            sound0.set_volume(1.0)
#        else:
#            sound0.set_volume(0.0)
#        wert1= struct.unpack_from('>H',buffer(input,2,2))[0]
#        #print 'wert 1: ' + str(wert1 )
#        if (wert1 >0 and wert1<limit):
#            #ratio1 = maxOctave - (wert1 -offset)* (maxOctave - minOctave) / (limit - offset)
#	    ratio1 = 1.0-(max(min(wert1,250.0),100.0)-100.0)/150.0
#            #print 'ratio 1: ' + str(ratio1)
#            sound1.set_volume(1.0)
#        else:
#            sound1.set_volume(0.0)
#        wert2= struct.unpack_from('>H',buffer(input,4,2))[0]
#        #print 'wert 2: ' + str(wert2 )
#        if (wert2 >0 and wert2<limit):
#	    #ratio2 = maxOctave - (wert2 -offset)* (maxOctave - minOctave) / (limit - offset)
#	    ratio2 = 1.0-(max(min(wert2,250.0),100.0)-100.0)/150.0
#            #print 'ratio 2: ' + str(ratio2)
#            sound2.set_volume(1.0)
#        else:
#            sound2.set_volume(0.0)
#        wert3= struct.unpack_from('>H',buffer(input,6,2))[0]
#        print 'wert 0: ' + str(wert0 )+' wert 1: ' + str(wert1 )+' wert 2: ' + str(wert2 )+' wert 3: ' + str(wert3 )
#        if (wert3 >0 and wert3<limit):
#            #ratio3 = maxOctave - (wert3 -offset)* (maxOctave - minOctave) / (limit - offset)
#	    ratio3 = 1.0-(max(min(wert3,250.0),100.0)-100.0)/150.0
#            #print 'ratio 3: ' + str(ratio3)
#            sound3.set_volume()
#        else:
#            sound3.set_volume(0.0)


