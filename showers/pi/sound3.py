import os, sys, time, wave, pygame, pygame.mixer, numpy, pyaudio, pygame.sndarray, scikits.samplerate, serial, struct

numSensors=2
limit = 250 #in cm, alles was darueber ist wird ignoriert
minDistanz = 20.0
maxDistanz=250.0
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
pygame.mixer.init(44100, -16,2,4096)
#waveRead = wave.open(os.path.join(sys.path[0],'Downloads/shower.wav'),'rb')
#waveRead1 = wave.open(os.path.join(sys.path[0],'Downloads/om.wav'),'rb')

#sampleRate = waveRead.getframerate()
#channels = waveRead.getnchannels()
#p = pyaudio.PyAudio()
#stream = p.open(format=p.get_format_from_width(waveRead.getsampwidth()),
 #   channels=waveRead.getnchannels(),
#    rate=waveRead.getframerate(),
#    output=True)
#stream1 = p.open(format=p.get_format_from_width(waveRead1.getsampwidth()),
#    channels=waveRead1.getnchannels(),
#    rate=waveRead1.getframerate(),
#    output=True)
#print ("format: "+str(p.get_format_from_width(waveRead.getsampwidth()))+", #channels: "+str(waveRead.getnchannels())+",  rate: "+str(waveRead.getframerate()))
#print ("format: "+str(p.get_format_from_width(waveRead1.getsampwidth()))+", #channels: "+str(waveRead1.getnchannels())+",  rate: #"+str(waveRead1.getframerate()))

sound0= pygame.mixer.Sound("Downloads/shower.wav")
sound1= pygame.mixer.Sound("Downloads/om.wav")
#sound2= pygame.mixer.Sound("Downloads/shower.wav")
#sound3= pygame.mixer.Sound("Downloads/om.wav")
#soundChannel0= pygame.mixer.Channel(0)
#soundChannel1= pygame.mixer.Channel(1)
#soundChannel2= pygame.mixer.Channel(2)
#soundChannel3= pygame.mixer.Channel(3)
sound0.set_volume(1.0)
sound1.set_volume(1.0)
#sound2.set_volume(0.0)
#sound3.set_volume(0.0)
#soundChannel0.play(sound0, -1)
#soundChannel1.play(sound1, -1)
#soundChannel2.play(sound2, -1)
#soundChannel3.play(sound3, -1)
#sound0.play(-1)
#sound1.play(-1)
snd_array0=pygame.sndarray.array(sound0)
snd_array1=pygame.sndarray.array(sound1)
lastsound0=sound0
lastsound1=sound1
#byteString = waveRead.readframes(150) # Read at most 1000 samples from the file.
#byteString1 = waveRead1.readframes(150) # Read at most 1000 samples from the file.
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
		ratio0=0.01
		if (wert0 >0 and wert0< limit):
#			ratio0 = maxOctave - (wert0-offset) / (limit-offset)
			ratio0 = 1.0-(max(min(wert0,maxDistanz),minDistanz)-minDistanz)/(maxDistanz-minDistanz)
#			print('max(min(wert0,250.0),100.0)/150'+str( max(min(wert0,250.0),100.0)/150.0))
#			print 'ratio 0: ' + str(ratio0)
#			sound0.set_volume(ratio0)
#		else:
#			sound0.set_volume(0.0)
			#print 'wert 1: ' + str(wert1 )
	if (len(sensors)>1):
		wert1= int(sensors[1])
#		print 'wert1: '+str(wert1)
		ratio1=0.01
		if (wert1 >0 and wert1< limit):
			#ratio1 = maxOctave - (wert1 -offset)* (maxOctave - minOctave) / (limit - offset)
			ratio1 = 1.0-(max(min(wert0,maxDistanz),minDistanz)-minDistanz)/(maxDistanz-minDistanz)
#			print 'ratio 1: ' + str(ratio1)
#			sound1.set_volume(ratio1)
#		else:
#			sound1.set_volume(0.0)
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
    
#	array = numpy.fromstring(byteString, numpy.int16)
	snd_out0=lastsound0
	if (ratio0 <= 0.0):
		ratio0=0.01
		snd_resample0 = scikits.samplerate.resample(snd_array0, ratio0, "sinc_fastest").astype(snd_array0.dtype)
		snd_out0 = pygame.sndarray.make_sound(snd_resample0)
#	byteString = scikits.samplerate.resample(array, ratio0, "sinc_fastest").astype(numpy.int16).tostring()
#	stream.write(byteString)
#	byteString = waveRead.readframes(150) # Read at most 500 samples from the file.
#	if byteString == '' : # If file is over then rewind.
#		waveRead.rewind()
#		byteString = waveRead.readframes(150)
	snd_out1=lastsound1
	if (len(sensors)>1):
#		array1 = numpy.fromstring(byteString1, numpy.int16)
		if (ratio1 <= 0.0):
			ratio1=0.01
		snd_resample1 = scikits.samplerate.resample(snd_array1, ratio1, "sinc_fastest").astype(snd_array1.dtype)
		snd_out1 = pygame.sndarray.make_sound(snd_resample1)

	lastsound0.stop()
	snd_out0.play()
	lastsound0=snd_out0

	if (len(sensors)>1):
		lastsound1.stop()
		snd_out1.play()
		lastsound1=snd_out1

		#		byteString1 = scikits.samplerate.resample(array, ratio1, "sinc_fastest").astype(numpy.int16).tostring()
#		stream1.write(byteString1)
#		byteString1 = waveRead1.readframes(150) # Read at most 500 samples from the file.
#		if byteString1 == '' : # If file is over then rewind.
#			waveRead1.rewind()
#			byteString1 = waveRead1.readframes(150)

		
		
		
		
#waveRead.close()

