#include <iostream>
#include <map>

#define PART 2

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
enum my_enum {
    EXAMPLE_1,
    EXAMPLE_2,
    EXAMPLE_3,
    EXAMPLE_4,
};

class my_class {
public:
    int a;
    double b;
    std::string str;
    my_enum options;
};

class my_class_available_vars {
public:
    enum available_vars {
        a,
        b,
        str,
        options,
    };

    my_class_available_vars() {
        available_vars_collection = {
                {"a", a},
                {"b", b},
                {"str", str},
                {"options", options},
        };
    }

    available_vars get_var_md(const std::string &var_name) const {
        return available_vars_collection.at(var_name);
    }

private:
    std::map<std::string, available_vars> available_vars_collection;
};

template <my_class_available_vars::available_vars VAR_NAME>
class get_prop {};

template <>
class get_prop<my_class_available_vars::a> {
public:
    explicit get_prop(my_class *my_class_ptr) : mc_ptr(my_class_ptr) {}

    int& prop() {
        return mc_ptr->a;
    }

private:
    my_class *mc_ptr;
};

template <>
class get_prop<my_class_available_vars::b> {
public:
    explicit get_prop(my_class *my_class_ptr) : mc_ptr(my_class_ptr) {}

    double& prop() {
        return mc_ptr->b;
    }

private:
    my_class *mc_ptr;
};

template <>
class get_prop<my_class_available_vars::str> {
public:
    explicit get_prop(my_class *my_class_ptr) : mc_ptr(my_class_ptr) {}

    std::string& prop() {
        return mc_ptr->str;
    }

private:
    my_class *mc_ptr;
};

template <>
class get_prop<my_class_available_vars::options> {
public:
    explicit get_prop(my_class *my_class_ptr) : mc_ptr(my_class_ptr) {}

    my_enum& prop() {
        return mc_ptr->options;
    }

private:
    my_class *mc_ptr;
};


int main() {
    my_class mc;
    const my_class_available_vars mc_vars;
    const my_class_available_vars::available_vars av = my_class_available_vars::options;
    get_prop<av>

    return EXIT_SUCCESS;
}
#elif PART == 3
#endif