import matplotlib.pyplot as pyplot
import serial
import sys
import time
from PIL import Image
import webbrowser
PORT='/dev/ttyACM1'
ser= serial.Serial(PORT, 115200,timeout=1)
while TRUE:
    ser.write(x)
    time.sleep

#x=[1,2,3,4,5,6,7,8,9,10]
#y=[1,2,3,4,5,6,7,8,9,10]
#pyplot.plot(x)
#pyplot.show()
#float x(100)
x=[None] *100
for i in range(0,10):
    x[i]=i*i
    pyplot.plot(x)
    #pyplot.show(block=False)
    pyplot.savefig('test.png')
    #graph=Image.open('test.png')
    #graph.show()
    webbrowser.open('test.png')
    time.sleep(1)
    #graph.close()

    #figure1.close()
