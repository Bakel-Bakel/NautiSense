import React, { useState, useEffect } from 'react';
import { ethers } from 'ethers';
import BoatData from '../artifacts/contracts/BoatData.sol/BoatData.json';

const BlockchainData = ({ boatId, currentData }) => {
    const [contract, setContract] = useState(null);
    const [account, setAccount] = useState(null);
    const [latestData, setLatestData] = useState(null);
    const [error, setError] = useState(null);
    const [isRecording, setIsRecording] = useState(false);

    useEffect(() => {
        const initBlockchain = async () => {
            try {
                // Check if MetaMask is installed
                if (typeof window.ethereum !== 'undefined') {
                    // Request account access
                    const accounts = await window.ethereum.request({ method: 'eth_requestAccounts' });
                    setAccount(accounts[0]);

                    // Create provider and signer
                    const provider = new ethers.BrowserProvider(window.ethereum);
                    const signer = await provider.getSigner();

                    // Contract address - replace with your deployed contract address
                    const contractAddress = "0x5FbDB2315678afecb367f032d93F642f64180aa3";
                    
                    // Create contract instance
                    const contractInstance = new ethers.Contract(
                        contractAddress,
                        BoatData.abi,
                        signer
                    );
                    
                    setContract(contractInstance);
                } else {
                    setError("Please install MetaMask to use this feature");
                }
            } catch (err) {
                setError(err.message);
            }
        };

        initBlockchain();
    }, []);

    // Effect to record data when isRecording is true and we have new data
    useEffect(() => {
        const recordDataToBlockchain = async () => {
            if (isRecording && contract && boatId && currentData) {
                try {
                    const tx = await contract.recordData(
                        boatId,
                        Math.round(currentData.roll * 100), // Convert to integer with 2 decimal precision
                        Math.round(currentData.pitch * 100),
                        Math.round(currentData.yaw * 100)
                    );
                    await tx.wait();
                    await fetchLatestData();
                } catch (err) {
                    setError(err.message);
                }
            }
        };

        recordDataToBlockchain();
    }, [isRecording, currentData, contract, boatId]);

    const fetchLatestData = async () => {
        if (!contract || !boatId) return;

        try {
            const data = await contract.getLatestData(boatId);
            setLatestData({
                roll: (Number(data[0]) / 100).toFixed(2), // Convert back to decimal
                pitch: (Number(data[1]) / 100).toFixed(2),
                yaw: (Number(data[2]) / 100).toFixed(2),
                timestamp: new Date(Number(data[3]) * 1000).toLocaleString()
            });
        } catch (err) {
            setError(err.message);
        }
    };

    return (
        <div className="blockchain-data">
            <h2>Blockchain Data Storage</h2>
            {error && <div className="error">{error}</div>}
            
            {account ? (
                <>
                    <p>Connected Account: {account}</p>
                    <div className="controls">
                        <button 
                            onClick={() => setIsRecording(!isRecording)}
                            className={isRecording ? "recording" : ""}
                        >
                            {isRecording ? "Stop Recording" : "Start Recording"}
                        </button>
                        <button onClick={fetchLatestData}>Fetch Latest Data</button>
                    </div>
                    
                    {latestData && (
                        <div className="data-display">
                            <h3>Latest Blockchain Data:</h3>
                            <p>Roll: {latestData.roll}°</p>
                            <p>Pitch: {latestData.pitch}°</p>
                            <p>Yaw: {latestData.yaw}°</p>
                            <p>Timestamp: {latestData.timestamp}</p>
                        </div>
                    )}
                </>
            ) : (
                <p>Please install MetaMask to use blockchain features</p>
            )}
        </div>
    );
};

export default BlockchainData; 