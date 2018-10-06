#include "napi.h"
// #include <sys/sysinfo.h>
#include <sys/types.h>
#include <vm/vm_param.h>
#include <sys/sysctl.h>
#include <iostream>
#include <string>
#include <sys/vmmeter.h>
#include <sys/resource.h>

using namespace std;
using namespace Napi;


template<class T>
T sysInfo(const int firstLevel, const int secondLevel ){
    int name[2];
    name[0] = firstLevel;
    name[1] = secondLevel;
    T infoValue;
    size_t len = sizeof(infoValue);
    sysctl(name, (u_int) 2, &infoValue, &len, NULL, 0);
    return infoValue;
}

int sysInfo(const int firstLevel, const int secondLevel, void *info ){
    int name[2];
    name[0] = firstLevel;
    name[1] = secondLevel;
    size_t len = sizeof(info);
    int status = sysctl(name, (u_int) 2, &info, &len, NULL, 0);
    return status;
}

Value getSysInfo(const CallbackInfo& info){
    Env env = info.Env();

    unsigned int  nCPU = sysInfo<unsigned int>(CTL_HW, HW_NCPU);
    long byteOrder = sysInfo<long>(CTL_HW, HW_BYTEORDER);
    long physMem = sysInfo<long>(CTL_HW, HW_PHYSMEM);
    long userMem = sysInfo<long>(CTL_HW, HW_USERMEM);
    long pageSize = sysInfo<long>(CTL_HW, HW_PAGESIZE);
    long realMem = sysInfo<long>(CTL_HW, HW_REALMEM);

    // struct loadavg load_info;
    // int statusLoadavg = sysInfo(CTL_VM, VM_LOADAVG, &load_info);
    struct vmtotal vm_info;
    int statusVmtotal = sysInfo(CTL_VM, VM_TOTAL, &vm_info);
    bool swapping_enable = sysInfo<bool>(CTL_VM, VM_SWAPPING_ENABLED);
    long vFreeMin = sysInfo<long>(CTL_VM, VM_V_FREE_MIN);
    long vFreeReserved = sysInfo<long>(CTL_VM, VM_V_FREE_RESERVED);
    long vFreeTarget = sysInfo<long>(CTL_VM, VM_V_FREE_TARGET);
    long vInactiveTarget = sysInfo<long>(CTL_VM, VM_V_INACTIVE_TARGET);
    long vPageoutFreeMin = sysInfo<long>(CTL_VM, VM_V_PAGEOUT_FREE_MIN);
    // long vOvercommit = sysInfo<long>(CTL_VM, VM_OVERCOMMIT);

    Object vmtotal = Object::New(env);
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

    Object ret = Object::New(env);
    ret.Set("nCPU", nCPU);
    ret.Set("byteOrder", byteOrder);
    ret.Set("physMem", physMem);
    ret.Set("userMem", userMem);
    ret.Set("pageSize", pageSize);
    ret.Set("realMem", realMem);
    ret.Set("vmtotal", vmtotal);
    ret.Set("swapping_enable", swapping_enable);
    ret.Set("vFreeMin", vFreeMin);
    ret.Set("vFreeReserved", vFreeReserved);
    ret.Set("vFreeTarget", vFreeTarget);
    ret.Set("vInactiveTarget", vInactiveTarget);
    ret.Set("vPageoutFreeMin", vPageoutFreeMin);

    return ret;
}


/*
 * Initialize Node.JS Binding exports
 * 
 * @header: napi.h
 */
Object Init(Env env, Object exports) {
    exports.Set("getSysInfo", Function::New(env, getSysInfo));
    return exports;
}

// Export
NODE_API_MODULE(Winservices, Init)
