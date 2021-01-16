#include <iostream>
#include <map>
#include <typeindex>
#include <variant>

#define PART 3

#if PART == 1 // Functions Control
class my_class {
public:
    void function_a() const {
        std::cout << "my_class::function_a\n";
    }

    void function_b() const {
        std::cout << "my_class::function_b\n";
    }

    void function_c() const {
        std::cout << "my_class::function_c\n";
    }
};

class my_class_functions_collection {
private:
    using my_class_func_t = void(my_class::*)() const;

public:
    explicit my_class_functions_collection(my_class *my_class_ptr) : mc_ptr(my_class_ptr) {
        functions_collection = {
                {"function_a", &my_class::function_a},
                {"function_b", &my_class::function_b},
                {"function_c", &my_class::function_c},
        };
    }

    void call_function(std::string &&func_name) {
        (mc_ptr->*(functions_collection.at(func_name)))();
    }

private:
    std::map<std::string, my_class_func_t> functions_collection;
    my_class *mc_ptr;
};

int main() {
    my_class mc;
    my_class_functions_collection mc_functions(&mc);

    std::string desired_function;
    std::cin >> desired_function;
    mc_functions.call_function("function_a");
    mc_functions.call_function("function_b");
    mc_functions.call_function(std::move(desired_function));

    return EXIT_SUCCESS;
}
#elif PART == 2
// helper type for the visitor
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

struct my_class {
    int i;
    float f;
    std::string s;
    double d;
};

class my_class_available_vars {
public:
    using desired_class = my_class;
    using desired_class_int = int(desired_class::*);
    using desired_class_double = double(desired_class::*);
    using desired_class_float = float(desired_class::*);
    using desired_class_string = std::string(desired_class::*);
    using desired_class_variant = std::variant<desired_class_int, desired_class_float, desired_class_string, desired_class_double>;
    using simple_types_variant = std::variant<int, float, std::string, double>;

public:
    my_class_available_vars() {
        available_vars_collection = {
                {"i", &desired_class::i},
                {"f", &desired_class::f},
                {"s", &desired_class::s},
                {"d", &desired_class::d},
        };
    }

    [[nodiscard]] desired_class_variant get_var_md(const std::string &var_name) const {
        return available_vars_collection.at(var_name);
    }

private:
    std::map<std::string, desired_class_variant> available_vars_collection;
};

void input_value_based_on_variable_type(my_class_available_vars::simple_types_variant& user_new_val, const std::string &variable) {
    // The following section can be improved...
    if (variable == "i") {
        int i;
        std::cout << "Enter integer:\n";
        std::cin >> i;
        user_new_val = i;
    } else if (variable == "f") {
        float f;
        std::cout << "Enter float:\n";
        std::cin >> f;
        user_new_val = f;
    } else if (variable == "s") {
        std::string s;
        std::cout << "Enter string:\n";
        std::getline(std::cin >> std::ws, s);
        user_new_val = s;
    } else if (variable == "d") {
        double d;
        std::cout << "Enter double:\n";
        std::cin >> d;
        user_new_val = d;
    } else {
        throw std::runtime_error("Variable not found.");
    }
}

int main() {

    // Declarations

    my_class mc;
    mc.i = 3;
    mc.s = "My str";

    my_class_available_vars mc_vars;
    my_class_available_vars::desired_class_variant desired_variable;

    // Print function for desired_variable
    auto print_var = [&mc] (my_class_available_vars::desired_class_variant desired_variable) {
        std::visit(overloaded {
                [&](auto variable) { std::cout << mc.*(variable) << "\n"; },
        }, desired_variable);
    };

    // A function to set new value to a class variable
    auto set_var = [&mc] (my_class_available_vars::desired_class_variant desired_variable, const my_class_available_vars::simple_types_variant &new_val) {
        if (desired_variable.index() != new_val.index()) throw std::runtime_error("Desired variable doesn't match new_val type.");
        std::visit(overloaded {
            [&](my_class_available_vars::desired_class_int variable) { mc.*(variable) = std::get<int>(new_val); },
            [&](my_class_available_vars::desired_class_float variable) { mc.*(variable) = std::get<float>(new_val); },
            [&](my_class_available_vars::desired_class_string variable) { mc.*(variable) = std::get<std::string>(new_val); },
            [&](my_class_available_vars::desired_class_double variable) { mc.*(variable) = std::get<double>(new_val); },
        }, desired_variable);
    };

    // Print Variables

    print_var(mc_vars.get_var_md("i")); // 3
    print_var(mc_vars.get_var_md("s")); // My str

    // Set Variables

    desired_variable = mc_vars.get_var_md("s");
    set_var(desired_variable, "My new string");
    print_var(desired_variable); // My new string

    desired_variable = mc_vars.get_var_md("d");
    set_var(desired_variable, 0.56);
    print_var(desired_variable); // 0.56

    // User Input

    std::string variable;
    std::cout << "Choose variable (i, f, s, d):\n";
    std::cin >> variable;

    my_class_available_vars::simple_types_variant user_new_val;
    input_value_based_on_variable_type(user_new_val, variable);

    desired_variable = mc_vars.get_var_md(variable);
    set_var(desired_variable, user_new_val);
    print_var(desired_variable);

    return EXIT_SUCCESS;
}
#elif PART == 3

#endif