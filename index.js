/**
 * @namespace nixmem
 * @desc UNIX Memory
 */
const nixmem = require("bindings")("nixmem.node");

console.time("sysinfo");
nixmem.getSysInfo();
console.timeEnd("sysinfo");