var RegisterSystem = artifacts.require("./RegisterSystem.sol");

var addressSystem = "BULeR8JyUWhiuuCMU/HLA0Q5pzkYT+cHII3ZKBey3Bo=";
var addressBogazici = "QfeDAys9MPDs2XHExtc84jKGHxZg/aj52DTh0vtA3Xc=";
var addressHacettepe = "1iTZde/ndBHvzhcl7V68x44Vx7pl8nwx9LqnM/AfJUg=";
var addressItu = "oNspPPgszVUFw0qmGFfWwh1uxVUXgvBxleXORHj07g8=";
var addressOdtu = "R56gy4dn24YOjwyesTczYa8m5xhP6hF2uTMCju/1xkY=";
var addressKoc = "UfNSeSGySeKg11DVNEnqrUtxYRVor4+CvluI8tVv62Y=";
var addressOsmangazi = "ROAZBWtSacxXQrOe3FGAqJDyJjFePR5ce4TSIzmJ0Bc=";

module.exports = function(deployer) {
  deployer.deploy(RegisterSystem, {privateFor: [addressBogazici, addressHacettepe, addressItu, addressOdtu, addressKoc, addressOsmangazi]});
};
