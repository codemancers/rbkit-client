#include "rbcommands.h"

const char* RBKit::CmdStartProfile::serialize()
{
    return "start_memory_profile";
}


const char* RBKit::CmdStopProfile::serialize()
{
    return "stop_memory_profile";
}


const char* RBKit::CmdObjSnapshot::serialize()
{
    return "objectspace_snapshot";
}


const char* RBKit::CmdTriggerGC::serialize()
{
    return "trigger_gc";
}
