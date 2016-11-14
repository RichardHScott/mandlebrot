#include "parseinput.h"

std::unique_ptr<Input_Params> ParseInput::Parse(int argc, char** argv) {
    return parse_args(argc, argv); 
}

std::unique_ptr<Input_Params> ParseInput::parse_args(int argc, char** argv) {
    auto params = std::make_unique<Input_Params>();

    for(int i = 0; i < argc; ++i) {
        if(argv[i][0] != '-') {
            continue;
        }

        if(argv[i][1] == 'h' && argv[i][2] == '\0') {
            //print_usage();
            return nullptr;
        }

        char* argument  = argv[i] + sizeof(char);

        std::string arg(argument);
        size_t pos = arg.find(std::string("="));

        if(pos == std::string::npos || pos == arg.length()) {
            continue;
        }

        //otherwise we have found command=value so we split it
        std::string command = arg.substr(0, pos);
        std::string value = arg.substr(pos+1, arg.length());

        const auto command_list = std::map<std::string, std::function<void()>>{
            { "xmin", [&params, &value] { params->x_min = std::stod(value); } },
            { "xmax", [&params, &value] { params->x_max = std::stod(value); } },
            { "ymin", [&params, &value] { params->y_min = std::stod(value); } },
            { "ymax", [&params, &value] { params->y_max = std::stod(value); } },
            { "xdivs", [&params, &value] { params->x_divisions = std::stoi(value); } },
            { "ydivs", [&params, &value] { params->y_divisions = std::stoi(value); } },
            { "strat", [&params, &value] {
                for(auto const & strat : strategies)
                    if(strat.m_name == value) {
                        params->strategy = strat;
                        break;
                    }
                } 
            },
            { "vel", [&params, &value] { params->compute_velocity = (value == "true"); } }
        };

        auto iter = command_list.find(command);
        if(iter != command_list.end()) {
            (iter->second)();
        }
    }

    return params;
}