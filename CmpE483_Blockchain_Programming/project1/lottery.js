var Web3=require('web3');

var addresscontract = "0x64c0be4c521debb1b7880abd2bd293a019e827d2"; 
var testaddresscontract = "0x5a37d51c3afee6332bc6ab862a908b72e52d9519";

abicontract = [
	{
		"constant": true,
		"inputs": [],
		"name": "getPriceMoney",
		"outputs": [
			{
				"name": "",
				"type": "uint256"
			}
		],
		"payable": false,
		"stateMutability": "view",
		"type": "function"
	},
	{
		"constant": false,
		"inputs": [],
		"name": "collectPrice",
		"outputs": [],
		"payable": false,
		"stateMutability": "nonpayable",
		"type": "function"
	},
	{
		"constant": false,
		"inputs": [
			{
				"name": "hash1",
				"type": "bytes32"
			},
			{
				"name": "hash2",
				"type": "bytes32"
			},
			{
				"name": "hash3",
				"type": "bytes32"
			}
		],
		"name": "purchaseTicket",
		"outputs": [],
		"payable": true,
		"stateMutability": "payable",
		"type": "function"
	},
	{
		"constant": true,
		"inputs": [
			{
				"name": "num",
				"type": "uint8"
			}
		],
		"name": "generateHash",
		"outputs": [
			{
				"name": "",
				"type": "bytes32"
			}
		],
		"payable": false,
		"stateMutability": "view",
		"type": "function"
	},
	{
		"constant": false,
		"inputs": [],
		"name": "determineWinner",
		"outputs": [],
		"payable": false,
		"stateMutability": "nonpayable",
		"type": "function"
	},
	{
		"constant": true,
		"inputs": [],
		"name": "getBlockNumber",
		"outputs": [
			{
				"name": "",
				"type": "uint256"
			}
		],
		"payable": false,
		"stateMutability": "view",
		"type": "function"
	},
	{
		"constant": false,
		"inputs": [
			{
				"name": "number1",
				"type": "uint8"
			},
			{
				"name": "number2",
				"type": "uint8"
			},
			{
				"name": "number3",
				"type": "uint8"
			}
		],
		"name": "revealNumber",
		"outputs": [],
		"payable": false,
		"stateMutability": "nonpayable",
		"type": "function"
	},
	{
		"constant": true,
		"inputs": [
			{
				"name": "rank",
				"type": "uint8"
			}
		],
		"name": "random",
		"outputs": [
			{
				"name": "",
				"type": "bytes32"
			}
		],
		"payable": false,
		"stateMutability": "view",
		"type": "function"
	},
	{
		"inputs": [],
		"payable": false,
		"stateMutability": "nonpayable",
		"type": "constructor"
	}
];

lotterycontract = web3.eth.contract(abicontract).at(addresscontract);
testcontract = web3.eth.contract(abicontract).at(testaddresscontract); 

eth.defaultAccount=eth.accounts[0] ;


/*
** First accounts must be unclocked to be able to make payment
*/
personal.unlockAccount(eth.accounts[0], "YOUR_PASSWORD", 0) ;
personal.unlockAccount(eth.accounts[1], "YOUR_PASSWORD", 0) ;
personal.unlockAccount(eth.accounts[2], "YOUR_PASSWORD", 0) ;
personal.unlockAccount(eth.accounts[3], "YOUR_PASSWORD", 0) ;
personal.unlockAccount(eth.accounts[4], "YOUR_PASSWORD", 0) ;


/*
** First Round: Purchase ticket with 5 different accounts. You can increase the number of accounts or buy more than 1 ticket with 1 account.
** Wait for the first round to be able to run these scripts below
** For the actual contract it lies between #block: 0-20000, 40000-60000, 80000-10000...
** For the test contract it lies between #block: 0-20, 40-60, 80,100...
*/
var hash1 = testcontract.generateHash(1);
var hash2 = testcontract.generateHash(2);
var hash3 = testcontract.generateHash(3);
testcontract.purchaseTicket.sendTransaction(hash1, hash2, hash3, {from:eth.accounts[0], to: testaddresscontract, value:web3.toWei(8,"finney"), gas:(300000)});

