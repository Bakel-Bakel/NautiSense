// SPDX-License-Identifier: Unlicensed
pragma solidity 0.8.18;

contract DemoContract {
    address public owner;

    struct Receivers {
        string name;
        uint256 tokens;
    }

    mapping(address => Receivers) public users;

    modifier onlyOwner() {
        require(msg.sender == owner, "Not the contract owner");
        _;
    }

    constructor() {
        owner = msg.sender;
        users[owner].tokens = 100; // Owner starts with 100 tokens
    }

    function double(uint _value) public pure returns (uint) {
        return _value * 2; // Multiplies input by 2
    }

    function register(string memory _name) public {
        users[msg.sender].name = _name; // Assign a name to the sender in the mapping
    }

    function giveToken(address receiver, uint256 amount) public onlyOwner {
        require(users[owner].tokens >= amount, "Not enough tokens");
        users[owner].tokens -= amount;
        users[receiver].tokens += amount;
    }

    // Function to buy tokens with Ether
    function buyTokens(uint256 amount) public payable {
        uint256 cost = amount * 2 ether; // 1 token costs 2 Ether
        require(msg.value >= cost, "Insufficient Ether sent");
        require(users[owner].tokens >= amount, "Not enough tokens available");

        // Transfer tokens to the buyer
        users[msg.sender].tokens += amount;
        users[owner].tokens -= amount;
    }

    // Function to get the Ether balance of the contract
    function getContractBalance() public view returns (uint256) {
        return address(this).balance; // Returns the balance of Ether in the contract
    }
}