#include "napi.h"
#include <errno.h>
#include <sstream>

#if __FreeBSD__
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/vmmeter.h>
#include <sys/resource.h>
#include <vm/vm_param.h>
#else
#include <map>
#include <sys/sysinfo.h>
#endif

#if __FreeBSD__

/**
 * sysctl C++ wrapper
 */
template<class T>
T sysInfo(const int firstLevel, const int secondLevel) {
    int name[2] = {firstLevel, secondLevel};
    T infoValue;
    size_t len = sizeof(infoValue);
    sysctl(name, (u_int) 2, &infoValue, &len, NULL, 0);

    return infoValue;
}

/**
 * sysctl C++ structured wrapper
 */
int sysInfo(const int firstLevel, const int secondLevel, void* info) {
    int ret;
    int name[2] = {firstLevel, secondLevel};
    size_t len = sizeof(info);

    ret = sysctl(name, (u_int) 2, NULL, &len, NULL, 0);
    if (ret != -1) {
        return ret;
    }
    info = malloc(len);

    return sysctl(name, (u_int) 2, &info, &len, NULL, 0);
}

/**
 * FreeBSD getSysInfo binding Worker
 */
class SysInfoWorker : public Napi::AsyncWorker {
    public:
        SysInfoWorker(Napi::Function& callback) : AsyncWorker(callback) {}
        ~SysInfoWorker() {}
    private:
        struct vmtotal vm_info;
        unsigned int NCPU;
        long BYTEORDER;
        long PHYSMEM;
        long USERMEM;
        long PAGESIZE;
        long REALMEM;
        bool SWAPPING_ENABLED;
        long V_FREE_MIN;
        long V_FREE_RESERVED;
        long V_FREE_TARGET;
        long V_INACTIVE_TARGET;
        long V_PAGEOUT_FREE_MIN;

    void Execute() {
        if (sysInfo(CTL_VM, VM_TOTAL, &vm_info) == -1) {
            SetError("sysctl failed for CTL_VM.VM_TOTAL");
        }

        NCPU = sysInfo<unsigned int>(CTL_HW, HW_NCPU);
        BYTEORDER = sysInfo<long>(CTL_HW, HW_BYTEORDER);
        PHYSMEM = sysInfo<long>(CTL_HW, HW_PHYSMEM);
        USERMEM = sysInfo<long>(CTL_HW, HW_USERMEM);
        PAGESIZE = sysInfo<long>(CTL_HW, HW_PAGESIZE);
        REALMEM = sysInfo<long>(CTL_HW, HW_REALMEM);
        SWAPPING_ENABLED = sysInfo<bool>(CTL_VM, VM_SWAPPING_ENABLED);
        V_FREE_MIN = sysInfo<long>(CTL_VM, VM_V_FREE_MIN);
        V_FREE_RESERVED = sysInfo<long>(CTL_VM, VM_V_FREE_RESERVED);
        V_FREE_TARGET = sysInfo<long>(CTL_VM, VM_V_FREE_TARGET);
        V_INACTIVE_TARGET = sysInfo<long>(CTL_VM, VM_V_INACTIVE_TARGET);
        V_PAGEOUT_FREE_MIN = sysInfo<long>(CTL_VM, VM_V_PAGEOUT_FREE_MIN);
    }

    void OnError(const Napi::Error& e) {
        std::stringstream error;
        error << e.what() << ", error code: " << errno << std::endl;
        Napi::Error::New(Env(), error.str()).ThrowAsJavaScriptException();
    }

    void OnOK() {
        Napi::HandleScope scope(Env());
        Napi::Object vmtotal = Napi::Object::New(env);
        Napi::Object ret = Napi::Object::New(env);

        vmtotal.Set("t_vm", vm_info.t_vm);
        vmtotal.Set("t_avm", vm_info.t_avm);
        vmtotal.Set("t_rm", vm_info.t_rm);
        vmtotal.Set("t_arm", vm_info.t_arm);
        vmtotal.Set("t_vmshr", vm_info.t_vmshr);
        vmtotal.Set("t_avmshr", vm_info.t_avmshr);
        vmtotal.Set("t_rmshr", vm_info.t_rmshr);
        vmtotal.Set("t_armshr", vm_info.t_armshr);
        vmtotal.Set("t_free", vm_info.t_free);
        vmtotal.Set("t_rq", vm_info.t_rq);
        vmtotal.Set("t_dw", vm_info.t_dw);
        vmtotal.Set("t_pw", vm_info.t_pw);
        vmtotal.Set("t_sl", vm_info.t_sl);
        vmtotal.Set("t_sw", vm_info.t_sw);

        ret.Set("vmtotal", vmtotal);
        ret.Set("nCPU", NCPU);
        ret.Set("byteOrder", BYTEORDER);
        ret.Set("physMem", PHYSMEM);
        ret.Set("userMem", USERMEM);
        ret.Set("pageSize", PAGESIZE);
        ret.Set("realMem", REALMEM);
        ret.Set("swapping_enable", SWAPPING_ENABLED);
        ret.Set("vFreeMin", V_FREE_MIN);
        ret.Set("vFreeReserved", V_FREE_RESERVED);
        ret.Set("vFreeTarget", V_FREE_TARGET);
        ret.Set("vInactiveTarget", _INACTIVE_TARGET);
        ret.Set("vPageoutFreeMin", V_PAGEOUT_FREE_MIN);

        Callback().Call({Env().Null(), ret});
    }
};

