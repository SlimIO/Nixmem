#include "napi.h"
#include <errno.h>
#include <sstream>
#include <iostream>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/vmmeter.h>
#include <sys/resource.h>
#include <vm/vm_param.h>

using namespace std;
using namespace Napi;

template<class T>
T sysInfo(const int firstLevel, const int secondLevel) {
    int name[2] = {firstLevel, secondLevel};
    T infoValue;
    size_t len = sizeof(infoValue);
    sysctl(name, (u_int) 2, &infoValue, &len, NULL, 0);

    return infoValue;
}

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

Value getSysInfo(const CallbackInfo& info){
    Env env = info.Env();
    struct vmtotal vm_info;

    if (sysInfo(CTL_VM, VM_TOTAL, &vm_info) == -1) {
        stringstream err;
        err << "sysctl failed for CTL_VM.VM_TOTAL: " << errno << endl;
        Error::New(env, err.str()).ThrowAsJavaScriptException();
        return env.Null();
    }

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
    ret.Set("nCPU", sysInfo<unsigned int>(CTL_HW, HW_NCPU));
    ret.Set("byteOrder", sysInfo<long>(CTL_HW, HW_BYTEORDER));
    ret.Set("physMem", sysInfo<long>(CTL_HW, HW_PHYSMEM));
    ret.Set("userMem", sysInfo<long>(CTL_HW, HW_USERMEM));
    ret.Set("pageSize", sysInfo<long>(CTL_HW, HW_PAGESIZE));
    ret.Set("realMem", sysInfo<long>(CTL_HW, HW_REALMEM));
    ret.Set("vmtotal", vmtotal);
    ret.Set("swapping_enable", sysInfo<bool>(CTL_VM, VM_SWAPPING_ENABLED));
    ret.Set("vFreeMin", sysInfo<long>(CTL_VM, VM_V_FREE_MIN));
    ret.Set("vFreeReserved", sysInfo<long>(CTL_VM, VM_V_FREE_RESERVED));
    ret.Set("vFreeTarget", sysInfo<long>(CTL_VM, VM_V_FREE_TARGET));
    ret.Set("vInactiveTarget", sysInfo<long>(CTL_VM, VM_V_INACTIVE_TARGET));
    ret.Set("vPageoutFreeMin", sysInfo<long>(CTL_VM, VM_V_PAGEOUT_FREE_MIN));

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
NODE_API_MODULE(Nixmem, Init)
