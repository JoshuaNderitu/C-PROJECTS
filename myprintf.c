#include <unistd.h>
#include <stdarg.h>
#include <stdint.h>

/* ============================================================
   LOW-LEVEL OUTPUT UTILITIES
   These replace standard library functions like puts/printf
   ============================================================ */

void put_char(char c) {
    write(1, &c, 1);
}

void put_str(const char *s) {
    if (!s) return;
    while (*s)
        put_char(*s++);
}

int str_len(const char *s) {
    int i = 0;
    while (s && s[i])
        i++;
    return i;
}

/* ============================================================
   NUMBER → STRING CONVERSION
   Supports: signed, unsigned, hex (upper/lower)
   ============================================================ */

void put_unsigned(unsigned long n) {
    if (n >= 10)
        put_unsigned(n / 10);
    put_char((n % 10) + '0');
}

void put_signed(long n) {
    if (n < 0) {
        put_char('-');
        n = -n;
    }
    put_unsigned(n);
}

void put_hex(unsigned long n, int uppercase) {
    char *base = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
    if (n >= 16)
        put_hex(n / 16, uppercase);
    put_char(base[n % 16]);
}

/* ============================================================
   PRINT WITH PADDING AND ALIGNMENT
   Handles: width, left-align, zero-padding
   ============================================================ */

void print_padded(const char *s, int width, int left_align, int zero_pad) {
    int len = str_len(s);
    int padding = (width > len) ? width - len : 0;

    if (!left_align) {
        char pad = zero_pad ? '0' : ' ';
        for (int i = 0; i < padding; i++)
            put_char(pad);
    }

    put_str(s);

    if (left_align) {
        for (int i = 0; i < padding; i++)
            put_char(' ');
    }
}

/* ============================================================
   INTEGER FORMATTER WITH PADDING
   Handles: +, -, width, zero-pad
   ============================================================ */

void print_int(long n, int width, int left_align, int zero_pad, int show_plus) {
    char buffer[32];
    int i = 0;
    long temp = n;

    // Convert number to string (reverse order)
    int negative = (n < 0);
    if (negative) n = -n;

    do {
        buffer[i++] = (n % 10) + '0';
        n /= 10;
    } while (n > 0);

    // Add sign
    if (negative)
        buffer[i++] = '-';
    else if (show_plus)
        buffer[i++] = '+';

    buffer[i] = '\0';

    // Reverse buffer
    for (int j = 0; j < i / 2; j++) {
        char tmp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = tmp;
    }

    print_padded(buffer, width, left_align, zero_pad);
}

/* ============================================================
   FORMAT PARSER
   Handles flags: -, 0, +
   Handles width: numbers
   Handles specifiers: c, s, d, i, u, x, X, p
   ============================================================ */

int my_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {

        // Normal character → print directly
        if (*fmt != '%') {
            put_char(*fmt++);
            continue;
        }

        fmt++; // skip %

        // -------- FLAGS --------
        int left_align = 0;
        int zero_pad = 0;
        int show_plus = 0;

        while (*fmt == '-' || *fmt == '0' || *fmt == '+') {
            if (*fmt == '-') left_align = 1;
            if (*fmt == '0') zero_pad = 1;
            if (*fmt == '+') show_plus = 1;
            fmt++;
        }

        // -------- WIDTH --------
        int width = 0;
        while (*fmt >= '0' && *fmt <= '9') {
            width = width * 10 + (*fmt - '0');
            fmt++;
        }

        // -------- SPECIFIER --------
        char spec = *fmt++;

        switch (spec) {

        case 'c': {
            char c = va_arg(args, int);
            char s[2] = {c, '\0'};
            print_padded(s, width, left_align, zero_pad);
            break;
        }

        case 's': {
            const char *s = va_arg(args, const char*);
            if (!s) s = "(null)";
            print_padded(s, width, left_align, zero_pad);
            break;
        }

        case 'd':
        case 'i': {
            long d = va_arg(args, int);
            print_int(d, width, left_align, zero_pad, show_plus);
            break;
        }

        case 'u': {
            unsigned long u = va_arg(args, unsigned int);
            char buffer[32];
            int i = 0;
            unsigned long temp = u;

            // Convert unsigned → string
            do {
                buffer[i++] = (temp % 10) + '0';
                temp /= 10;
            } while (temp);

            buffer[i] = '\0';

            // Reverse it
            for (int j = 0; j < i / 2; j++) {
                char tmp = buffer[j];
                buffer[j] = buffer[i - j - 1];
                buffer[i - j - 1] = tmp;
            }

            print_padded(buffer, width, left_align, zero_pad);
            break;
        }

        case 'x':
        case 'X': {
            unsigned long hex = va_arg(args, unsigned int);
            char buffer[32];
            int i = 0;

            char *base = (spec == 'x')
                ? "0123456789abcdef"
                : "0123456789ABCDEF";

            unsigned long tmp = hex;
            do {
                buffer[i++] = base[tmp % 16];
                tmp /= 16;
            } while (tmp);

            buffer[i] = '\0';

            // Reverse
            for (int j = 0; j < i / 2; j++) {
                char t = buffer[j];
                buffer[j] = buffer[i - j - 1];
                buffer[i - j - 1] = t;
            }

            print_padded(buffer, width, left_align, zero_pad);
            break;
        }

        case 'p': {
            unsigned long ptr = (unsigned long)va_arg(args, void*);
            put_str("0x");
            put_hex(ptr, 0);
            break;
        }

        default:
            put_char(spec);
        }
    }

    va_end(args);
    return 0;
}

/* ============================================================
   EXAMPLE MAIN (you can delete this)
   ============================================================ */

int main() {
    my_printf("Normal: Hello %s!\n", "World");
    my_printf("Width: [%10s]\n", "test");
    my_printf("Left : [%-10s]\n", "test");
    my_printf("Zero : [%05d]\n", 42);
    my_printf("Plus : [%+5d]\n", 42);
    my_printf("Hex  : [%08x]\n", 48879);
    my_printf("Ptr  : [%p]\n", main);

    return 0;
}
