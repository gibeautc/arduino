import sys,serial,argparse,datetime
#import Tkinter,tkMessageBox
import numpy as np
from time import sleep
from collections import deque

import matplotlib.pyplot as plt
import matplotlib.animation as animation
#class Index(object):
#    ind=0

#    def up(self,event):
 #       self.ind+=1


#plot class
class AnalogPlot:
        #constr
        def __init__(self,strPort, maxLen):
                #open serial port
                self.ser=serial.Serial(strPort,9600)
                self.ax=deque([0.0]*maxLen)
                self.ay=deque([0.0]*maxLen)
                self.maxLen=maxLen
                File_name=str(datetime.datetime.now())
                self.ofile=open('Logs/'+File_name,'w')
                print('Init Complete')

        #add to buffer
        def addToBuf(self,buf,val):
                if len(buf)<self.maxLen:
                        buf.append(val)
                else:
                        buf.pop()
                        buf.appendleft(val)

        #add data
        def add(self,data):
                assert(len(data)==2)
                self.addToBuf(self.ax,data[0])
                self.addToBuf(self.ay,data[1])

        #update plot
        def update(self,frameNum,a0,a1):
                try:
                    line=self.ser.readline()
                    
                    print(line)
                    #CHANGE FOR INT VALUES OF TEMP
                    data=[float(val) for val in line.split()]
                    #print data
                    if(len(data)==2):
                        self.add(data)
                        self.ofile.write(str(line))
                        a0.set_data(range(self.maxLen),self.ax)
                        a1.set_data(range(self.maxLen),self.ay)
                except KeyboardInterrupt:
                    print('exiting')

                return a0,
        #clean up
        def close(self):
                #close serial
                self.ser.flush()
                self.ser.close()
                #close file
                self.ofile.close

def Mbox(title,text):
    #ctypes.windll.user32.MessageBoxW(0,text,title,style)
    tkMessageBox.showinfo(title,text)

#main() function
def main():
        print('Starting up...')
        strPort='/dev/ttyACM1'

        print('Reading from serial port %s...' %strPort)

        #plot Parameters
        analogPlot=AnalogPlot(strPort,100)
        print('Plotting Data')

        #setup animation
        fig=plt.figure()
        ax=plt.axes(xlim=(0,100),ylim=(0,500))
        a0,=ax.plot([],[])
        a1,=ax.plot([],[])
        anim=animation.FuncAnimation(fig,analogPlot.update,fargs=(a0,a1),interval=50)

        #show plot
        plt.show()

        #clean up
        analogPlot.close()


        print('exiting.')

#call main
main()





























