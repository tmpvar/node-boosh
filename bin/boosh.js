#!/usr/bin/env node

var path = require('path');
module.paths.push(path.join(__dirname, '..'));

console.log(module);
console.log(require('boosh'));
