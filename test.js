/*
 *  Author: SpringHack - springhack@live.cn
 *  Last modified: 2019-04-17 01:33:01
 *  Filename: test.js
 *  Description: Created by SpringHack using vim automatically.
 */
const native = require('./build/Release/napi-callback-thread.node');

native.startWork(function (date) {
  console.log(date.trim());
});

// Prevent exit
process.stdin.resume();
