import cv #Import functions from OpenCV
cv.NamedWindow('a_window', cv.CV_WINDOW_AUTOSIZE)
capture1=cv.CaptureFromCAM(0)
# Captures come with set and get methods.
#cv.GetCaptureProperty()
#cv.SetCaptureProperty()
image1=cv.QueryFrame(capture1)
image=cv.LoadImage('/home/holger/Bilder/Drops_Imapct.JPG', cv.CV_LOAD_IMAGE_COLOR) #Load the image
font = cv.InitFont(cv.CV_FONT_HERSHEY_SIMPLEX, 1, 1, 0, 3, 8) #Creates a font
x = 100
y = 100
cv.PutText(image,"Hello World!!!", (x,y),font, 255) #Draw the text
cv.ShowImage('a_window', image) #Show the image
#cv.Waitkey(10000)
cv.WaitKey(20000)
cv.SaveImage('image.png', image) #Saves the image
