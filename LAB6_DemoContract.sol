// SPDX-License-Identifier: Unlicensed
pragma solidity 0.8.26;

contract DemoContract {
    address public owner;

    struct Receivers {
        string name;
        uint256 tokens;
    }

    mapping(address => Receivers) public users;

    modifier onlyOwner() {
        require(msg.sender == owner, "Not the owner");
        _;
    }

    constructor() {
        owner = msg.sender;
        users[owner].tokens = 100;
    }

    function double(uint _value) public pure returns (uint) {
        return _value * 2;
    }

    function register(string memory _name) public {
        users[msg.sender].name = _name;
    }

    function giveToken(address _receiver, uint256 _amount) public onlyOwner {
        require(users[owner].tokens >= _amount, "Not enough tokens");
        users[owner].tokens -= _amount;
        users[_receiver].tokens += _amount;
    }

    function buyTokens(uint256 _amount) public payable {
        uint256 cost = _amount * 2 ether;

        require(msg.value >= cost, "Not enough Ether sent");
        require(users[owner].tokens >= _amount, "Owner does not have enough tokens");

        // Transfer tokens
        users[owner].tokens -= _amount;
        users[msg.sender].tokens += _amount;
    }

    // Utility to get contract's Ether balance
    function getContractBalance() public view returns (uint) {
        return address(this).balance;
    }
}
