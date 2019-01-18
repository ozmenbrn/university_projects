var chai = require('chai');
var assert = chai.assert;
var Web3 = require('./index');
var web3 = new Web3(new Web3.providers.HttpProvider("http://172.20.3.66:22000"));

var maker = "0xca843569e3427144cead5e4d5999a3d0ccf92b8e";
var voter = "0x0fbdc686b912d7722dc86510934589e0aaf3b55a";

web3.quorum.getNodeInfo(function(error, result){
    console.log(error, result);
});

var nodeInfo = web3.quorum.nodeInfo;
console.log("Maker", maker, web3.quorum.isBlockMaker(maker));
console.log("Voter", voter, web3.quorum.isVoter(voter));

var txHash = "0x3c0edbdce5beff35c0160a68b55ba2749f9ddb54167dd958f2a78ac5116485a7"
var address = "0x1932c48b2bf8102ba33b4a6b545c32236e342f34"
var abi = [{"constant":true,"inputs":[],"name":"storedData","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"x","type":"uint256"}],"name":"set","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"get","outputs":[{"name":"retVal","type":"uint256"}],"payable":false,"type":"function"},{"inputs":[{"name":"initVal","type":"uint256"}],"type":"constructor"}];

var private = web3.eth.contract(abi).at(address)
console.log("Old value", private.get().toString());

/*private.set(14,{from: web3.eth.coinbase,privateFor:["ROAZBWtSacxXQrOe3FGAqJDyJjFePR5ce4TSIzmJ0Bc="]}, function(error, transactionHash){
    console.log(error, transactionHash);
    console.log(web3.eth.getTransaction(transactionHash));
});*/