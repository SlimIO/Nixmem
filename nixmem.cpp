#include "napi.h"
#include <sys/sysinfo.h>
#include <iostream>
#include <string>

using namespace std;
using namespace Napi;

Value getSysInfo(const CallbackInfo& info){
    Env env = info.Env();

    struct sysinfo s_info;
    int status = sysinfo(&s_info);
    if(status != 0){
        printf("Code error = %d\n", status);
    }

    Object ret = Object::New(env);
    Array loadArr = Array::New(env, (size_t) 3);
    loadArr[(unsigned)0] = Number::New(env, s_info.loads[0]);
    loadArr[(unsigned)1] = Number::New(env, s_info.loads[1]);
    loadArr[(unsigned)2] = Number::New(env, s_info.loads[2]);
    
    ret.Set("uptime", s_info.uptime);
    ret.Set("loads", loadArr);
    ret.Set("totalram", s_info.totalram);
    ret.Set("freeram", s_info.freeram);
    ret.Set("sharedram", s_info.sharedram);
    ret.Set("bufferram", s_info.bufferram);
    ret.Set("totalswap", s_info.totalswap);
    ret.Set("freeswap", s_info.freeswap);
    ret.Set("procs", s_info.procs);
    ret.Set("totalhigh", s_info.totalhigh);
    ret.Set("freehigh", s_info.freehigh);
    ret.Set("mem_unit", s_info.mem_unit);
    ret.Set("_f", s_info._f);

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
