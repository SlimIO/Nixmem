/**
 * @namespace nixmem
 * @desc UNIX Memory
 */
const nixmem = require("node-gyp-build")(__dirname);

console.time("sysinfo");
console.log(nixmem.getSysInfo());
console.timeEnd("sysinfo");
