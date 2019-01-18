
pragma solidity ^0.4.23;
pragma experimental ABIEncoderV2;

contract RegisterSystem {
  //uint public storedData;

  mapping (string => bytes32) studentAddresses;
  mapping (bytes32 => string[]) arrayCourse;

  uint public counter;

  string[] public registeredStudents;
  string[] public tempArray;

  constructor() public {

    counter = 4;

  }

  function registerStudent(bytes32 addr, string studentName) public payable{

    studentAddresses[studentName] = addr;
    registeredStudents.push(studentName);

    //registeredStudents[msg.sender].push(studentName)
    //arrayCourse[addr] = [];

  }

  function registerCourse(bytes32 addr, string courseCode) public payable {

    arrayCourse[addr].push(courseCode);
    counter = counter + 1;

  }


  function transferStudent(string studentName, bytes32 addr) public payable{
    
    studentAddresses[studentName] = addr;
    registeredStudents.push(studentName);

  }

  function transferCourse(string course, bytes32 addr) public {
    
    arrayCourse[addr].push(course);

  }

  function getCourses(bytes32 addr) view public returns (string[]){
    delete tempArray;
    for(uint i = 0; i < arrayCourse[addr].length; i++){
      tempArray.push(arrayCourse[addr][i]);
    }
    tempArray.push(arrayCourse[addr][arrayCourse[addr].length - 1]);
    return tempArray;
  }

  function getStudentAddress(string name) view public returns (bytes32){
    return studentAddresses[name];
  }

  function getRegisteredStudents() view public returns (string[]){ //Experimental Part
    delete tempArray;
    for(uint i = 0; i < registeredStudents.length; i++){
      tempArray.push(registeredStudents[i]);
    }
    tempArray.push(registeredStudents[registeredStudents.length - 1]);
    return tempArray;
  }

  function returnLengthCourses(bytes32 addr) view public returns (uint){
    return arrayCourse[addr].length;
  }

  function lengthRegisteredStudents() view public returns (uint){
    return registeredStudents.length;
  }


  function set(uint x) public {
    counter =  x;
  }

  
  function get() view public returns (uint) {
    return counter;
  }
  

  
}