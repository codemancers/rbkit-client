#ifndef RBKIT_DEBUG_H
#define RBKIT_DEBUG_H


// have macros which can help in easing out printing debug information
#define DEBUG_THREAD_IDS
#define DEBUG_FUNCTION_NAMES
#define DEBUG_LINE_NUMBERS


// helpers for sringifying macros. note the reverse order!
#define _XSTR_(s) _STR_(s)
#define _STR_(s) #s


#ifdef DEBUG_THREAD_IDS
#define DUMP_THREAD_FORMAT  "tid:%x "
#define DUMP_THREAD_ARG ,(QThread::currentThreadId())
#else
#define DUMP_THREAD_FORMAT
#define DUMP_THREAD_ARG
#endif


#ifdef DEBUG_FUNCTION_NAMES
#define DUMP_FUNCTION_NAME_FORMAT  "%s()"
#define DUMP_FUNCTION_NAME_ARG  ,__func__
#else
#define DUMP_FUNCTION_NAME_FORMAT
#define DUMP_FUNCTION_NAME_ARG
#endif


#ifdef DEBUG_LINE_NUMBERS
#define DUMP_LINE_NUMBER_FORMAT  ":" _XSTR_(__LINE__)
#else
#define DUMP_LINE_NUMBER_FORMAT
#endif


#define PREFIX_FORMAT DUMP_THREAD_FORMAT DUMP_FUNCTION_NAME_FORMAT DUMP_LINE_NUMBER_FORMAT
#define PREFIX_ARGS   DUMP_THREAD_ARG DUMP_FUNCTION_NAME_ARG


#define rbPrintf  qDebug


// define macros here which can be used.
#define ENTER_SIGN_  "> "
#define EXIT_SIGN_   "< "


#define ENTER0(format)                        rbPrintf(ENTER_SIGN_ PREFIX_FORMAT format PREFIX_ARGS)
#define ENTER1(format, a1)                    rbPrintf(ENTER_SIGN_ PREFIX_FORMAT format PREFIX_ARGS, a1)
#define ENTER2(format, a1, a2)                rbPrintf(ENTER_SIGN_ PREFIX_FORMAT format PREFIX_ARGS, a1, a2)
#define ENTER3(format, a1, a2, a3)            rbPrintf(ENTER_SIGN_ PREFIX_FORMAT format PREFIX_ARGS, a1, a2, a3)

#define EXIT0(format)                         rbPrintf(EXIT_SIGN_ PREFIX_FORMAT format PREFIX_ARGS)
#define EXIT1(format, a1)                     rbPrintf(EXIT_SIGN_ PREFIX_FORMAT format PREFIX_ARGS, a1)
#define EXIT2(format, a1, a2)                 rbPrintf(EXIT_SIGN_ PREFIX_FORMAT format PREFIX_ARGS, a1, a2)


#endif // RBKIT_DEBUG_H
