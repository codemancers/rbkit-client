#ifndef RBKIT_COMMANDS_H
#define RBKIT_COMMANDS_H


namespace RBKit
{
    class CommandBase
    {
    public:
        virtual const char* serialize() = 0;
    };

    class CmdStartProfile : public CommandBase
    {
        virtual const char* serialize();
    };

    class CmdStopProfile : public CommandBase
    {
        virtual const char* serialize();
    };

    class CmdObjSnapshot : public CommandBase
    {
        virtual const char* serialize();
    };

    class CmdTriggerGC : public CommandBase
    {
        virtual const char* serialize();
    };
}

#endif // RBKIT_COMMANDS_H
