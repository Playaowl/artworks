import cv2
import serial
port="/dev/ttyACM1"
serialFromArduino=serial.Serial(port,9600)
serialFromArduino.flushInput()
cam = cv2.VideoCapture(2)
s, img = cam.read()
 
winName = "Movement Indicator"
cv2.namedWindow(winName, cv2.CV_WINDOW_AUTOSIZE)
 
while s:
    if(serialFromArduino.inWaiting()>0):
        input = serialFromArduino.read(1)
        num=ord(input)
    cv2.imshow( winName + str(num) ,img )
     
    s, img = cam.read()
     
    key = cv2.waitKey(10)
    if key == 27:
        cv2.destroyWindow(winName)
        break
     
print "Goodbye"
