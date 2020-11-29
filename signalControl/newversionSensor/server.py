import socket
import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD)

carGreen = 16
carYellow = 18
carRed=22

personGreen = 7
personRed=3
GPIO.setwarnings(False)

GPIO.setup(carGreen, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(carYellow, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(carRed, GPIO.OUT, initial=GPIO.LOW)

GPIO.setup(personGreen, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(personRed, GPIO.OUT, initial=GPIO.LOW)

lighttime=15

###################################################

HOST = '192.168.0.11'

PORT = 8080

s= socket.socket(socket.AF_INET, socket.SOCK_STREAM)

print('Socket created')

try:
    s.bind((HOST, PORT))
except socket.error:
    print('Bind failed')


s.listen(5)
print ('Socket awaiting messages')

(conn, addr) = s.accept()

print('Connected')

while True:

#    data = conn.recv(1024)
#   print('받은 데이터:' + data.decode('utf-8'))
# reply = ''
#
#    if data.decode('utf-8') == 'time':
#        reply = 'Time'
#
#    else:
 #       reply = 'Unknown command'

    GPIO.output(carRed, GPIO.HIGH)
    GPIO.output(personGreen, GPIO.HIGH)
 #   time.sleep(greenLightTime)
    start= time.time()
    
    while True:
      if int(time.time()-start) % 3 ==0 :
         time.sleep(1)
         end = time.time()
         temp = int(end-start)
         
         MESSAGE = str(lighttime-temp) + 'seconds left'
         if lighttime-temp > 0 :
             conn.send(MESSAGE.encode('utf-8'))
         

      if temp >= lighttime :
         break

    GPIO.output(carRed, GPIO.LOW)
    GPIO.output(personGreen, GPIO.LOW)
    GPIO.output(carGreen, GPIO.HIGH)
    GPIO.output(personRed, GPIO.HIGH)
    MESSAGE= 'green light'
    conn.send(MESSAGE.encode('utf-8'))
    time.sleep(lighttime)   
    
    GPIO.output(carGreen, GPIO.LOW)
    ###보행자용 신호 그대로

    GPIO.output(carYellow, GPIO.HIGH)
    MESSAGE='yellow light'
    conn.send(MESSAGE.encode('utf-8'))
    time.sleep(5)

    GPIO.output(carYellow, GPIO.LOW)
    GPIO.output(personRed, GPIO.LOW)

conn.close()





