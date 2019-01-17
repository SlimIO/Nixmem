# Nixmem
[![Maintenance](https://img.shields.io/badge/Maintained%3F-yes-green.svg)](https://github.com/SlimIO/Nixmem/commit-activity)
[![GitHub license](https://img.shields.io/github/license/Naereen/StrapDown.js.svg)](https://github.com/SlimIO/Nixmem/blob/master/LICENSE)
![V1.0](https://img.shields.io/badge/version-1.0.0-blue.svg)
![N-API](https://img.shields.io/badge/N--API-experimental-orange.svg)

SlimIO Nixmem is a Node.js binding which bring information about memory consumption. This module has been designed for UNIX systems (For windows please take a look at [Winmem](https://github.com/SlimIO/Winmem)).

| Linux | BSD | SunOs |
| --- | --- | --- |
| ✔️ | ✔️ | ✔️ |

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

## How to build the project

Before building the project, be sure to get the following npm package installed:

- Install (or upgrade to) NodeJS v10+ and npm v6+
- [Windows build tools](https://www.npmjs.com/package/windows-build-tools)

Then, just run normal npm install command:

```bash
$ npm install
```

## Available commands

All projects commands are described here:

| command | description |
| --- | --- |
| npm run prebuild | Generate addon prebuild |
| npm run doc | Generate JSDoc .HTML documentation (in the /docs root directory) |
| npm run coverage | Generate coverage of tests |
| npm run report | Generate .HTML report of tests coverage |

> the report command have to be triggered after the coverage command.
