#ifndef RBKIT_COMMANDS_H
#define RBKIT_COMMANDS_H

#include <QString>


namespace RBKit
{
    class CommandBase
    {
    public:
        virtual QString serialize() const = 0;
    };

    class CmdStartProfile : public CommandBase
    {
        virtual QString serialize() const;
    };

    class CmdStopProfile : public CommandBase
    {
        virtual QString serialize() const;
    };

    class CmdObjSnapshot : public CommandBase
    {
        virtual QString serialize() const;
    };

    class CmdTriggerGC : public CommandBase
    {
        virtual QString serialize() const;
    };
}

#endif // RBKIT_COMMANDS_H
