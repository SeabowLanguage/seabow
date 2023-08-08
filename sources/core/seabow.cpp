/**
 * @file seabow.cpp
 * @author LucaStarz
 * @brief 
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2023
*/

#include "core/seabow.hpp"

sbw_none init_seabow(sbw_none)
{
#ifdef _WIN32 // Enable UNICODE on Windows
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    SetConsoleTitleA((LPCSTR)"seabow");
#endif
}

std::vector<SeabowOption*> get_seabow_options(int argc, char **argv)
{
    std::vector<SeabowOption*> seabow_options;
    if (argc < 2) {
        seabow_options.push_back(new SeabowOption(OPT_INTERPRETER));
        return seabow_options;
    }
    
    // Parse seabow command
    if (string_equals(argv[1], "int"))
        seabow_options.push_back(new SeabowOption(OPT_INTERPRETER));
    else if (string_equals(argv[1], "build"))
        seabow_options.push_back(new SeabowOption(OPT_BYTECODE_COMPILER));
    else if (string_equals(argv[1], "run"))
        seabow_options.push_back(new SeabowOption(OPT_BYTECODE_INTERPRETER));
    else if (string_equals(argv[1], "cmp"))
        seabow_options.push_back(new SeabowOption(OPT_COMPILER));
    else if (string_equals(argv[1], "pack"))
        seabow_options.push_back(new SeabowOption(OPT_PACKAGE_LIBRARY));
    else if (string_equals(argv[1], "install"))
        seabow_options.push_back(new SeabowOption(OPT_INSTALL));
    else if (string_equals(argv[1], "publish"))
        seabow_options.push_back(new SeabowOption(OPT_PUBLISH));

    // Parse seabow options
    for (int i=seabow_options.size()+1; i<argc; i++) {
        char *current = argv[i];
        if (string_equals(current, "-o") || string_equals(current, "--output")) {
            // TODO
        } /* --output */ else if (string_equals(current, "-h") || string_equals(current, "--help")) {
            seabow_options.insert(seabow_options.begin(), new SeabowOption(OPT_HELP));
            return seabow_options;
        } /* --help */ else {
            seabow_options.insert(seabow_options.begin(), new SeabowOption(OPT_ERROR, "Unknown seabow option '" + sbw_string(current) + "', try 'seabow -h' for more helps"));
            return seabow_options;
        } /* Unknown seabow option */
    }

    return seabow_options;
}