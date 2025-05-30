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

# Temporarily comment out the serial reader function
# async def imu_reader():
#     ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
#     print(f"Connected to {SERIAL_PORT}")
#     while True:
#         if ser.in_waiting:
#             try:
#                 line = ser.readline().decode('utf-8', errors='ignore').strip()
#                 print(f"Received line: {line}")  # Debug print
#             except Exception as e:
#                 print(f"Serial decode error: {e}")
#                 continue

#             # Change regex back to expect comma-separated values
#             match = re.match(r'^-?\d+(\.\d+)?,-?\d+(\.\d+)?,-?\d+(\.\d+)?$', line)
#             if match:
#                 try:
#                     # Change parsing back to split comma-separated values
#                     roll, pitch, yaw = map(float, line.split(','))
#                     data = {
#                         'roll': roll,
#                         'pitch': pitch,
#                         'yaw': yaw,
#                         'timestamp': time.time()
#                     }
#                     print(f"Parsed data: {data}")  # Debug print

#                     # Log to plain text file
#                     with open(LOG_FILE, "a") as logf:
#                         logf.write(f"{data['timestamp']},{data['roll']},{data['pitch']},{data['yaw']}\n")

#                     # Log to CSV file
#                     with open(CSV_FILE, "a", newline="") as csvf:
#                         writer = csv.writer(csvf)
#                         writer.writerow([data['timestamp'], data['roll'], data['pitch'], data['yaw']])

#                     # Broadcast to all connected clients with robust error handling
#                     if connected_clients:
#                         msg = json.dumps(data)
#                         print(f"Attempting to send to {len(connected_clients)} clients: {msg}")  # Debug print
                        
#                         send_tasks = []
#                         valid_clients = []
#                         for client in list(connected_clients): # Iterate over a copy
#                             # Explicitly check if the client object is a WebSocketServerProtocol and is not closed
#                             if isinstance(client, websockets.WebSocketServerProtocol) and not client.closed:
#                                 send_tasks.append(client.send(msg))
#                                 valid_clients.append(client)
#                             else:
#                                 # Optional: Log if an unexpected object or a closed client is in the set
#                                 if not isinstance(client, websockets.WebSocketServerProtocol):
#                                      print(f"Warning: Unexpected object type in connected_clients: {type(client)}")
#                                 # Do not add to send_tasks if not a valid, open websocket

#                         if send_tasks:
#                             print(f"Actually sending to {len(valid_clients)} valid clients.")
#                             await asyncio.gather(*send_tasks, return_exceptions=True)
#                         else:
#                              print("No valid connected clients to send to.")

#                     else:
#                         print("No connected clients")  # Debug print
#                 except Exception as e:
#                     print(f"Error parsing data: {e}")
#             else:
#                 # Optionally print non-data lines for debugging
#                 if line:
#                     print(f"Ignored line: {line}")
#         await asyncio.sleep(0.01)

async def handler(websocket):
    # Explicitly check and add only if it's the expected protocol type
    if isinstance(websocket, websockets.WebSocketServerProtocol):
        connected_clients.add(websocket)
        print(f"WebSocket client connected. Total clients: {len(connected_clients)}")
        try:
            # Wait for the websocket to close
            await websocket.wait_closed()
        finally:
            # Explicitly check and remove only if it's the expected protocol type and is in the set
            if isinstance(websocket, websockets.WebSocketServerProtocol) and websocket in connected_clients:
                 connected_clients.remove(websocket)
                 print(f"WebSocket client disconnected. Remaining clients: {len(connected_clients)}")
            # Optional: Log if a non-protocol object was somehow in the set during disconnect
            elif not isinstance(websocket, websockets.WebSocketServerProtocol):
                 print(f"Warning: Non-protocol object attempting to disconnect: {type(websocket)}")
    else:
        print(f"Warning: Handler received unexpected object type: {type(websocket)}. Ignoring connection.")
        # If it's not the right type, maybe close it just in case
        if hasattr(websocket, 'close'):
             await websocket.close()


async def main():
    # Temporarily commented out the serial reader task
    # asyncio.create_task(imu_reader())
    # Start the WebSocket server
    async with websockets.serve(handler, "0.0.0.0", 8765):
        print("WebSocket server started on ws://0.0.0.0:8765")
        await asyncio.Future()  # run forever

if __name__ == "__main__":
    asyncio.run(main()) 