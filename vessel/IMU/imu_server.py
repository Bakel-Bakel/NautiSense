import serial
import asyncio
import websockets
import json
import time
import re
import csv
import os

# Configure serial port
SERIAL_PORT = '/dev/ttyACM0'  # or '/dev/ttyACM0' depending on your setup
BAUD_RATE = 115200

LOG_FILE = "imu_data.log"
CSV_FILE = "imu_data.csv"

# Create CSV file with headers if it doesn't exist
if not os.path.exists(CSV_FILE):
    with open(CSV_FILE, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["timestamp", "roll", "pitch", "yaw"])

connected_clients = set()

async def imu_reader():
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    print(f"Connected to {SERIAL_PORT}")
    while True:
        if ser.in_waiting:
            try:
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                print(f"Received line: {line}")  # Debug print
            except Exception as e:
                print(f"Serial decode error: {e}")
                continue

            match = re.match(r'Roll: ([\-0-9.]+) \| Pitch: ([\-0-9.]+) \| Yaw: ([\-0-9.]+)', line)
            if match:
                try:
                    roll = float(match.group(1))
                    pitch = float(match.group(2))
                    yaw = float(match.group(3))
                    data = {
                        'roll': roll,
                        'pitch': pitch,
                        'yaw': yaw,
                        'timestamp': time.time()
                    }
                    print(f"Parsed data: {data}")  # Debug print

                    # Log to plain text file
                    with open(LOG_FILE, "a") as logf:
                        logf.write(f"{data['timestamp']},{data['roll']},{data['pitch']},{data['yaw']}\n")

                    # Log to CSV file
                    with open(CSV_FILE, "a", newline="") as csvf:
                        writer = csv.writer(csvf)
                        writer.writerow([data['timestamp'], data['roll'], data['pitch'], data['yaw']])

                    # Broadcast to all connected clients
                    if connected_clients:
                        msg = json.dumps(data)
                        print(f"Sending to {len(connected_clients)} clients: {msg}")  # Debug print
                        await asyncio.gather(*(client.send(msg) for client in connected_clients))
                    else:
                        print("No connected clients")  # Debug print
                except Exception as e:
                    print(f"Error parsing data: {e}")
            else:
                # Optionally print non-data lines for debugging
                if line:
                    print(f"Ignored line: {line}")
        await asyncio.sleep(0.01)

async def handler(websocket):
    connected_clients.add(websocket)
    print(f"WebSocket client connected. Total clients: {len(connected_clients)}")
    try:
        await websocket.wait_closed()
    finally:
        connected_clients.remove(websocket)
        print(f"WebSocket client disconnected. Remaining clients: {len(connected_clients)}")

async def main():
    # Start the serial reader task
    asyncio.create_task(imu_reader())
    # Start the WebSocket server
    async with websockets.serve(handler, "0.0.0.0", 8765):
        print("WebSocket server started on ws://0.0.0.0:8765")
        await asyncio.Future()  # run forever

if __name__ == "__main__":
    asyncio.run(main()) 