import serial
import time
import signal
import matplotlib.pyplot as plt
import numpy as np

ser = serial.Serial()
ser.port = '/dev/tty.WALL-E-DevB'
ser.baudrate = 9600
ser.timeout = 0.5

def sighandler(signum, frame):
    try:
        res = input("Ctrl-C pressed. Exit? y/n ")
        if res == 'y':
            raise("Exiting")
    except:
        ser.write(b"e")
        ser.close()
        print("bye")
        exit(1)

signal.signal(signal.SIGINT, sighandler)

print("Hello!")

ser.open()
print("Connected to " + ser.name)

while True:
    time.sleep(0.1)
    ser.write(b"r")
    input("\n\nPress enter for race start!")
    ser.reset_input_buffer()
    ser.write(b"c")

    # Keep track of my own time
    timestart = time.time()
    datarecv = []
    mazedone = False

    while not(mazedone):
        inp = ser.readline()
        if len(inp) > 0:
            print("RECV:",inp)
            if chr(inp[0]) == ":":
                print("DATA: ", inp[2:22])
                datarecv.extend(inp[2:22])
            elif chr(inp[0]) == "T":
                timestop = time.time()
                print("Maze Done! TIME PC", timestop - timestart, sep=' ')
                x = np.linspace(0, len(datarecv) / 20, num=len(datarecv))
                y = np.array(datarecv)
                plt.title("Race Data Team 1")
                plt.xlabel("Time [0.1s]")
                plt.ylabel("Error [ADC Units]")
                plt.ylim((0, 255))
                plt.xlim((0, len(datarecv) / 20))
                plt.plot(x, y, color="red", marker="o")
                plt.show()
                mazedone = True
                break

