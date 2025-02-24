import serial
import time

arduino = serial.Serial('/dev/ttyACM0', 9600, timeout=1)  # Adjust to /dev/ttyACM0 if needed

while True:
    data = arduino.readline().decode().strip()
    if data:
        print("Received:", data)
    time.sleep(1)
