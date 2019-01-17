/**
 * @namespace nixmem
 * @desc UNIX Memory
 */
const nixmem = require("node-gyp-build")(__dirname);

/**
 * @function getSysInfo
 * @memberof nixmem#
 * @returns {Promise<Nixmem.MemInfo>}
 */
function getSysInfo() {
    return new Promise((resolve, reject) => {
        nixmem.getSysInfo((err, ret) => {
            if (err) {
                return reject(err);
            }

            return resolve(ret);
        });
    });
}

module.exports = {
    getSysInfo
};
