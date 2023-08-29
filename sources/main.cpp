#include "core/seabow.hpp"

int main(int argc, char **argv)
{
    std::vector<SeabowOption*> seabow_options = get_seabow_options(argc, argv);
    SeabowOption *opt1 = seabow_options.front();
    switch (opt1->OptionType()) {
        case OPT_ERROR: {
            sbw_print(3, "\033[31mERROR: ", opt1->Value().c_str(), "\n\033[0m");
        } return 1;

        case OPT_HELP: {
            sbw_print(1, "\033[36mSeabow is a tool for managing Seabow files (.sbw, .sbb)\n\n\tUsage: seabow <command> [options]\n\nSeabow commands:\n\t- <none>:\tstart seabow interpreter\n\t- int:\t\tstart seabow interpreter or interpret seabow source code (.sbw)\n\t- build:\tCompile seabow source code (.sbw) to seabow bytecode file (.sbb)\n\t- run:\t\tRun seabow bytecode file (.sbb)\n\t- cmp:\t\tCompile seabow source code (.sbw) to native code executable (.exe, ...)\n\t- pack:\t\tCompile seabow source code (.sbw) to seabow library\n\t- install:\tInstall seabow library(ies)\n\t- publish:\tManage a seabow library\n\n\nSeabow options:\n\t- -h (--help):\tShow seabow helps\n\t- -o (--output):\tSpecify an output path\n\033[0m");
        } return 0;

        case OPT_INTERPRETER: {

        } break;

        case OPT_BYTECODE_COMPILER: {

        } break;

        case OPT_BYTECODE_INTERPRETER: {

        } break;

        case OPT_COMPILER: {
            
        } break;

        default: break;
    }

    sbw_print(3, "\n\n---\nint128: ", std::to_string(sizeof(sbw_int128)).c_str(), "\n");
    sbw_print(3, "double: ", std::to_string(sizeof(sbw_double)).c_str(), "\n");
    sbw_print(3, "ldouble: ", std::to_string(sizeof(sbw_ldouble)).c_str(), "\n");
    sbw_print(3, "char: ", std::to_string(sizeof(sbw_char)).c_str(), "\n");
    sbw_print(3, "char of string: ", std::to_string(sizeof(sbw_string("²").at(0))).c_str(), "\n");
    sbw_print(1, "平仮名, ひらがな  ☠️\n---\n");

    return 0;
}
