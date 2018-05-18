
pragma solidity ^0.4.21;


contract Lottery {
    
    mapping (bytes32 => address) fullTickets ; // Dictionary, key value as hashes generated from users commitment and corresponding addresses for Full Tickets
    mapping (bytes32 => address) halfTickets ; // Dictionary, key value as hashes generated from users commitment and corresponding addresses for Half Tickets
    mapping (bytes32 => address) quarterTickets ; // Dictionary, key value as hashes generated from users commitment and corresponding addresses for Quarter Tickets
    mapping (bytes32 => address) hashAddresses; //  Dictionary, key value as hashes generated from users commitment and corresponding addresses for all above
    mapping (address => uint8) ticket; // To keep track of ticket type. Changes from 1 to 3 as Quarter to Full Ticket.

    mapping (address => uint256) private balanceWinner; // If the user wins the lottery, his/her payoff will be added here
    mapping (address => bool) private isWinner; // Keep Track whether the user wins or not. If not, user cannot withdraw money

    uint blockLimitPurchase = 20; // Purchase Ticket round limit. After contract pass 20000 block, ticket purchase not allowed until next lottery.
    uint blockLimitLottery = 40; // Reveal Number and also lottery limit. After that time, reveal number part not allowed.
    uint blockLimitEndRound; // To determine the round is finished or not. Enhences every round end.
    
    uint prizeMoney; // Total money that contracts hold.

    bytes32[] numbers1st; // Keeps all the tickets for 1st numbers to determine the 1st price winner
    bytes32[] numbers2nd; // Keeps all the tickets for 2nd numbers to determine the 2nd price winner
    bytes32[] numbers3rd; // Keeps all the tickets for 3rd numbers to determine the 3rd price winner
    
    uint initBlockNum; // Keep the block number in a variable when the conract started to keep track of current block number
    uint8 counter_; // Counter variable to get random number
    
    address owner; // Message owner
    
    // Constructor of contract
    function Lottery() public {
        owner = msg.sender; // Set variable owner to msg owner
        state = LotteryState.FirstRound; // Initialize state from the first round
        initBlockNum = block.number; // Get the initial block number
        counter_ = 1; // Counter starts from 1
        prizeMoney = 0; // Initialize prize Money to 0
        blockLimitEndRound = blockLimitLottery; // next round will start when the blocklimitendround is reached.
        
    }
    
    enum LotteryState { FirstRound, SecondRound } //Enumarate the states
    
    LotteryState state; // Initialize state variable



    // Payable function to buy ticket and enter the random number. Before running this function, all the user must generate their own secret number
    // by commintment of their N with addresses and give those input parameters to this function to be able to run it. Simply, generateHash function
    // helps to generate hashes. It can be used in that part.
    function purchaseTicket(bytes32 hash1, bytes32 hash2, bytes32 hash3) public payable{
        
        if((getBlockNumber() % blockLimitLottery) > blockLimitPurchase){ // If it lies under 20000-40000, it means we are in the second round
            state = LotteryState.SecondRound;                            // Make state changes 
        }
        else{                                                            // If it lies under 0-20000, it means we are in the first round
            state = LotteryState.FirstRound;                             // Make state changes
        }
        
        require(state == LotteryState.FirstRound);  // To be able to purchase ticket, first round must be in play
        if(msg.value == 2 finney){  // Quarter Ticket purchase
            ticket[msg.sender] = 1;
            prizeMoney += 2 finney;
        }
        else if(msg.value == 4 finney){ // Half Ticket purchase
            ticket[msg.sender] = 2;
            prizeMoney += 4 finney;
        }
        else if(msg.value == 8 finney){ // Full Ticket purchase
            ticket[msg.sender] = 3;
            prizeMoney += 8 finney;
        }
        else {  // Revert all except those 3
            revert();
        }
        
        hashAddresses[hash1] = msg.sender; // Save the hash for 1st price from user to be reveal in second round.
        hashAddresses[hash2] = msg.sender; // Save the hash for 2nd price from user to be reveal in second round.
        hashAddresses[hash3] = msg.sender; // Save the hash for 3rd price from user to be reveal in second round.

    }
    
    // 2nd round. Users must reveal their numbers. If their commintments from 1st round is matching with this commintment by entering number. Than,
    // it is okey for that user to go on.
    function revealNumber(uint8 number1,uint8 number2, uint8 number3) public {
        
        if((getBlockNumber() % blockLimitLottery) > blockLimitPurchase){ // If it lies under 20000-40000, it means we are in the second round
            state = LotteryState.SecondRound;                            // Make state changes 
        }
        else{                                                            // If it lies under 0-20000, it means we are in the first round
            state = LotteryState.FirstRound;                             // Make state changes 
        }
        
        bytes32 hash1 = keccak256(number1, msg.sender); // Make commintment with their secret numbers with their addresses for the 1st price
        bytes32 hash2 = keccak256(number2, msg.sender); // Make commintment with their secret numbers with their addresses for the 2nd price
        bytes32 hash3 = keccak256(number3, msg.sender); // Make commintment with their secret numbers with their addresses for the 3rd price
        
        
        require(number1<=250 || number2<=250 || number3 <=250); // Number is uint8 so, it must be smaller than 250
        require(state == LotteryState.SecondRound); // Check whether the state is in reveal part or not

        require(hashAddresses[hash1] == msg.sender); // This is the part that check whether the users submit a valid random numbers with their addresses or not
        require(hashAddresses[hash2] == msg.sender); // This is the part that check whether the users submit a valid random numbers with their addresses or not
        require(hashAddresses[hash3] == msg.sender); // This is the part that check whether the users submit a valid random numbers with their addresses or not
        
        
        if(ticket[msg.sender] == 3){ // After all, if random numbers are valid then look for the ticket and add it to full Tickets if it is like that
            fullTickets[hash1] = msg.sender;
            fullTickets[hash2] = msg.sender;
            fullTickets[hash3] = msg.sender;
        }
        else if(ticket[msg.sender] ==2){ // After all, if random numbers are valid then look for the ticket and add it to half Tickets if it is like that
           
            halfTickets[hash1] = msg.sender;
            halfTickets[hash2] = msg.sender;
            halfTickets[hash3] = msg.sender;
        }
        else if(ticket[msg.sender] ==1){ // After all, if random numbers are valid then look for the ticket and add it to quarter Tickets if it is like that
           
            quarterTickets[hash1] = msg.sender;
            quarterTickets[hash2] = msg.sender;
            quarterTickets[hash3] = msg.sender;
        }
        else{   // Else throw
            revert();
        }
        
        
        numbers1st.push(hash1); // For the 1st price, gather all the hash values into the array.
        numbers2nd.push(hash2); // For the 2nd price, gather all the hash values into the array.
        numbers3rd.push(hash3); // For the 3rd price, gather all the hash values into the array.
        
    }
    
    // Function to determine Winner. After the lottery finished, this function must be called to announce the winners
    function determineWinner() public {
        
        counter_ += 1;
        bytes32 winningHash1 = random(1); // get 1st number randomly from calling random function with parameter 1 
        counter_ += 1;
        
        bytes32 winningHash2 = random(2); // get 2nd number randomly from calling random function with parameter 2
        counter_ += 1;
        while (hashAddresses[winningHash2] == hashAddresses[winningHash1]){ // Uniqe winner!!! if it is same with 1st reselect random number
            winningHash2 = random(2);
            counter_ += 1;
        }
        
        bytes32 winningHash3 = random(3);  // get 3rd number randomly from calling random function with parameter 3
        while ( (hashAddresses[winningHash3] == hashAddresses[winningHash2]) || (hashAddresses[winningHash3] == hashAddresses[winningHash1])){
            winningHash3 = random(3); // Uniqe winner!!! if it is same with 1st or 2nd reselect random number
            counter_ += 1;
        }
        
        distributeFunds(winningHash1,winningHash2,winningHash3); // After winning numbers are selected, call this function to distribute funds
        
        delete numbers1st; // Delete the numbers array from previous lottery to start on a new lottery
        delete numbers2nd; // Delete the numbers array from previous lottery to start on a new lottery
        delete numbers3rd; // Delete the numbers array from previous lottery to start on a new lottery

    }
    
    // Function to distriube the money to winning users
    function distributeFunds(bytes32 winningHash1, bytes32 winningHash2, bytes32 winningHash3) private { 
        
        uint256 payoff ; // Price to paid off will be calculated in this variable.
        
        if ((uint)(fullTickets[winningHash1]) != 0) { // 1st price for full ticket
            payoff = prizeMoney/2; // calculated amount
            isWinner[fullTickets[winningHash1]] = true; // make the boolean function true to be able to collect price
            balanceWinner[fullTickets[winningHash1]] += payoff; // Balance of the winner increases.
            prizeMoney -= payoff; // Price will be decrease from the total amount.
            
        }
        else if ((uint)(halfTickets[winningHash1]) != 0) { // 1st price for half ticket
            payoff = prizeMoney/4; // calculated amount
            isWinner[halfTickets[winningHash1]] = true;  // make the boolean function true to be able to collect price
            balanceWinner[halfTickets[winningHash1]] += payoff;  // Balance of the winner increases.
            prizeMoney -= payoff; // Price will be decrease from the total amount.
        
        }
        else if ((uint)(quarterTickets[winningHash1]) != 0) {  // 1st price for quarter ticket
            payoff = prizeMoney/8; // calculated amount
            isWinner[quarterTickets[winningHash1]] = true; // make the boolean function true to be able to collect price
            balanceWinner[quarterTickets[winningHash1]] += payoff;  // Balance of the winner increases.
            prizeMoney -= payoff; // Price will be decrease from the total amount.
        
        }
        
        if ((uint)(fullTickets[winningHash2]) != 0) {  // 2nd price for full ticket
            payoff = prizeMoney/4; // calculated amount
            isWinner[fullTickets[winningHash2]] = true; // make the boolean function true to be able to collect price
            balanceWinner[fullTickets[winningHash2]] += payoff; // Balance of the winner increases.
            prizeMoney -= payoff; // Price will be decrease from the total amount.
            
        }
        else if ((uint)(halfTickets[winningHash2]) != 0) { // 2nd price for half ticket
            payoff = prizeMoney/8; // calculated amount
            isWinner[halfTickets[winningHash2]] = true; // make the boolean function true to be able to collect price 
            balanceWinner[halfTickets[winningHash2]] += payoff; // Balance of the winner increases.
            prizeMoney -= payoff; // Price will be decrease from the total amount.
        
        }
        else if ((uint)(quarterTickets[winningHash2]) != 0) { // 2nd price for quarter ticket
            payoff = prizeMoney/16; // calculated amount
            isWinner[quarterTickets[winningHash2]] = true; // make the boolean function true to be able to collect price
            balanceWinner[quarterTickets[winningHash2]] += payoff; // Balance of the winner increases.
            prizeMoney -= payoff; // Price will be decrease from the total amount.
        
        }
        
        
        if ((uint)(fullTickets[winningHash3]) != 0) { // 3rd price for full ticket
            payoff = prizeMoney/8; // calculated amount
            isWinner[fullTickets[winningHash3]] = true; // make the boolean function true to be able to collect price
            balanceWinner[fullTickets[winningHash3]] += payoff; // Balance of the winner increases.
            prizeMoney -= payoff; // Price will be decrease from the total amount.
            
        }
        else if ((uint)(halfTickets[winningHash3]) != 0) { // 3rd price for half ticket
            payoff = prizeMoney/16; // calculated amount
            isWinner[halfTickets[winningHash3]] = true; // make the boolean function true to be able to collect price
            balanceWinner[halfTickets[winningHash3]] += payoff; // Balance of the winner increases.
            prizeMoney -= payoff; // Price will be decrease from the total amount.
        
        }
        else if ((uint)(quarterTickets[winningHash3]) != 0) { // 3rd price for quarter ticket
            payoff = prizeMoney/32; // calculated amount
            isWinner[quarterTickets[winningHash3]] = true; // make the boolean function true to be able to collect price
            balanceWinner[quarterTickets[winningHash3]] += payoff; // Balance of the winner increases.
            prizeMoney -= payoff; // Price will be decrease from the total amount.
        
        }
            
    }
    
    
    // Random number generator
    function random(uint8 rank) public view returns (bytes32) {
        uint temp = 0;
        
        if(rank == 1){ // For the 1st price
            temp = (uint)(block.blockhash(block.number - counter_))%(numbers1st.length); // Randomly index is choosen from the array
            return numbers1st[temp]; // return the hash of corresponding random number index
        }
        else if(rank == 2){ // For the 2nd price
            temp = (uint)(block.blockhash(block.number - counter_))%(numbers2nd.length); // Randomly index is choosen from the array
            return numbers2nd[temp]; // return the hash of corresponding random number index
        }
        else{ // For the 3rd price
            temp = (uint)(block.blockhash(block.number - counter_))%(numbers3rd.length); // Randomly index is choosen from the array
            return numbers3rd[temp]; // return the hash of corresponding random number index
        }
        
    }
    
    // Function to collect price if the user wins
    function collectPrice() public{
        
        if(isWinner[msg.sender]){ // To be able to collect it user must be win
            
            msg.sender.transfer(balanceWinner[msg.sender]); // Transfer the amont to the winning user
            
            balanceWinner[msg.sender] = 0; // Make his/her balance 0 after collecting money
            isWinner[msg.sender] = false; // Make the boolen false again to not collect the price again
            

        }
        
    }
    
    // Returns the block number started when the contract start
    function getBlockNumber() public view returns(uint){
        
        return block.number - initBlockNum; // Current block number of the contract
        
    }
    
    /* Not used in working algorithm. Just for only testing to take all the money.
    function witdrawAll() public{
        
        msg.sender.transfer(prizeMoney);
        prizeMoney = 0;
    }*/
    
    // To see the total price
    function getPriceMoney() public view returns(uint){
        return prizeMoney;
    }
    
    // Commintment a number with the address to generate 
    function generateHash(uint8 num) public view returns(bytes32){
        return keccak256(num, msg.sender);
    }
}
