import clientP as c
import time
import Adafruit_DHT as dht
import Adafruit_BBIO.GPIO as GPIO

c.initClient("192.168.7.1",5000)
GPIO.setup("P8_10",GPIO.OUT)

while True:
    humi, temp = dht.read_retry(dht.DHT11,"P8_11")
    print 'Temp={0:0.1f}*  Humidity={1:0.1f}%'.format(temp, humi)
    answ = c.storeMessage(temp,humi)

    if answ == 1:
        GPIO.output("P8_10",GPIO.HIGH)
    elif answ == 2:
        GPIO.output("P8_10",GPIO.LOW)
    
    time.sleep(10)
