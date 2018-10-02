#include "napi.h"
// #include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <iostream>
#include <string>

using namespace std;
using namespace Napi;


template<class T>
T sysInfo(const int firstLevel, const int secondLevel ){
    int name[2];
    name[0] = firstLevel;
    name[1] = secondLevel;
    size_t len;
    sysctl(name, (u_int) 2, NULL, &len, NULL, 0);
    T infoValue = (T) malloc(len);
    sysctl(name, (u_int) 2, (void *)infoValue, &len, NULL, 0);
    //cout << "typeof : " <<  typeid(infoValue).name() << endl;
    return infoValue;
}
 
// int sysCtl(const int firstLevel, const int secondLevel ){
//     int name[2];
//     name[0] = firstLevel;
//     name[1] = secondLevel;
//     size_t len;
//     sysctl(name, (u_int) 2, NULL, &len, NULL, 0);
//     int  infoValue = (int) malloc(len);
//     sysctl(name, (u_int) 2, (void *)infoValue, &len, NULL, 0);
    
//     return String(infoValue);
// }


Value getSysInfo(const CallbackInfo& info){
    Env env = info.Env();

    // const int name[] = {
    //     CTL_HW,
    //     HW_MACHINE
    //     HW_MODEL,
    //     HW_NCPU,
    //     HW_BYTEORDER,
    //     HW_PHYSMEM,
    //     HW_USERMEM,
    //     HW_MACHINE_ARCH,
    //     HW_REALMEM
    // };

    // u_int namelen = sizeof(name);
    char * machine = sysInfo<char *>(CTL_HW, HW_MACHINE);
    char * model = sysInfo<char *>(CTL_HW, HW_MODEL);
    long int nCPU = sysInfo<long int>(CTL_HW, HW_NCPU);
    long int  byteOrder = sysInfo<long int >(CTL_HW, HW_BYTEORDER);
    long int  physMem = sysInfo<long int >(CTL_HW, HW_PHYSMEM);
    long int  userMem = sysInfo<long int >(CTL_HW, HW_USERMEM);
    char * machineArch = sysInfo<char *>(CTL_HW, HW_MACHINE_ARCH);
    long int  realMem = sysInfo<long int >(CTL_HW, HW_REALMEM);

    // cout << "HW_MACHINE : " << machine << endl;
    // cout << "HW_MODEL : " << model << endl;
    // cout << "HW_NCPU : " << nCPU << endl;
    // cout << "HW_BYTEORDER : " << byteOrder << endl;
    // cout << "HW_PHYSMEM : " << physMem << endl;
    // cout << "HW_USERMEM : " << userMem << endl;
    // cout << "HW_MACHINE_ARCH : " << machineArch << endl;
    // cout << "HW_REALMEM : " << realMem << endl;
    
    Object ret = Object::New(env);

    ret.Set("machine", machine);
    ret.Set("model", model);
    ret.Set("nCPU", nCPU);
    ret.Set("byteOrder", byteOrder);
    ret.Set("physMem", physMem);
    ret.Set("physMem", userMem);
    ret.Set("physMem", machineArch);
    ret.Set("physMem", realMem);

    return ret;
}


// Value getSysInfo(const CallbackInfo& info){
//     Env env = info.Env();

//     struct sysinfo s_info;
//     int status = sysinfo(&s_info);
//     if(status != 0){
//         printf("Code error = %d\n", status);
//     }

//     Object ret = Object::New(env);
//     Array loadArr = Array::New(env, (size_t) 3);
//     loadArr[(unsigned)0] = Number::New(env, s_info.loads[0]);
//     loadArr[(unsigned)1] = Number::New(env, s_info.loads[1]);
//     loadArr[(unsigned)2] = Number::New(env, s_info.loads[2]);
    
//     ret.Set("uptime", s_info.uptime);
//     ret.Set("loads", loadArr);
//     ret.Set("totalram", s_info.totalram);
//     ret.Set("freeram", s_info.freeram);
//     ret.Set("sharedram", s_info.sharedram);
//     ret.Set("bufferram", s_info.bufferram);
//     ret.Set("totalswap", s_info.totalswap);
//     ret.Set("freeswap", s_info.freeswap);
//     ret.Set("procs", s_info.procs);
//     ret.Set("totalhigh", s_info.totalhigh);
//     ret.Set("freehigh", s_info.freehigh);
//     ret.Set("mem_unit", s_info.mem_unit);
//     ret.Set("_f", s_info._f);

//     return ret;
// }


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
