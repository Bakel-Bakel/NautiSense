##Raspberry Command Book


  ```bash
  sudo apt install nmap
  nmap -sn 192.168.1.0/24
  ```
```bash
ping raspberrypi5.local
```
If you get a response, it means the Pi is online.

---

## **✅ Step 3: Connect to Raspberry Pi via SSH**
Now, **log into the Raspberry Pi from your laptop** using SSH.

1. Open a **terminal** (Mac/Linux) or **PowerShell** (Windows).
2. Connect via SSH:
   ```bash
   ssh bakelbakel@raspberrypi5.local
   ```
   *(Replace `raspberrypi5.local` with the IP if needed, e.g., `ssh bakelbakel@192.168.1.100`)*
3. **Enter your password** (the one you set in OS customization).

If successful, you should see a welcome message like:
```
Linux raspberrypi5 5.15.0-rpi #1 SMP Debian
bakelbakel@raspberrypi5:~$
```
🎉 **You're now inside the Raspberry Pi!**

---

## **✅ Step 4: Update the Raspberry Pi (First Boot)**
Before doing anything else, update your Pi’s software:
```bash
sudo apt update && sudo apt upgrade -y
```
This ensures your Raspberry Pi is fully updated.

---

## **✅ Step 5: Enable Serial Port for Arduino Communication**
Since you'll be **connecting the Arduino via USB**, make sure the serial port is enabled.

1. Run:
   ```bash
   sudo raspi-config
   ```
2. Go to:
   ```
   Interfacing Options → Serial
   ```
3. Select:
   - **"No"** for login shell over serial.
   - **"Yes"** for enabling serial hardware.
4. Exit and reboot:
   ```bash
   sudo reboot
   ```

---

## **✅ Step 6: Connect Your Arduino to the Raspberry Pi**
Since your **Arduino has a USB Type-B port**, you can **connect it directly** to the Raspberry Pi using a **USB cable**.

### **🔹 Check if the Arduino is Detected**
Once plugged in, run:
```bash
ls /dev/ttyUSB* /dev/ttyACM*
```
If it shows something like `/dev/ttyUSB0` or `/dev/ttyACM0`, your Arduino is connected!

---

## **✅ Step 7: Test Serial Communication**
Now, let's test if the Raspberry Pi can receive data from the Arduino.

### **1️⃣ Upload This Code to Your Arduino**
Use the **Arduino IDE** on your laptop to upload this sketch:
```cpp
void setup() {
    Serial.begin(9600);  // Start serial communication
}

void loop() {
    Serial.println("Hello from Arduino!");  // Send test message
    delay(1000);  // Send every 1 second
}
```
---

### **2️⃣ Read Serial Data on Raspberry Pi**
On the Raspberry Pi, create a Python script:
```bash
nano arduino_serial.py
```
Paste this code inside:
```python
import serial
import time

arduino = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)  # Adjust to /dev/ttyACM0 if needed

while True:
    data = arduino.readline().decode().strip()
    if data:
        print("Received:", data)
    time.sleep(1)
```
Save and exit (**CTRL + X → Y → Enter**).

---

### **3️⃣ Run the Python Script**
Run:
```bash
python3 arduino_serial.py
```
If everything works, you should see:
```
Received: Hello from Arduino!
Received: Hello from Arduino!
```

---

## **🚀 Next Steps**
Now that your Raspberry Pi and Arduino are connected, you can:
✔ **Modify the Arduino to send sensor data** (e.g., temperature readings).  
✔ **Process the data on the Raspberry Pi** (log it, send to the cloud).  
✔ **Run the script automatically at boot**.  

Would you like help with **data logging**, **cloud integration**, or **running the script automatically** on startup? 🚀
