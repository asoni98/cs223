#include <stdio.h>
#include <stdlib.h>
int fpeek();

int main() {
    char c;
    char ch;
    int state = 0;
    int state_prev = 0; //used for remembering the old state for @ and ""

    while((c=getchar())!=EOF) {

          ch = fpeek(stdin); //ch holds the next character

          switch(c) {
            case '/':
                switch(ch) {
                  case '/':
                    if (state == 0) {state = 1;}
                    break;

                 case '*':
                    if (state == 0) {state = 5;}
                    break;
                           }
            break;

            case '\n':
              switch(state) {
                case 0: state = 0; break;
                case 1: state = 0; break; //not possible
                case 2: state = 0; break; //not possible
                case 3: state = 0; break; //empty line
                case 4: printf("%c", c); state = 0; break; //print new line for //
                case 5: state = 0; break; //not possible
                case 6: state = 0; break; //not possible
                case 7: state = 7; break; //empty line
                case 8: printf("%c", c); state = 7; break;} //print new line for /*
            break;

            case '*':
              if (ch == '/') {
              switch(state) {
                case 0: state = 0; break;
                case 1: state = 0; break;
                case 2: state = 0; break;
                case 3: state = 0; break;
                case 4: state = 0; break;
                case 5: state = 0; break;
                case 6: state = 0; break;
                case 7: state = 0; break;  //check this
                case 8: printf("\n"); state = 0; break;
                //maybe case 9
                case 10:
                if (state_prev == 7) {
                state = 0;
              } else if (state_prev == 8) {
                printf("\n");
                state = 0;}
                break;
              }}
            break;

            case '\t':  //fall through
            case ' ': {
              if (state == 10) {state = state_prev;}
              if (ch == '@' && state != 0) {//if there is a space before @ then store previous state and go into state 9
                state_prev = state;
                state = 9;
              }}
            break;
            case '@': {
              if (state == 3 || state == 7) { //if still in the beginning of a comment with @ then go to state 9
                state_prev = state;
                state = 9;
               }}
            break;

            case '\"': {
              if (state == 0) {state = 11;} //state for start of "
              else if (state == 11) {state = 0;}}
            break;
            case '\'': {
              if (state == 0) {state = 12;} //state for start of '
              else if (state == 12) {state = 0;}}
            break;

            case '\\': {
              if (ch == '\n') {
                state_prev = state;
                state = 13;
              }}
            break;

          }

            switch(state) {
              /* CODING STATE */
              case 0: break;

              /**** Commenting States  *****/
              case 1: state = 2; break; //delays setting state 3 by two characters
              case 2: state = 3; break;
              case 3:
                if (c == '\t' || c == ' ' || c =='*' || c =='@') {state = 3;}
                else {state = 4; printf("%c", c);}
              break;
              case 4: printf("%c", c); break; //print "//" comments

              case 5: state = 6; break; //delays setting state 7 by two characters
              case 6: state = 7; break;
              case 7:
                if (c == '\t' || c == ' ' || c == '*' || c == '@' || c =='\n') {state = 7;}
                else {state = 8; printf("%c", c);}
                break;
              case 8: printf("%c", c); break; //print "/*" comments

              case 9:
                if (state_prev == 4 || state_prev == 8) {
                  printf("%c", c);
                  state = 10;}
                else {state = 10;}
                break;
              case 10: if (ch == '\n') {state = state_prev;} break;

              case 11: state = 11; break;
              case 12: state = 12; break;

              case 13: state = 14; break;
              case 14: if (ch != EOF) {state = state_prev;} break;
            }

            if (ch == EOF) {
              switch(state) {
                case 0: state = 0; break;
                case 1: state = 0; break;
                case 2: state = 0; break;
                case 3: state = 0; break;
                case 4: printf("\n"); state = 0; break;
                case 5: state = 0; break;
                case 6: state = 0; break;
                case 7: state = 0; break;
                case 8: printf("\n"); state = 0; break;
                case 9:
                case 10:
                if (state_prev == 4 || state_prev == 8) {
                  printf("\n");
                  state = 0;
                  break;}
                case 11: state = 0; break;
                case 12: state = 0; break;
                case 13:
                case 14: printf("%c", c); state = 0; break;}}

        //  printf("%c:%d ", c, state);
      }
    return 0;
}

int fpeek()
{
  char ch;
  ch = getchar();
  ungetc(ch, stdin);
  return ch;
}
