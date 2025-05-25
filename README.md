# NautiSense
## System Architecture
![System FlowChart](https://github.com/user-attachments/assets/66155ee3-2505-4f35-a3ac-121ae5e0d181)

TODO: proper documentation

   # Update system
   sudo apt update && sudo apt upgrade -y
   
   # Install Node.js and npm
   curl -fsSL https://deb.nodesource.com/setup_18.x | sudo -E bash -
   sudo apt install -y nodejs
   
   # Install web server (Nginx)
   sudo apt install nginx
   
   check the framework the frontend is. dive deep into it
   
      # Build the React app
   cd ns_frontend
   npm install
   npm run build
   
   # Copy build to web server directory
   sudo cp -r build/* /var/www/html/

I'll examine the frontend framework by looking at the package.json and source files.
Let me check the source directory structure to understand the application organization.
Let me look at the main App.js file to understand the application structure:
Based on my analysis, here's a detailed breakdown of the frontend framework:
Framework: The project is built using React.js, specifically:
React version 19.0.0
React DOM version 19.0.0
Created using Create React App (react-scripts 5.0.1)
Application Structure:
It's a single-page application (SPA) with a split-panel design
Main components:
ControlsPanel: Left panel for user inputs
VisualizationPanel: Right panel for data visualization
Features:
Real-time control interface for a towing tank simulation
Input controls for:
Wave Height (cm)
Wave Frequency (Hz)
Wave Direction (degrees)
Towing Speed (knots)
Start/Stop simulation controls
Planned visualization features (currently marked as TODOs)
State Management:
Uses React's built-in useState hooks for local state management
State variables for all control parameters
Boolean state for simulation running status
Development Setup:
Includes testing setup with Jest and React Testing Library
ESLint configuration for code quality
Browser compatibility configuration via browserslist
Development scripts for:
npm start: Development server
npm build: Production build
npm test: Run tests
npm eject: Eject from Create React App
Planned Features (based on TODOs):
Integration with a Flask backend
Real-time data visualization
3D model integration
RAO/RMS/MSI visualizations
Styling:
Uses CSS modules (App.css)
Custom styling for controls and visualization panels
The application appears to be a control interface for a towing tank simulation system, with plans for real-time data visualization and 3D modeling. It's built with modern React practices and follows a component-based architecture. The frontend is set up to communicate with a Flask backend (though this integration is still pending implementation).

Hardware Setup:
Connect the MPU9250/MPU6500 sensor to the Raspberry Pi's I2C pins
Enable I2C interface using sudo raspi-config
Install required Python packages for sensor communication
Performance Considerations:
The Raspberry Pi should be able to handle the React frontend without issues
For real-time sensor data processing, you might want to use a Raspberry Pi 3 or newer
Consider using a headless setup (without GUI) to save resources
Potential Challenges:
Real-time data processing might be resource-intensive
WebSocket connections for real-time updates need to be properly configured
Sensor data processing might need optimization for the Pi's CPU
Would you like me to provide more specific details about any of these aspects or help with a particular part of the deployment process?


pip3 install pyserial websockets
python3 imu_server.py
cd ns_frontend
npm install
npm run build
sudo apt install nginx
sudo cp -r build/* /var/www/html/
