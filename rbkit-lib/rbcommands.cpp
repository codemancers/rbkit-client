#include "rbcommands.h"

QString RBKit::CmdStartProfile::serialize() const
{
    return QString("start_memory_profile");
}


QString RBKit::CmdStopProfile::serialize() const
{
    return QString("stop_memory_profile");
}


QString RBKit::CmdObjSnapshot::serialize() const
{
    return QString("objectspace_snapshot");
}


QString RBKit::CmdTriggerGC::serialize() const
{
    return QString("trigger_gc");
}
