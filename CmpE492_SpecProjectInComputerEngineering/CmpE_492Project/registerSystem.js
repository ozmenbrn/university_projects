var RegisterSystem = artifacts.require("./RegisterSystem.sol");

var addressSystem = "BULeR8JyUWhiuuCMU/HLA0Q5pzkYT+cHII3ZKBey3Bo=";
var addressBogazici = "QfeDAys9MPDs2XHExtc84jKGHxZg/aj52DTh0vtA3Xc=";
var addressHacettepe = "1iTZde/ndBHvzhcl7V68x44Vx7pl8nwx9LqnM/AfJUg=";
var addressItu = "oNspPPgszVUFw0qmGFfWwh1uxVUXgvBxleXORHj07g8=";
var addressOdtu = "R56gy4dn24YOjwyesTczYa8m5xhP6hF2uTMCju/1xkY=";
var addressKoc = "UfNSeSGySeKg11DVNEnqrUtxYRVor4+CvluI8tVv62Y=";
var addressOsmangazi = "ROAZBWtSacxXQrOe3FGAqJDyJjFePR5ce4TSIzmJ0Bc=";

var student1 = "0x627306090abab3a6e1400e9345bc60c78a8bef57";
var student2 = "0xf17f52151ebef6c7334fad080c5704d77216b732";
var student3 = "0xc5fdf4076b8f3a5357c5e395ab970b5b54098fef";
var student4 = "0x821aea9a577a9b44299b9c15c88cf3087f3b5544";
var student5 = "0x0d1d4e623d10f9fba5db95830f7d3839406c6af2";
var student6 = "0x2932b7a2355d6fecc4b5c0b6bd44cc31df247a2e";
var student7 = "0x2191ef87e392377ec08e7c08eb105ef5448eced5";
var student8 = "0x0f4f2ac550a1b4e2280d04c21cea7ebd822934b5";
var student9 = "0x6330a553fc93768f612722bb8c2ec78ac90b3bbc";
var student10 = "0x5aeda56215b167893e80b4fe645ba6d5bab767de";

