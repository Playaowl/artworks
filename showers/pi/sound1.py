import os, sys, wave, pygame, numpy, pyaudio, scikits.samplerate, serial, struct

numSensors=4
limit = 200 #in cm, alles was darueber ist wird ignoriert
minOctave= -1.0
maxOctave= 1.0
"""
minOctave, maxOctave: the highest and lowest pitch changes. 0 is no change.
"""

##self.minOctave = minOctave
##self.maxOctave = maxOctave
##self.width = width
##self.mouseDown = False
ratio0 = 1.0 # The resampling ratio
port="/dev/ttyACM1"
serialFromArduino = serial.Serial(port, 9600)
serialFromArduino.flushInput()




waveRead = wave.open(os.path.join(sys.path[0],
"Downloads/103591__timbre__103539-timbre-15362-djgriffin-tibetan-chant-1-stretchedx3-bassy-tail-3.wav"),
'rb')
sampleRate = waveRead.getframerate()
channels = waveRead.getnchannels()
p = pyaudio.PyAudio()

stream = p.open(format=p.get_format_from_width(waveRead.getsampwidth()),
    channels=waveRead.getnchannels(),
    rate=waveRead.getframerate(),
    output=True)

#fout = open(os.path.join(sys.path[0], "musicdata.txt"), 'w') # For troubleshooting
byteString = waveRead.readframes(100) # Read at most 1000 samples from the file.

while len(byteString) != 0:
    waiting=serialFromArduino.inWaiting()
    if (waiting > numSensors*2-1):
        input=serialFromArduino.read(numSensors*2)
        wert0= struct.unpack_from('>H',buffer(input,0,2))[0]
        print 'wert 0: ' + str(wert0 )
        if (wert0 >0 and wert0<limit):
            ratio0 = 2 ** -(minOctave + wert0 * (maxOctave - minOctave) / limit)
            print 'ratio: ' + str(ratio0)
            
        
    #self.handleEvent(pygame.event.poll()) # This does not wait for an event.
    #fout.write(str(soundOutput.getLeft()) + "\n") # For troubleshooting
    #if soundOutput.getLeft() < 0.2: # If there is less than 0.2 seconds left in the sound buffer.
    array = numpy.fromstring(byteString, dtype=numpy.int16)
    byteString = scikits.samplerate.resample(array, ratio0, "sinc_fastest").astype(numpy.int16).tostring()
    stream.write(byteString)
    byteString = waveRead.readframes(100) # Read at most 500 samples from the file.
    if byteString == '' : # If file is over then rewind.
        waveRead.rewind()
        byteString = waveRead.readframes(100)

waveRead.close()


