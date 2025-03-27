#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <filesystem>
#include <optional>
#include <sstream>

struct CompilerOptions {
    std::string input_file;
    std::optional<std::string> output_file;
    bool generate_python = false;
    bool generate_cpp = false;
};

struct Instruction {
    enum class Type {
        SET,
        ADD,
        SUB
    };
    Type type;
    int value;
};

std::optional<CompilerOptions> parse_args(int argc, char* argv[]) {
    CompilerOptions options;
    
    for (int i = 1; i < argc; i++) {
        std::string_view arg = argv[i];
        
        if (arg == "--python") {
            options.generate_python = true;
        }
        else if (arg == "--cpp") {
            options.generate_cpp = true;
        }
        else if (arg == "-o" && i + 1 < argc) {
            options.output_file = argv[++i];
        }
        else if (arg[0] == '-') {
            std::cerr << "Unknown option: " << arg << std::endl;
            return std::nullopt;
        }
        else {
            options.input_file = arg;
        }
    }

    if (options.input_file.empty()) {
        std::cerr << "No input file specified" << std::endl;
        return std::nullopt;
    }

    if (!options.generate_python && !options.generate_cpp) {
        options.generate_cpp = true; // Default to C++
    }

    if (options.generate_python && options.generate_cpp) {
        std::cerr << "Cannot generate both Python and C++ output" << std::endl;
        return std::nullopt;
    }

    return options;
}

std::vector<Instruction> parse_slg_file(const std::string& filename) {
    std::vector<Instruction> instructions;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::string_view sv(line);
        size_t space_pos = sv.find(' ');
        if (space_pos == std::string_view::npos) {
            throw std::runtime_error("Invalid instruction format: " + line);
        }

        std::string_view cmd = sv.substr(0, space_pos);
        std::string_view value_str = sv.substr(space_pos + 1);
        
        int value;
        try {
            value = std::stoi(std::string(value_str));
        } catch (const std::exception&) {
            throw std::runtime_error("Invalid number format: " + std::string(value_str));
        }

        Instruction inst;
        if (cmd == "SET") inst.type = Instruction::Type::SET;
        else if (cmd == "ADD") inst.type = Instruction::Type::ADD;
        else if (cmd == "SUB") inst.type = Instruction::Type::SUB;
        else throw std::runtime_error("Unknown instruction: " + std::string(cmd));
        
        inst.value = value;
        instructions.push_back(inst);
    }

    return instructions;
}

std::string generate_cpp(const std::vector<Instruction>& instructions) {
    std::string output = "#include <iostream>\n\nint main() {\n";
    
    bool first = true;
    for (const auto& inst : instructions) {
        if (first) {
            std::stringstream ss;
            ss << "  int state = " << inst.value << ";\n";
            output += ss.str();
            first = false;
        } else {
            std::stringstream ss;
            switch (inst.type) {
                case Instruction::Type::ADD:
                    ss << "  state += " << inst.value << ";\n";
                    break;
                case Instruction::Type::SUB:
                    ss << "  state -= " << inst.value << ";\n";
                    break;
                default:
                    break;
            }
            output += ss.str();
        }
    }
    
    output += "  std::cout << \"state = \" << state << std::endl;\n";
    output += "  return 0;\n}\n";
    return output;
}

std::string generate_python(const std::vector<Instruction>& instructions) {
    std::string output;
    
    bool first = true;
    for (const auto& inst : instructions) {
        if (first) {
            std::stringstream ss;
            ss << "state = " << inst.value << "\n";
            output += ss.str();
            first = false;
        } else {
            std::stringstream ss;
            switch (inst.type) {
                case Instruction::Type::ADD:
                    ss << "state += " << inst.value << "\n";
                    break;
                case Instruction::Type::SUB:
                    ss << "state -= " << inst.value << "\n";
                    break;
                default:
                    break;
            }
            output += ss.str();
        }
    }
    
    output += "print(f\"state = {state}\")\n";
    return output;
}

int main(int argc, char* argv[]) {
    try {
        auto options = parse_args(argc, argv);
        if (!options) {
            return 1;
        }

        auto instructions = parse_slg_file(options->input_file);
        std::string output;
        
        if (options->generate_cpp) {
            output = generate_cpp(instructions);
        } else {
            output = generate_python(instructions);
        }

        if (options->output_file) {
            std::ofstream out_file(*options->output_file);
            if (!out_file.is_open()) {
                std::cerr << "Could not open output file: " << *options->output_file << std::endl;
                return 1;
            }
            out_file << output;
        } else {
            std::cout << output;
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