module.exports = function(done) {
  console.log("Getting deployed version of RegisterSystem...")
  RegisterSystem.deployed().then(function(instance) {
    console.log("Building the network...");
    instance.registerStudent(student1, "Barın Özmen", {privateFor: [addressBogazici]});
    instance.registerStudent(student2, "Aras Uçar", {privateFor: [addressBogazici]});
    instance.registerStudent(student3, "Ayşe Kuter", {privateFor: [addressBogazici]});
    instance.registerStudent(student4, "Emirhan Ayan", {privateFor: [addressBogazici]});
    instance.registerStudent(student5, "Berkay Özdin", {privateFor: [addressBogazici]});
    instance.registerStudent(student6, "Nisanur Çiftçi", {privateFor: [addressBogazici]});
    instance.registerStudent(student7, "Samet Kalkan", {privateFor: [addressBogazici]});
    instance.registerStudent(student8, "Bilgehan Ayan", {privateFor: [addressKoc]});
    instance.registerStudent(student9, "Damla Memigüven", {privateFor: [addressKoc]});
    instance.registerStudent(student10, "Ayşenur Bayrak", {privateFor: [addressKoc]});
    instance.registerStudent(student1, "Deniz Kılıç", {privateFor: [addressKoc]});
    instance.registerStudent(student2, "Aysu Öner", {privateFor: [addressKoc]});
    instance.registerStudent(student3, "Dilara Sezen", {privateFor: [addressKoc]});

    instance.registerStudent(student4, "Umut Çepoğlu", {privateFor: [addressItu]});
    instance.registerStudent(student5, "Gizem Özen", {privateFor: [addressItu]});
    instance.registerStudent(student6, "Ozan Gültekin", {privateFor: [addressItu]});
    instance.registerStudent(student7, "Emir Koçer", {privateFor: [addressItu]});
    instance.registerStudent(student8, "Maria Kuznetsov", {privateFor: [addressItu]});
    instance.registerStudent(student9, "Mete Kökümer", {privateFor: [addressItu]});
    instance.registerStudent(student10, "Sezi Kaya", {privateFor: [addressItu]});

    instance.registerCourse(student1, "CmpE483", {privateFor: [addressBogazici]});
    instance.registerCourse(student1, "CmpE492", {privateFor: [addressBogazici]});
    instance.registerCourse(student1, "CmpE480", {privateFor: [addressBogazici]});
    instance.registerCourse(student1, "CmpE443", {privateFor: [addressBogazici]});
    instance.registerCourse(student1, "CmpE451", {privateFor: [addressBogazici]});
    instance.registerCourse(student1, "IE310", {privateFor: [addressBogazici]});

    instance.registerCourse(student2, "MATH202", {privateFor: [addressBogazici]});
    instance.registerCourse(student2, "AD131", {privateFor: [addressBogazici]});
    instance.registerCourse(student2, "CHE433", {privateFor: [addressBogazici]});
    instance.registerCourse(student2, "PHYS201", {privateFor: [addressBogazici]});
    instance.registerCourse(student2, "CmpE150", {privateFor: [addressBogazici]});

    instance.registerCourse(student3, "CHE383", {privateFor: [addressBogazici]});
    instance.registerCourse(student3, "CHEM498", {privateFor: [addressBogazici]});
    instance.registerCourse(student3, "HTR312", {privateFor: [addressBogazici]});
    instance.registerCourse(student3, "STS323", {privateFor: [addressBogazici]});
    instance.registerCourse(student3, "EC102", {privateFor: [addressBogazici]});

    instance.registerCourse(student4, "MATH201", {privateFor: [addressBogazici]});
    instance.registerCourse(student4, "CmpE220", {privateFor: [addressBogazici]});
    instance.registerCourse(student4, "CmpE230", {privateFor: [addressBogazici]});
    instance.registerCourse(student4, "CmpE491", {privateFor: [addressBogazici]});
    instance.registerCourse(student4, "MATH102", {privateFor: [addressBogazici]});


    instance.registerCourse(student5, "CHE323", {privateFor: [addressBogazici]});
    instance.registerCourse(student5, "CHEM401", {privateFor: [addressBogazici]});
    instance.registerCourse(student5, "HTR311", {privateFor: [addressBogazici]});
    instance.registerCourse(student5, "TK211", {privateFor: [addressBogazici]});
    instance.registerCourse(student5, "CmpE350", {privateFor: [addressBogazici]});

    instance.registerCourse(student6, "CmpE322", {privateFor: [addressBogazici]});
    instance.registerCourse(student6, "CmpE300", {privateFor: [addressBogazici]});
    instance.registerCourse(student6, "PHYS202", {privateFor: [addressBogazici]});
    instance.registerCourse(student6, "CmpE344", {privateFor: [addressBogazici]});
    return instance.registerCourse(student6, "CmpE160", {privateFor: [addressBogazici]});
    
  }).then(function(result) {
    console.log("Transaction:", result.tx);
    console.log("Finished!");
    done();
  }).catch(function(e) {
    console.log(e);
    done();
  });
};
/*
RegisterSystem.deployed().then(function(instance) {return instance.registerStudent("0x6330a553fc93768f612722bb8c2ec78ac90b3bbc", "Registering Student", {privateFor: ["QfeDAys9MPDs2XHExtc84jKGHxZg/aj52DTh0vtA3Xc="]});})
RegisterSystem.deployed().then(function(instance) {return instance.registerCourse("0x5aeda56215b167893e80b4fe645ba6d5bab767de", "New Course", {privateFor: ["QfeDAys9MPDs2XHExtc84jKGHxZg/aj52DTh0vtA3Xc="]});})

RegisterSystem.deployed().then(function(instance) {return instance.getCourses("0x627306090abab3a6e1400e9345bc60c78a8bef57");})
RegisterSystem.deployed().then(function(instance) {return instance.getRegisteredStudents();})

RegisterSystem.deployed().then(function(instance) {return instance.getStudentAddress("Barın Özmen");})
RegisterSystem.deployed().then(function(instance) {return instance.get();})
RegisterSystem.deployed().then(function(instance) {return instance.getCourses("0x627306090abab3a6e1400e9345bc60c78a8bef57");})
RegisterSystem.deployed().then(function(instance) {return instance.returnLengthCourses("0x627306090abab3a6e1400e9345bc60c78a8bef57");})
truffle migrate --reset --all

*/