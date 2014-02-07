import os, sys, time, wave, pygame, pygame.mixer, numpy, pyaudio, scikits.samplerate, serial, struct

numSensors=4
limit = 250 #in cm, alles was darueber ist wird ignoriert
minDistanz = 50.0
maxDistanz=150.0
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
pygame.mixer.init(44100, -16,1,1024)
sound0= pygame.mixer.Sound("Downloads/shower.wav")
sound1= pygame.mixer.Sound("Downloads/om.wav")
#sound2= pygame.mixer.Sound("Downloads/shower.wav")
#sound3= pygame.mixer.Sound("Downloads/om.wav")
soundChannel0= pygame.mixer.Channel(0)
soundChannel1= pygame.mixer.Channel(1)
#soundChannel2= pygame.mixer.Channel(2)
#soundChannel3= pygame.mixer.Channel(3)
sound0.set_volume(0.0)
sound1.set_volume(0.0)
#sound2.set_volume(0.0)
#sound3.set_volume(0.0)
soundChannel0.play(sound0, -1)
soundChannel1.play(sound1, -1)
#soundChannel2.play(sound2, -1)
#soundChannel3.play(sound3, -1)
inputString=serialFromArduino.readline();


while True:
	inputString=serialFromArduino.readline();
	serialFromArduino.write('\r')
#	print 'inputString: '+inputString
	inputString=inputString.strip();
	sensors=inputString.split(',')
	if (inputString[:1].isdigit() and len(sensors) >0):
		wert0= int(sensors[0])
#		print 'wert0: '+str(wert0)
		if (wert0 >0 and wert0< limit):
			ratio0 = maxOctave - (wert0-offset) / (limit-offset)
			ratio0 = 1.0-(max(min(wert0,maxDistanz),minDistanz)-minDistanz)/(maxDistanz-minDistanz)
#			print('max(min(wert0,250.0),100.0)/150'+str( max(min(wert0,250.0),100.0)/150.0))
#			print 'ratio 0: ' + str(ratio0)
			sound0.set_volume(ratio0)
		else:
			sound0.set_volume(0.0)
			#print 'wert 1: ' + str(wert1 )
	if (len(sensors)>1):
		wert1= int(sensors[1])
#		print 'wert1: '+str(wert1)
		if (wert1 >0 and wert1< limit):
			#ratio1 = maxOctave - (wert1 -offset)* (maxOctave - minOctave) / (limit - offset)
			ratio1 = 1.0-(max(min(wert0,maxDistanz),minDistanz)-minDistanz)/(maxDistanz-minDistanz)
#			print 'ratio 1: ' + str(ratio1)
			sound1.set_volume(ratio1)
		else:
			sound1.set_volume(0.0)
	if (len(sensors)>2):
		wert2= int(sensors[2])
#		print 'wert2: '+str(wert2)
		if (wert2 >0 and wert2<limit):
		#ratio2 = maxOctave - (wert2 -offset)* (maxOctave - minOctave) / (limit - offset)
			ratio2 = 1.0-(max(min(wert2,250.0),100.0)-100.0)/150.0
		#print 'ratio 2: ' + str(ratio2)
			sound2.set_volume(ratio2)
		else:
			sound2.set_volume(0.0)
	if (len(sensors)>3):
		wert3= int(sensors[3])
#		print 'wert3: '+str(wert3)
		if(wert3 >0 and wert3<limit):
		#ratio3 = maxOctave - (wert3 -offset)* (maxOctave - minOctave) / (limit - offset)
			ratio3 = 1.0-(max(min(wert3,250.0),100.0)-100.0)/150.0
		#print 'ratio 3: ' + str(ratio3)
			sound3.set_volume(ratio3)
		else:
			sound3.set_volume(0.0)


