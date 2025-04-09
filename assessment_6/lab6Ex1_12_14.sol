pragma solidity >=0.5.0 <0.8.0;
//Ex1
contract Factory {
    //empty
    }
//Ex2
contract productID  {
    uint public idDigits = 16; // Declares and initializes the variable

}
//Ex3
contract ProductID {
    uint public idDigits = 16; // Existing variable with value 16
    uint public idModulus = 10 ** idDigits; // New variable set to 10^idDigits
}

 //Ex4-5
contract ProductFactory {
    // struct
    struct Product {
        string name; 
        uint id;     
    }

}

//Ex5
contract ProductFactory5 {
    // Define the Product struct
    struct Product {
        string name; // The product's name
        uint id;     // The product's unique ID
    }

    // Create a public dynamic array of Product structs
    Product[] public products;
}


        
//Ex6

contract ProductFactory6 {
    struct Product {
        string name; // Name of the product
        uint id;     // Product ID
    }

    Product[] public products; // Array to store products

    // Private function to create and add a new Product
    function _createProduct(string memory _name, uint _id) private {
        products.push(Product(_name, _id)); // Adding the new Product to the array
    }
}


//Ex7
contract ProductFactory7 {

    // Private function to create a new Product
/*    function _generateRandomId(string memory _str) public view returns (uint) {
        //empty
    }*/
}
//Ex8
contract ProductFactory8 {

        struct Product {
        string name; // Name of the product
        uint id;     // Product ID
    }
    uint public idDigits = 16; // Existing variable with value 16
    uint public idModulus = 10 ** idDigits; // New variable set to 10^idDigits
    Product[] public products; // Array to store products

    // Private function to create a new Product
    function _generateRandomId(string memory _str) private view returns (uint256) {
        uint256 rand = uint256(keccak256(abi.encodePacked(_str)));
        return rand % idModulus;
}

    function _createProduct(string memory _name, uint _id) private {
        products.push(Product(_name, _id)); // Adding the new Product to the array
    }

    function  createProduct(string memory _name) public{
        uint256 randId = _generateRandomId(_name);
        _createProduct(_name, randId);
    }

//Ex9
    // event
    event NewProduct(uint indexed ArrayProductId, string name, uint id);

    function _createProduct9(string memory _name, uint _id) private {
        uint productId = products.push(Product(_name, _id)) - 1; 
        emit NewProduct(productId, _name, _id); 


}



}


//Ex10

contract ProductFactory10 {

        struct Product {
        string name; // Name of the product
        uint id;     // Product ID
    }

    Product[] public products; // Array to store products
    mapping(uint => address) public productToOwner;
    mapping(address => uint) public ownerProductCount;
    uint public idDigits = 16; 
    uint public idModulus = 10 ** idDigits; 
    // Private function to create a new Product
    function _createProduct(string memory _name, uint _id) private {
        uint productId = products.push(Product(_name, _id)) - 1; // Add product to array and get its ID
        productToOwner[productId] = msg.sender; // Assign ownership to the sender
        ownerProductCount[msg.sender]++; // Increment the owner's product count
    }
    
    function _generateRandomId(string memory _str) private view returns (uint) {
        uint rand = uint(keccak256(abi.encodePacked(_str))); // Generate hash and convert to uint
        return rand % idModulus; // Limit ID to 16 digits
    }
    function createRandomProduct(string memory _name) public {
        uint randId = _generateRandomId(_name); // Generate random ID using the product name
        _createProduct(_name, randId); // Create and store the product
    }
//Ex11
    function Ownership(uint _productId) public {
        require(productToOwner[_productId] == address(0), "Product already has an owner."); // Ensure product isn't owned yet
        productToOwner[_productId] = msg.sender; // Assign ownership to msg.sender
        ownerProductCount[msg.sender]++; // Increment owner's product count
    }


}

//Ex12

contract ProductFactory12 {
    struct Product {
        string name; // Product name
        uint id;     // Product ID
    }

    // Array to store all products
    Product[] public products;

    // Mappings for ownership information
    mapping(uint => address) public productToOwner;
    mapping(address => uint) public ownerProductCount;

    function getProductsByOwner(address _owner) external view returns (uint[] memory) {
        uint counter = 0; // Counter for the result array index

        // Create a new memory array to store product IDs owned by the given address
        uint[] memory result = new uint[](ownerProductCount[_owner]);

        // Iterate through the `products` array
        for (uint i = 0; i < products.length; i++) {
            // Check if the owner matches
            if (productToOwner[i] == _owner) {
                result[counter] = i; // Add the product ID to the result array
                counter++; // Increment the counter
            }
        }

        return result; // Return the result array
    }
}
//Ex14



contract ProductFactory14 {
    struct Product {
        string name; // Product name
        uint id;     // Product ID
    }

    // Array to store all products
    Product[] public products;

    // Mappings to store ownership information
    mapping(uint => address) public productToOwner;
    mapping(address => uint) public ownerProductCount;

    // Function to get products owned by a specific address
    function getProductsByOwner(address _owner) external view returns (uint[] memory) {
        uint counter = 0; // Counter for the result array index

        // Create a new memory array to store product IDs owned by the given address
        uint[] memory result = new uint[](ownerProductCount[_owner]);

        // Iterate through the `products` array
        for (uint i = 0; i < products.length; i++) {
            // Check if the owner matches
            if (productToOwner[i] == _owner) {
                result[counter] = i; // Add the product ID to the result array
                counter++; // Increment the counter
            }
        }

        return result; // Return the result array
    }
}


