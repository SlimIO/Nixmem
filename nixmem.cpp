#include "napi.h"
#include <sys/sysinfo.h>
#include <iostream>
#include <string>

using namespace std;
using namespace Napi;


Value getSysInfo(const CallbackInfo& info){
    Env env = info.Env();

    sysinfo s_info;
    int status = sysinfo(&s_info);
    printf("Code error = %d\n", error);
    
    Object ret = Object::New(env);  
    ret.Set("loads", s_info.loads);
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
