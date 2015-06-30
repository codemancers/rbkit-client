#ifndef RBKIT_COMMANDS_H
#define RBKIT_COMMANDS_H

#include <QString>


namespace RBKit
{
    class CommandBase
    {
    public:
        virtual QString serialize() = 0;
    };

    class CmdStartProfile : public CommandBase
    {
        virtual QString serialize();
    };

    class CmdStopProfile : public CommandBase
    {
        virtual QString serialize();
    };

    class CmdObjSnapshot : public CommandBase
    {
        virtual QString serialize();
    };

    class CmdTriggerGC : public CommandBase
    {
        virtual QString serialize();
    };

    class CmdPing : public CommandBase
    {
    public:
        virtual QString serialize();
    };

    class CmdHandshake : public CommandBase
    {
    public:
        virtual QString serialize();
    };

    class CmdStartCPUProfile : public CommandBase
    {
        virtual QString serialize();
    };

    class CmdStopCPUProfile : public CommandBase
    {
        virtual QString serialize();
    };
}

#endif // RBKIT_COMMANDS_H
