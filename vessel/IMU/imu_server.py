import serial
import asyncio
import websockets
import json
import time

# Configure serial port
SERIAL_PORT = '/dev/ttyACM0'  # or '/dev/ttyACM0' depending on your setup
BAUD_RATE = 115200

async def read_imu_data(websocket):
    try:
        # Open serial port
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        print(f"Connected to {SERIAL_PORT}")
        
        while True:
            if ser.in_waiting:
                # Read line from serial
                line = ser.readline().decode('utf-8').strip()
                try:
                    # Parse the comma-separated values
                    roll, pitch, yaw = map(float, line.split(','))
                    
                    # Create data packet
                    data = {
                        'roll': roll,
                        'pitch': pitch,
                        'yaw': yaw,
                        'timestamp': time.time()
                    }
                    
                    # Send data to connected clients
                    await websocket.send(json.dumps(data))
                except ValueError as e:
                    print(f"Error parsing data: {e}")
                    continue
            await asyncio.sleep(0.01)  # Small delay to prevent CPU overuse
            
    except serial.SerialException as e:
        print(f"Serial error: {e}")
    except Exception as e:
        print(f"Error: {e}")
    finally:
        if 'ser' in locals():
            ser.close()

async def main():
    async with websockets.serve(read_imu_data, "0.0.0.0", 8765):
        print("WebSocket server started on ws://0.0.0.0:8765")
        await asyncio.Future()  # run forever

if __name__ == "__main__":
    asyncio.run(main()) 