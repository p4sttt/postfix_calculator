#include "postfix.h"
#include "common.h"
#include "stack.h"

#include <ctype.h>
#include <stdio.h>

int
precedence (int c)
{
    switch (c) {
    case '!':
        return 3;
    case '&':
        return 2;
    case '|':
        return 1;
    default:
        return 0;
    }
}

char *
op2str (char op)
{
    switch (op) {
    case '!':
        return "!";
    case '&':
        return "&&";
    case '|':
        return "||";
    default:
        return "<ERR>";
    }
}

void
postfix (FILE *in, FILE *out)
{
    stack_t s;
    int c, peekc;

    stack_init (&s);

    while ((c = fgetc (in)) != EOF && c != '\n' && c != '=') {
        if (isspace (c)) {
            continue;
        }

        if (isdigit (c)) {
            int val;
            ungetc (c, in);
            fscanf (in, "%d", &val);
            fprintf (out, "%d ", val);
            continue;
        } else if (c == '-' || c == '+') {
            int val;
            if (fscanf (in, "%d", &val) != 1) {
                error ("Lexical error: Unexpected token sequence near '%c'", c);
            }
            fprintf (out, "%c%d ", c, val);
        }

        if (c == '(') {
            stack_push (&s, c);
            continue;
        } else if (c == ')') {
            while (!stack_empty (&s) && stack_peek (&s) != '(') {
                fprintf (out, "%s ", op2str (stack_pop (&s)));
            }
            if (stack_empty (&s)) {
                error ("Syntax error: Unbalanced parentheses");
            }
            stack_pop (&s);
            continue;
        }

        if (c == '!') {
            stack_push (&s, '!');
            continue;
        } else if (c == '&') {
            if ((peekc = fgetc (in)) != '&') {
                error (
                    "Lexical error: Excepted '&&' but found '%c%c'", c, peekc);
            }

            while (!stack_empty (&s) &&
                   precedence (stack_peek (&s)) >= precedence ('&')) {
                fprintf (out, "%s ", op2str (stack_pop (&s)));
            }
            stack_push (&s, '&');
            continue;
        } else if (c == '|') {
            if ((peekc = fgetc (in)) != '|') {
                error (
                    "Lexical error: Excepted '||' but found '%c%c'", c, peekc);
            }

            while (!stack_empty (&s) &&
                   precedence (stack_peek (&s)) >= precedence ('|')) {
                fprintf (out, "%s ", op2str (stack_pop (&s)));
            }
            stack_push (&s, '|');
            continue;
        }

        error ("Lexical error: Unexpected lexeme '%c'", c);
    }

    while (!stack_empty (&s)) {
        c = stack_pop (&s);
        fprintf (out, "%s ", op2str (c));
    }

    fprintf (out, "\n");
}

int
main (void)
{
    postfix (stdin, stdout);
    return 0;
}
