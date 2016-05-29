#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <sr595.h>
#include <argp.h>

#define LOW 0
#define HIGH 1
#define DELAY 10

const char *argp_program_version =
  "shutter 0.1";
const char *argp_program_bug_address =
  "<>";

/* Program documentation. */
static char doc[] =
  "Shutter -- send command to shutter";

/* A description of the arguments we accept. */
static char args_doc[] = "ShutterId";

/* The options we understand. */
static struct argp_option options[] = {
  {"verbose",  'v', 0,      0,  "Produce verbose output" },
  {"quiet",    'q', 0,      0,  "Don't produce any output" },
  {"silent",   's', 0,      OPTION_ALIAS },
  { 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments
{
  char *args[1];                /* shutterId */
  int silent, verbose;
};

/* Parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;

  switch (key)
    {
    case 'q': case 's':
      arguments->silent = 1;
      break;
    case 'v':
      arguments->verbose = 1;
      break;

    case ARGP_KEY_ARG:
      if (state->arg_num >= 1)
        /* Too many arguments. */
        argp_usage (state);

      arguments->args[state->arg_num] = arg;
      break;

    case ARGP_KEY_END:
      if (state->arg_num < 1)
        /* Not enough arguments. */
        argp_usage (state);
      break;

    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc };


int main(int argc, char *argv[])
{
  struct arguments arguments;

  /* Default values. */
  arguments.silent = 0;
  arguments.verbose = 0;

  // Amount of outputs
  int OUTPUTS = 8;
  // Pin used for the data
  int PIN_DATA = 3;
  // Pin used for the clock
  int PIN_CLOCK = 0;
  // Pin used for the latch
  int PIN_LATCH = 2;

  int bit;
  int shutter;

  /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  /* Parse our arguments; every option seen by parse_opt will be reflected in arguments. */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  shutter = atoi(arguments.args[0]);

  wiringPiSetup () ;

  // Pin base 100 for 10 pins.
  sr595Setup (100, OUTPUTS, PIN_DATA, PIN_CLOCK, PIN_LATCH) ;

  printf ("Raspberry Pi - Shutter Test on shutter %d \n", shutter) ;

  // The 74HC595 can be in unstable state =: All 8 outputs to LOW
  for (bit = 0 ; bit < 8 ; ++bit)
        digitalWrite (100 + bit, LOW) ;
  delay (DELAY) ;

  digitalWrite (100 + shutter, HIGH);
  delay (DELAY);

  digitalWrite (100 + shutter, LOW) ;
  delay (DELAY) ;

  return 0 ;
}
