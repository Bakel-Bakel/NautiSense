// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

contract BoatData {
    struct SensorData {
        int256 roll;
        int256 pitch;
        int256 yaw;
        uint256 timestamp;
    }

    // Mapping from boat ID to array of sensor data
    mapping(string => SensorData[]) private boatData;
    
    // Events
    event DataRecorded(string boatId, int256 roll, int256 pitch, int256 yaw, uint256 timestamp);
    
    // Function to record new sensor data
    function recordData(
        string memory boatId,
        int256 roll,
        int256 pitch,
        int256 yaw
    ) public {
        SensorData memory newData = SensorData({
            roll: roll,
            pitch: pitch,
            yaw: yaw,
            timestamp: block.timestamp
        });
        
        boatData[boatId].push(newData);
        emit DataRecorded(boatId, roll, pitch, yaw, block.timestamp);
    }
    
    // Function to get latest data for a boat
    function getLatestData(string memory boatId) public view returns (
        int256 roll,
        int256 pitch,
        int256 yaw,
        uint256 timestamp
    ) {
        require(boatData[boatId].length > 0, "No data available for this boat");
        SensorData memory latest = boatData[boatId][boatData[boatId].length - 1];
        return (latest.roll, latest.pitch, latest.yaw, latest.timestamp);
    }
    
    // Function to get historical data for a boat
    function getHistoricalData(string memory boatId, uint256 startIndex, uint256 endIndex) 
        public 
        view 
        returns (SensorData[] memory) 
    {
        require(boatData[boatId].length > 0, "No data available for this boat");
        require(endIndex < boatData[boatId].length, "End index out of bounds");
        require(startIndex <= endIndex, "Invalid index range");
        
        uint256 length = endIndex - startIndex + 1;
        SensorData[] memory result = new SensorData[](length);
        
        for (uint256 i = 0; i < length; i++) {
            result[i] = boatData[boatId][startIndex + i];
        }
        
        return result;
    }
} 