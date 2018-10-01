/**
 * @namespace nixmem
 * @desc UNIX Memory
 */
const nixmem = require("bindings")("nixmem.node");

console.time("sysinfo");
console.log(nixmem.getSysInfo());
console.timeEnd("sysinfo");