#else

struct cmp_str {
   bool operator()(char const *a, char const *b){
      return strcmp(a, b) < 0;
   }
};

/**
 * All possible proc fields
 */
std::map<const char*, const char*, cmp_str> procFields = {
    {"MemTotal:", "memTotal"},
    {"MemFree:", "memFree"},
    {"MemShared:", "memShared"},
    {"MemAvailable:", "memAvailable"},
    {"SwapCached:", "swapCached"},
    {"SwapTotal:", "swapTotal"},
    {"SwapFree:", "swapFree"},
    {"AnonPages:", "anonPages"},
    {"PageTables:", "pageTables"},
    {"ShmemHugePages:", "shmemHugePages"},
    {"ShmemPmdMapped:", "shmemPmdMapped"},
    {"HugePages_Total:", "hugePagesTotal"},
    {"HugePages_Free:", "hugePagesFree"},
    {"HugePages_Rsvd:", "hugePagesRsvd"},
    {"HugePages_Surp:", "hugePagesSurp"},
    {"Hugepagesize:", "hugePageSize"},
    {"CommitLimit:", "commitLimit"},
    {"VmallocTotal:", "vMallocTotal"},
    {"VmallocUsed:", "vMallocUsed"},
    {"VmallocChunk:", "vMallocChunk"}
};

/**
 * Linux getSysInfo binding worker
 * 
 * @doc: https://access.redhat.com/solutions/406773
 * @doc: https://www.centos.org/docs/5/html/5.1/Deployment_Guide/s2-proc-meminfo.html
 * @doc: https://superuser.com/questions/521551/cat-proc-meminfo-what-do-all-those-numbers-mean
 */
class SysInfoWorker : public Napi::AsyncWorker {
    public:
        SysInfoWorker(Napi::Function& callback) : AsyncWorker(callback) {}
        ~SysInfoWorker() {}
    private:
        struct MemInfoEntry {
            const char* key;
            unsigned int value;
        };
        std::vector<MemInfoEntry> memoryEntries;

    void Execute() {
        char line[255], fieldName[50];
        unsigned int fieldValue;

        auto fd = fopen("/proc/meminfo", "r");
        if (fd == NULL) {
            SetError("failed to open /proc/meminfo");
        }

        while (fgets(line, sizeof(line), fd) != NULL) {
            if (sscanf(line, "%s %u", (char*) &fieldName, &fieldValue) == 2) {
                if (procFields.find(fieldName) != procFields.end()) {
                    memoryEntries.push_back(MemInfoEntry{ procFields.at(fieldName), fieldValue });
                }
            }
        }
        fclose(fd);
    }

    void OnError(const Napi::Error& e) {
        std::stringstream error;
        error << e.what() << ", error code: " << errno << std::endl;
        Napi::Error::New(Env(), error.str()).ThrowAsJavaScriptException();
    }

    void OnOK() {
        Napi::HandleScope scope(Env());
        Napi::Object ret = Napi::Object::New(Env());

        for(size_t i = 0; i < memoryEntries.size(); i++) {
            ret.Set(memoryEntries[i].key, memoryEntries[i].value);
        }
        Callback().Call({Env().Null(), ret});
    }
};

#endif

Napi::Value getSysInfo(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    Napi::Function cb;

    if (info.Length() < 1) {
        Napi::Error::New(env, "Wrong number of argument provided!").ThrowAsJavaScriptException();
        return env.Null();
    }
    if (!info[0].IsFunction()) {
        Napi::Error::New(env, "argument callback should be a Function!").ThrowAsJavaScriptException();
        return env.Null();
    }

    cb = info[0].As<Napi::Function>();
    (new SysInfoWorker(cb))->Queue();
    
    return env.Undefined();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("getSysInfo", Napi::Function::New(env, getSysInfo));

    return exports;
}

NODE_API_MODULE(Nixmem, Init)
