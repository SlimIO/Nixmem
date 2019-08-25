# Nixmem
[![Maintenance](https://img.shields.io/badge/Maintained%3F-yes-green.svg)](https://github.com/SlimIO/Nixmem/commit-activity)
[![GitHub license](https://img.shields.io/github/license/Naereen/StrapDown.js.svg)](https://github.com/SlimIO/Nixmem/blob/master/LICENSE)
![V1.0](https://img.shields.io/badge/version-1.0.0-blue.svg)
![N-API](https://img.shields.io/badge/N--API-experimental-orange.svg)

SlimIO Nixmem is a Node.js binding which bring information about memory consumption. This module has been designed for UNIX systems (For windows please take a look at [Winmem](https://github.com/SlimIO/Winmem)).

| Linux | BSD | SunOs |
| --- | --- | --- |
| ✔️ | ✔️ | ✔️ |

## Requirements
- [Node.js](https://nodejs.org/en/) v10 or higher
- UNIX system.

## Getting Started

This package is available in the Node Package Repository and can be easily installed with [npm](https://docs.npmjs.com/getting-started/what-is-npm) or [yarn](https://yarnpkg.com).

```bash
$ npm i @slimio/nixmem
# or
$ yarn add @slimio/nixmem
```

## Usage example

```js
const { getSysInfo } = require("@slimio/nixmem");

async function main() {
    const sysInfo = await getSysInfo();
    console.log(sysInfo());
}
main().catch(console.error);
```

## API

### getSysInfo(): Promise< MemInfo >
SysInfo will return information about your local memory consumption. The method will return data with a MemInfo interface:

```ts
interface MemInfo {
    memTotal: number;
    memFree: number;
    memShared: number;
    memAvailable: number;
    swapCached: number;
    swapTotal: number;
    swapFree: number;
    anonPages: number;
    pageTables: number;
    shmemHugePages: number;
    shmemPmdMapped: number;
    hugePagesTotal: number;
    hugePagesFree: number;
    hugePagesRsvd: number;
    hugePagesSurp: number;
    hugePageSize: number;
    commitLimit: number;
    vMallocTotal: number;
    vMallocUsed: number;
    vMallocChunk: number;
}
```

## Contribution Guidelines
To contribute to the project, please read the [code of conduct](https://github.com/SlimIO/Governance/blob/master/COC_POLICY.md) and the guide for [N-API compilation](https://github.com/SlimIO/Governance/blob/master/docs/native_addons.md).

## Dependencies

|Name|Refactoring|Security Risk|Usage|
|---|---|---|---|
|[node-addon-api](https://github.com/nodejs/node-addon-api)|⚠️Major|Low|Node.js C++ addon api|
|[node-gyp-build](https://github.com/prebuild/node-gyp-build)|⚠️Major|Low|Node-gyp builder|

## License
MIT