eth.defaultAccount=eth.accounts[1];
var hash1 = testcontract.generateHash(1);
var hash2 = testcontract.generateHash(2);
var hash3 = testcontract.generateHash(3);
testcontract.purchaseTicket.sendTransaction(hash1, hash2, hash3, {from:eth.accounts[1], to: testaddresscontract, value:web3.toWei(8,"finney"), gas:(300000)});

eth.defaultAccount=eth.accounts[2];
var hash1 = testcontract.generateHash(1);
var hash2 = testcontract.generateHash(2);
var hash3 = testcontract.generateHash(3);
testcontract.purchaseTicket.sendTransaction(hash1, hash2, hash3, {from:eth.accounts[2], to: testaddresscontract, value:web3.toWei(4,"finney"), gas:(300000)});

eth.defaultAccount=eth.accounts[3];
var hash1 = testcontract.generateHash(1);
var hash2 = testcontract.generateHash(2);
var hash3 = testcontract.generateHash(3);
testcontract.purchaseTicket.sendTransaction(hash1, hash2, hash3, {from:eth.accounts[3], to: testaddresscontract, value:web3.toWei(2,"finney"), gas:(300000)});

eth.defaultAccount=eth.accounts[4];
var hash1 = testcontract.generateHash(1);
var hash2 = testcontract.generateHash(2);
var hash3 = testcontract.generateHash(3);
testcontract.purchaseTicket.sendTransaction(hash1, hash2, hash3, {from:eth.accounts[4], to: testaddresscontract, value:web3.toWei(4,"finney"), gas:(300000)});

eth.defaultAccount=eth.accounts[0];



/*
** Second Round: Reveal the numbers for each participant
** For the Reveal Part. Wait for the second round to run these scripts below in test.
** For the actual contract it lies between #block: 20000-40000, 60000-80000, 100000-120000...
** For the test contract it lies between #block: 20-40, 60-80, 100-120...
*/
testcontract.revealNumber.sendTransaction(1,2,3,{from:eth.accounts[0], to: testaddresscontract, value:web3.toWei(0,"finney"), gas:(500000)});
eth.defaultAccount=eth.accounts[1];
testcontract.revealNumber.sendTransaction(1,2,3,{from:eth.accounts[1], to: testaddresscontract, value:web3.toWei(0,"finney"), gas:(500000)});
eth.defaultAccount=eth.accounts[2];
testcontract.revealNumber.sendTransaction(1,2,3,{from:eth.accounts[2], to: testaddresscontract, value:web3.toWei(0,"finney"), gas:(500000)});
eth.defaultAccount=eth.accounts[3];
testcontract.revealNumber.sendTransaction(1,2,3,{from:eth.accounts[3], to: testaddresscontract, value:web3.toWei(0,"finney"), gas:(500000)});
eth.defaultAccount=eth.accounts[4];
testcontract.revealNumber.sendTransaction(1,2,3,{from:eth.accounts[4], to: testaddresscontract, value:web3.toWei(0,"finney"), gas:(500000)});
eth.defaultAccount=eth.accounts[0];



// After the Lottery finished, Run this script to determine the winners.
testcontract.determineWinner.sendTransaction({from:eth.accounts[0], to: testaddresscontract, value:web3.toWei(0,"finney"), gas:(1000000)})



// After the winners determined, Winning users can collect their money prices with these scripts whenever they want.
// Not all of them can collect it. For only 5 tickets 2 of them will not be able to collect any price. 3 of them will be the winners if there are 5 tickets sold in total.
eth.defaultAccount=eth.accounts[0];
lotterycontract.collectPrice();
eth.defaultAccount=eth.accounts[1];
lotterycontract.collectPrice();
eth.defaultAccount=eth.accounts[2];
lotterycontract.collectPrice();
eth.defaultAccount=eth.accounts[3];
lotterycontract.collectPrice();
eth.defaultAccount=eth.accounts[4];
lotterycontract.collectPrice();
eth.defaultAccount=eth.accounts[0];

