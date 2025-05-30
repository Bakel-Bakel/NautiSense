const hre = require("hardhat");

async function main() {
  // Get the contract factory
  const BoatData = await hre.ethers.getContractFactory("BoatData");
  
  // Deploy the contract
  const boatData = await BoatData.deploy();
  
  // Wait for deployment to finish
  await boatData.waitForDeployment();
  
  // Get the contract address
  const address = await boatData.getAddress();
  
  console.log("BoatData deployed to:", address);
}

// Execute the deployment
main().catch((error) => {
  console.error(error);
  process.exitCode = 1;
}); 