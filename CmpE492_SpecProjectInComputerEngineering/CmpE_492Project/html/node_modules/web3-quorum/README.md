# Ethereum & Quorum JavaScript API

[![Join the chat at https://gitter.im/ethereum/web3.js](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/ethereum/web3.js?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

This is the fork version of the Web3 API Ethereum compatible [JavaScript API](https://github.com/ethereum/wiki/wiki/JavaScript-API)
which is extended to support the Quorum API.

[![NPM version][npm-image]][npm-url] [![Build Status][travis-image]][travis-url] [![dependency status][dep-image]][dep-url] [![dev dependency status][dep-dev-image]][dep-dev-url] [![Coverage Status][coveralls-image]][coveralls-url] [![Stories in Ready][waffle-image]][waffle-url]

<!-- [![browser support](https://ci.testling.com/ethereum/ethereum.js.png)](https://ci.testling.com/ethereum/ethereum.js) -->

You need to run a local Ethereum/Quorum node to use this library.

[Documentation](https://github.com/ethereum/wiki/wiki/JavaScript-API)
[Quorum API](https://github.com/jpmorganchase/quorum/blob/master/docs/api.md)

## Installation

### Node.js

```bash
npm install web3-quorum
```

## Usage
Use the `web3` object directly from global namespace:

```js
console.log(web3); // {eth: .., shh: ...} // it's here!
```

Set a provider (HttpProvider)

```js
web3.setProvider(new web3.providers.HttpProvider('http://localhost:8545'));
```

There you go, now you can use it:

```js
var nodeInfo = web3.quorum.nodeInfo;
web3.quorum.getNodeInfo(function(error, result){
    console.log(error, result);
});
```

Quorum functions

```js
web3.quorum.nodeInfo
web3.quorum.isBlockMaker(address)
web3.quorum.isVoter(address)
web3.quorum.canonicalHash(blockHash)
web3.quorum.makeBlock()
web3.quorum.vote()
web3.quorum.pauseBlockMaker()
web3.quorum.resumeBlockMaker()
```

## Contribute!

### Requirements

* Node.js
* npm

```bash
sudo apt-get update
sudo apt-get install nodejs
sudo apt-get install npm
sudo apt-get install nodejs-legacy
```

### Building (gulp)

```bash
npm run-script build
```


### Testing (mocha)

```bash
npm test
```

[npm-image]: https://badge.fury.io/js/web3.png
[npm-url]: https://npmjs.org/package/web3
[travis-image]: https://travis-ci.org/ethereum/web3.js.svg
[travis-url]: https://travis-ci.org/ethereum/web3.js
[dep-image]: https://david-dm.org/ethereum/web3.js.svg
[dep-url]: https://david-dm.org/ethereum/web3.js
[dep-dev-image]: https://david-dm.org/ethereum/web3.js/dev-status.svg
[dep-dev-url]: https://david-dm.org/ethereum/web3.js#info=devDependencies
[coveralls-image]: https://coveralls.io/repos/ethereum/web3.js/badge.svg?branch=master
[coveralls-url]: https://coveralls.io/r/ethereum/web3.js?branch=master
[waffle-image]: https://badge.waffle.io/ethereum/web3.js.svg?label=ready&title=Ready
[waffle-url]: https://waffle.io/ethereum/web3.js

