# CmpE483: Autonomous Decentralized Lottery As a Solidity Smart Contract

http://www.piazza.com/class_profile/get_resource/jda1696qwm25l/jgluwjc2444fk

You can view it on the course page: https://piazza.com/boun.edu.tr/spring2018/cmpe483/resources


<br>

<b>Installation:</b> Download the files type the following to terminal: <b>"nodejs server.js"</b> <br>
Then open the brave browser with you metamask account and go to url: http://localhost:8081/lottery.html

<h2>Function Calls:</h2>

+ <b>purchaseTicket(bytes32 hash1, bytes32 hash2, bytes32 hash3) public payable:</b> 
Payable function to buy ticket and enter the random number. Before running this function, all the user must generate their own secret number by commintment of their N with addresses and give those input parameters to this function to be able to run it. Simply, generateHash function helps to generate hashes. It can be used in that part.
  
+ <b>function revealNumber(uint8 number1,uint8 number2, uint8 number3) public:</b><br>
  2nd round. Users must reveal their numbers. If their commintments from 1st round is matching with this commintment by entering number. Than, it is okey for that user to go on.
  
+ <b>function determineWinner() public:</b><br>
  Function to determine Winner. After the lottery finished, this function must be called to announce the winners
  
+ <b>function distributeFunds(bytes32 winningHash1, bytes32 winningHash2, bytes32 winningHash3) private:</b><br>
  Function to distribute the money to the winning users
  
+ <b>function random(uint8 rank) public view returns (bytes32):</b><br>
  Random number generator
  
+ <b>function collectPrice() public:</b><br>
  Function to collect price if the user wins
  
+ <b>function getBlockNumber() public view returns(uint):</b><br>
  Returns the block number started from the beginning of the conract
  
+ <b>function getPriceMoney() public view returns(uint):</b><br>
  Returns the total amount of money stored in the conract
  
+ <b>function generateHash(uint8 num) public view returns(bytes32):</b><br>
  Commintment of the secret number for each user by combining with their addresses 

