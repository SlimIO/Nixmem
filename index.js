/**
 * @namespace nixmem
 * @desc UNIX Memory
 */
const nixmem = require("node-gyp-build")(__dirname);

nixmem.getSysInfo((err, ret) => {
    if (err) {
        return console.error(err);
    }

    return console.log(ret);
});
