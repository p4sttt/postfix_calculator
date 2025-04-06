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

void
postfix (FILE *in, FILE *out)
{
    stack_t s;
    int c, peekc;
    int val;

    stack_init (&s);

    while ((c = fgetc (in)) != EOF && c != '\n' && c != '=') {
        if (c == ' ' || c == '\t') {
            continue;
        }

        if (isdigit (c)) {
            ungetc (c, in);
            fscanf (in, "%d", &val);
            fprintf (out, "%d ", val);
            continue;
        } else if (c == '-' || c == '+') {
            fscanf (in, "%d", &val);
            fprintf (out, "%c%d ", c, val);
        }

        if (c == '(') {
            stack_push (&s, c);
            continue;
        } else if (c == ')') {
            while (!stack_empty (&s) && stack_peek (&s) != '(') {
                fprintf (out, "%c ", stack_pop (&s));
            }
            if (stack_empty (&s)) {
                error ("Lexical error: Unmatched parentheses");
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
                stack_pop (&s);
                fprintf (in, "&& ");
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
                stack_pop (&s);
                fprintf (in, "|| ");
            }
            stack_push (&s, '|');
            continue;
        }

        error ("Lexical error: Unexpected symbol '%c'", c);
    }

    while (!stack_empty (&s)) {
        c = stack_pop (&s);
        if (c == '!')
            printf ("! ");
        if (c == '&')
            printf ("&& ");
        if (c == '|')
            printf ("&& ");
    }

    fprintf (in, "\n");
}
