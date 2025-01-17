#include "Arduino.h"
#define LINE_BUF_SIZE 128 // Maximum input string length
#define ARG_BUF_SIZE 64   // Maximum argument string length
#define MAX_NUM_ARGS 8    // Maximum number of arguments

int LEDpin = 13;
int blink_cycles = 16; // How many times the LED will blink
bool error_flag = false;

char line[LINE_BUF_SIZE];
char args[MAX_NUM_ARGS][ARG_BUF_SIZE];

// Function declarations
int cmd_help();
int cmd_led();
int cmd_exit();
int cmd_bomb();

// List of functions pointers corresponding to each command
int (*commands_func[])(){&cmd_help, &cmd_led, &cmd_exit, &cmd_bomb};

// List of command names
const char *commands_str[] = {"help", "led", "exit", "bomb"};

// List of LED sub command names
const char *led_args[] = {"on", "off", "blink", "bomb"};

int num_commands = sizeof(commands_str) / sizeof(char *);

void cli_init() {
  Serial.println(
      "Welcome to this simple Arduino command line interface (CLI).");
  Serial.println("Type \"help\" to see a list of commands.");
}

void help_help() {
  Serial.println("The following commands are available:");

  for (int i = 0; i < num_commands; i++) {
    Serial.print("  ");
    Serial.println(commands_str[i]);
  }
  Serial.println("");
  Serial.println("You can for instance type \"help led\" for more info on the "
                 "LED command.");
}

void help_led() {
  Serial.print("Control the on-board LED, either on, off or blinking ");
  Serial.print(blink_cycles);
  Serial.println(" times:");
  Serial.println("  led on");
  Serial.println("  led off");
  Serial.println("  led blink hz");
  Serial.println("    where \"hz\" is the blink frequency in Hz.");
}

void help_exit() {
  Serial.println(
      "This will exit the CLI. To restart the CLI, restart the program.");
}

int cmd_bomb() {
  Serial.print("bomb has been plant ");
  Serial.print(blink_cycles);
  int bib = 80;
  int delay_ms = (int)round(10000.0 / (bib / 2));

  for (bib = 5; bib < 80; bib += 5) {
    delay_ms = (int)round(1000.0 / (bib / 2));
    for (int i = 0; i < 5; i++) {
      digitalWrite(LEDpin, HIGH);
      delay(delay_ms);
      digitalWrite(LEDpin, LOW);
      delay(delay_ms);
    }
  }
  digitalWrite(LEDpin, 1);
  delay(3000);
  digitalWrite(LEDpin, 0);
  return 0;
}

int cmd_led() {
  if (strcmp(args[1], led_args[0]) == 0) {
    Serial.println("Turning on the LED.");
    digitalWrite(LEDpin, HIGH);
  } else if (strcmp(args[1], led_args[1]) == 0) {
    Serial.println("Turning off the LED.");
    digitalWrite(LEDpin, LOW);
  } else if (strcmp(args[1], led_args[2]) == 0) {
    if (atoi(args[2]) > 0) {
      Serial.print("Blinking the LED ");
      Serial.print(blink_cycles);
      Serial.print(" times at ");
      Serial.print(args[2]);
      Serial.print(" Hz.\n");

      int delay_ms = (int)round(10000.0 / atoi(args[2]) / 2);

      for (int i = 0; i < blink_cycles; i++) {
        digitalWrite(LEDpin, HIGH);
        delay(delay_ms);
        digitalWrite(LEDpin, LOW);
        delay(delay_ms);
      }
    } else {
      Serial.println("Invalid frequency.");
    }
  } else if (strcmp(args[1], led_args[3]) == 0) {
    cmd_bomb();
  } else {
    Serial.println("Invalid command. Type \"help led\" to see how to use the "
                   "LED command.");
  }
  return 0;
}

int cmd_exit() {
  Serial.println("Exiting CLI.");
  while (1)
    ;
}

int execute() {
  for (int i = 0; i < num_commands; i++) {
    if (strcmp(args[0], commands_str[i]) == 0) {
      return (*commands_func[i])();
    }
  }
  // debug
  Serial.println("Invalid command. Type \"help\" for more.");
  return 0;
}

void read_line() {
  String line_string;

  while (!Serial.available())
    ;

  if (Serial.available()) {
    line_string = Serial.readStringUntil('\n');
    if (line_string.length() < LINE_BUF_SIZE) {
      line_string.toCharArray(line, LINE_BUF_SIZE);
      Serial.println(line_string);
    } else {
      Serial.println("Input string too long.");
      error_flag = true;
    }
  }
}

void parse_line() {
  char *argument;
  int counter = 0;

  argument = strtok(line, " ");

  while ((argument != NULL)) {
    if (counter < MAX_NUM_ARGS) {
      if (strlen(argument) < ARG_BUF_SIZE) {
        strcpy(args[counter], argument);
        argument = strtok(NULL, " ");
        counter++;
      } else {
        Serial.println("Input string too long.");
        error_flag = true;
        break;
      }
    } else {
      break;
    }
  }
}

void my_cli() {
  Serial.print("> ");

  read_line();
  if (!error_flag) {
    parse_line();
  }
  if (!error_flag) {
    execute();
  }

  memset(line, 0, LINE_BUF_SIZE);
  memset(args, 0, sizeof(args[0][0]) * MAX_NUM_ARGS * ARG_BUF_SIZE);

  error_flag = false;
}

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);

  cli_init();
}

void loop() { my_cli(); }

int cmd_help() {
  if (args[1] == NULL) {
    help_help();
  } else if (strcmp(args[1], commands_str[0]) == 0) {
    help_help();
  } else if (strcmp(args[1], commands_str[1]) == 0) {
    help_led();
  } else if (strcmp(args[1], commands_str[2]) == 0) {
    help_exit();
  } else {
    help_help();
  }
  return 0;
}
