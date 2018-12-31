declare namespace Nixmem {
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

    export function getSysInfo(): MemInfo;
}

export as namespace Nixmem;
export = Nixmem;
