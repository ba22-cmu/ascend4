// Define a macro to concatenate tokens
#define CONCAT(a, b) a ## b

// Define a macro to convert a string literal to a token
#define UNSTRINGIZE(s) CONCAT(s, _token)

#define p "x"

char * y = UNSTRINGIZE(p);
