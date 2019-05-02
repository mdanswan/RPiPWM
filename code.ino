import RPi.GPIO as g
import time as t

# pins
LED = 15 

TRIG = 11
ECHO = 13

# speed of sound in metres
SOS_M = 343

# Out of Range (OOR)
OOR = 24

# set pinmode
g.setmode(g.BOARD)

# setup pins
g.setup(LED, g.OUT);

g.setup(TRIG, g.OUT)
g.setup(ECHO, g.IN)

# turn off led
g.output(LED, g.LOW)

# pwm init
p = g.PWM(LED, 100)
# begin with a 100% duty cycle
p.start(100)

def pulse():
    #reset the trig cycle
    g.output(TRIG, g.LOW)
    t.sleep(0.5)

    # trigger the pulse
    g.output(TRIG, g.HIGH)
    t.sleep(0.00001);
    g.output(TRIG, g.LOW)

    #measure the time ECHO is high
    start = t.time()
    finish = t.time()

    while g.input(ECHO) == g.LOW:
        start = t.time()
        
    while g.input(ECHO) == g.HIGH:
        finish = t.time()
        
    # calculate difference between timestamps
    pulse_len = finish - start

    # calculate distance (metres)
    distance = (pulse_len * SOS_M) / 2
    
    t.sleep(1);
    
    # change the duty cycle orcording to the distance detected
    # the sensor is commonly representing a value near 24 as being
    # its 'out of range' value. Im using the OOR value and the distance value
    # to calculate the appropriate duty cycle.
    try:
        p.ChangeDutyCycle(100 / OOR * distance)
    except ValueError:
        print("Unable to change duty cycle")
    except:
        print("something unexpected went wrong ;{")
        
    return distance

try:
    while (1):
        print(pulse())
except KeyboardInterrupt:
    sys.exit(0);

g.cleanup()
