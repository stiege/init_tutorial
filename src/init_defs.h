typedef void (init_func)(void);

#define REGISTER_INIT_FUNCTION(func)        \
__attribute__((section(".init_functions"))) \
init_func * func ## _registration_ptr = &func;
