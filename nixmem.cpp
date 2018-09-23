#include "napi.h"
#include <iostream>
#include <string>

using namespace std;
using namespace Napi;

/*
 * Initialize Node.JS Binding exports
 * 
 * @header: napi.h
 */
Object Init(Env env, Object exports) {


    return exports;
}

// Export
NODE_API_MODULE(Winservices, Init)
