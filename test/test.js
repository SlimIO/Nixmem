"use strict";

// Require Third-party Dependencies
const ava = require("ava");
const is = require("@slimio/is");

// Require Internal Dependencies
const Nixmem = require("../");

ava("Nixmem should export one method", (assert) => {
    assert.deepEqual(Object.keys(Nixmem), ["getSysInfo"]);
    assert.is(typeof Nixmem.getSysInfo, "function");
});

ava("getSysInfo() should export right interface", async(assert) => {
    const sysInfo = await Nixmem.getSysInfo();

    assert.true(is.number(sysInfo.memTotal));
    assert.true(is.number(sysInfo.memFree));
    assert.true(is.number(sysInfo.swapCached));
    assert.true(is.number(sysInfo.swapTotal));
    assert.true(is.number(sysInfo.swapFree));
    assert.true(is.number(sysInfo.anonPages));
    assert.true(is.number(sysInfo.pageTables));
    assert.true(is.number(sysInfo.hugePagesTotal));
    assert.true(is.number(sysInfo.hugePagesFree));
    assert.true(is.number(sysInfo.hugePagesRsvd));
    assert.true(is.number(sysInfo.hugePagesSurp));
    assert.true(is.number(sysInfo.hugePageSize));
    assert.true(is.number(sysInfo.commitLimit));
    assert.true(is.number(sysInfo.vMallocTotal));
    assert.true(is.number(sysInfo.vMallocUsed));
    assert.true(is.number(sysInfo.vMallocChunk));
});

