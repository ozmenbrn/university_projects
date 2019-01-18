/*
 * NB: since truffle-hdwallet-provider 0.0.5 you must wrap HDWallet providers in a 
 * function when declaring them. Failure to do so will cause commands to hang. ex:
 * ```
 * mainnet: {
 *     provider: function() { 
 *       return new HDWalletProvider(mnemonic, 'https://mainnet.infura.io/<infura-key>') 
 *     },
 *     network_id: '1',
 *     gas: 4500000,
 *     gasPrice: 10000000000,
 *   },
 */

// File: `truffle.js`
module.exports = {
  networks: {
    development: {
      host: "127.0.0.1",
      port: 22000, // was 8545
      network_id: "*", // Match any network id
      gasPrice: 0,
      gas: 4500000
    },
    bogaziciUniversity:  {
      host: "127.0.0.1",
      port: 22001,
      network_id: "*", // Match any network id
      gasPrice: 0,
      gas: 4500000
    },
    hacettepeUniversity:  {
      host: "127.0.0.1",
      port: 22002,
      network_id: "*", // Match any network id
      gasPrice: 0,
      gas: 4500000
    },
    itu:  {
      host: "127.0.0.1",
      port: 22003,
      network_id: "*", // Match any network id
      gasPrice: 0,
      gas: 4500000
    },
    odtu:  {
      host: "127.0.0.1",
      port: 22004,
      network_id: "*", // Match any network id
      gasPrice: 0,
      gas: 4500000
    },
    kocUniversity:  {
      host: "127.0.0.1",
      port: 22005,
      network_id: "*", // Match any network id
      gasPrice: 0,
      gas: 4500000
    },
    osmangaziUniversity:  {
      host: "127.0.0.1",
      port: 22006,
      network_id: "*", // Match any network id
      gasPrice: 0,
      gas: 4500000
    }
  }
};
