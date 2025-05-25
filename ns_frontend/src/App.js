import React, { useState } from "react";
import "./App.css"; // Import the CSS file
import IMUDisplay from "./components/IMUDisplay";

export default function App() {
  return (
    <div className="container">
      {/* Left Panel - Controls */}
      <ControlsPanel />
      
      {/* Right Panel - Visualization */}
      <div className="right-panel">
        <VisualizationPanel />
        <IMUDisplay />
      </div>
    </div>
  );
}

// Controls Panel
function ControlsPanel() {
  const [waveHeight, setWaveHeight] = useState(0);
  const [waveFrequency, setWaveFrequency] = useState(0);
  const [waveDirection, setWaveDirection] = useState(0);
  const [towingSpeed, setTowingSpeed] = useState(0);
  const [isRunning, setIsRunning] = useState(false);

  const handleSubmit = () => {
    console.log("Sending settings:", { waveHeight, waveFrequency, waveDirection, towingSpeed, isRunning });
    // TODO: Send data to Flask backend
  };

  return (
    <div className="controls-panel">
      <h2>Towing Tank Controls</h2>

      <label>Wave Height (cm)</label>
      <input
        type="number"
        value={waveHeight}
        onChange={(e) => setWaveHeight(e.target.value)}
      />

      <label>Wave Frequency (Hz)</label>
      <input
        type="number"
        value={waveFrequency}
        onChange={(e) => setWaveFrequency(e.target.value)}
      />

      <label>Wave Direction (°)</label>
      <input
        type="number"
        value={waveDirection}
        onChange={(e) => setWaveDirection(e.target.value)}
      />

      <label>Towing Speed (knots)</label>
      <input
        type="number"
        value={towingSpeed}
        onChange={(e) => setTowingSpeed(e.target.value)}
      />

      <button onClick={() => setIsRunning(!isRunning)} className="control-button">
        {isRunning ? "Stop Simulation" : "Start Simulation"}
      </button>

      <button onClick={handleSubmit} className="submit-button">
        Submit Settings
      </button>
    </div>
  );
}

// Visualization Panel
function VisualizationPanel() {
  return (
    <div className="visualization-panel">
      <h2>Visualization</h2>
      <p>Real-time data will be displayed here.</p>
      {/* TODO: Integrate real-time graphs, 3D model, and RAO/RMS/MSI visualizations */}
    </div>
  );
}
