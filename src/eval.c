#include "eval.h"
#include "common.h"
#include "stack.h"

#include <ctype.h>
#include <stdio.h>

void
eval_postfix (FILE *in, FILE *out)
{
    int c, peekc;
    int rhs, lhs;
    stack_t s;

    stack_init (&s);

    while ((c = fgetc (in)) != EOF && c != '\n') {
        if (isspace (c)) {
            continue;
        }

        if (isdigit (c)) {
            ungetc (c, in);
            fscanf (in, "%d", &rhs);
            stack_push (&s, rhs);
            continue;
        } else if (c == '-' || c == '+') {
            if (fscanf (in, "%d", &rhs) != 1) {
                error ("Lexical error: Mismatch expression: Expected numeric "
                       "literal");
            }
            stack_push (&s, rhs);
            continue;
        }

        if (c == '!') {
            if (stack_empty (&s)) {
                error ("Semantic error: Not enough operands for operator '!'");
            }

            rhs = stack_pop (&s);
            stack_push (&s, !rhs);
            continue;
        } else if (c == '&') {
            if ((peekc = fgetc (in)) != '&') {
                error (
                    "Lexical error: Excepted '&&' but found '%c%c'", c, peekc);
            }
            if (stack_empty (&s)) {
                error ("Semantic error: Not enough operands for operator '&&'");
            }
            lhs = stack_pop (&s);
            if (stack_empty (&s)) {
                error ("Semantic error: Not enough operands for operator '&&'");
            }
            rhs = stack_pop (&s);
            stack_push (&s, rhs && lhs);
            continue;
        } else if (c == '|') {
            if ((peekc = fgetc (in)) != '|') {
                error (
                    "Lexical error: Excepted '||' but found '%c%c'", c, peekc);
            }
            if (stack_empty (&s)) {
                error ("Semantic error: Not enough operands for operator '||'");
            }
            lhs = stack_pop (&s);
            if (stack_empty (&s)) {
                error ("Semantic error: Not enough operands for operator '||'");
            }
            rhs = stack_pop (&s);
            stack_push (&s, rhs || lhs);
            continue;
        }
    }

    rhs = stack_pop (&s);
    if (!stack_empty (&s)) {
        error ("Semantic error: Too many operands");
    }

    fprintf (out, "%d\n", rhs);
}

int
main (void)
{
    eval_postfix (stdin, stdout);
    return 0;
}
