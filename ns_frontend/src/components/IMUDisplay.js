import React, { useState, useEffect } from 'react';

const IMUDisplay = () => {
    const [imuData, setImuData] = useState({
        roll: 0,
        pitch: 0,
        yaw: 0,
        timestamp: 0
    });
    const [connected, setConnected] = useState(false);

    useEffect(() => {
        console.log('Setting up WebSocket connection...');
        const ws = new WebSocket('ws://192.168.10.91:8765');

        ws.onopen = () => {
            console.log('Connected to IMU WebSocket');
            setConnected(true);
        };

        ws.onclose = () => {
            console.log('Disconnected from IMU WebSocket');
            setConnected(false);
        };

        ws.onerror = (error) => {
            console.error('WebSocket error:', error);
        };

        ws.onmessage = (event) => {
            console.log('Received message:', event.data);
            try {
                const data = JSON.parse(event.data);
                console.log('Parsed IMU data:', data);
                setImuData(data);
            } catch (error) {
                console.error('Error parsing IMU data:', error);
            }
        };

        return () => {
            console.log('Cleaning up WebSocket connection...');
            ws.close();
        };
    }, []);

    return (
        <div className="imu-display">
            <h2>IMU Data</h2>
            <div className="connection-status">
                Status: {connected ? 'Connected' : 'Disconnected'}
            </div>
            <div className="imu-values">
                <div className="imu-value">
                    <label>Roll:</label>
                    <span>{imuData.roll.toFixed(2)}°</span>
                </div>
                <div className="imu-value">
                    <label>Pitch:</label>
                    <span>{imuData.pitch.toFixed(2)}°</span>
                </div>
                <div className="imu-value">
                    <label>Yaw:</label>
                    <span>{imuData.yaw.toFixed(2)}°</span>
                </div>
            </div>
            <style jsx>{`
                .imu-display {
                    padding: 20px;
                    background: #f5f5f5;
                    border-radius: 8px;
                    box-shadow: 0 2px 4px rgba(0,0,0,0.1);
                }
                .connection-status {
                    margin: 10px 0;
                    padding: 5px;
                    background: ${connected ? '#4CAF50' : '#f44336'};
                    color: white;
                    border-radius: 4px;
                    text-align: center;
                }
                .imu-values {
                    display: grid;
                    grid-template-columns: repeat(3, 1fr);
                    gap: 20px;
                    margin-top: 20px;
                }
                .imu-value {
                    text-align: center;
                    padding: 15px;
                    background: white;
                    border-radius: 6px;
                    box-shadow: 0 1px 3px rgba(0,0,0,0.1);
                }
                .imu-value label {
                    display: block;
                    font-weight: bold;
                    margin-bottom: 5px;
                    color: #666;
                }
                .imu-value span {
                    font-size: 1.5em;
                    color: #2196F3;
                }
            `}</style>
        </div>
    );
};

export default IMUDisplay; 