#include "rbcommands.h"

QString RBKit::CmdStartProfile::serialize()
{
    return QString("start_memory_profile");
}


QString RBKit::CmdStopProfile::serialize()
{
    return QString("stop_memory_profile");
}


QString RBKit::CmdObjSnapshot::serialize()
{
    return QString("objectspace_snapshot");
}


QString RBKit::CmdTriggerGC::serialize()
{
    return QString("trigger_gc");
}


QString RBKit::CmdPing::serialize()
{
    return QString("ping");
}


QString RBKit::CmdHandshake::serialize()
{
    return QString("handshake");
}

QString RBKit::CmdStartCPUProfile::serialize()
{
    return QString("start_cpu_profiling");
}

QString RBKit::CmdStopCPUProfile::serialize()
{
    return QString("stop_cpu_profiling");
}
