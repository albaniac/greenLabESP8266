
#include "StringCommand.h"

/**
 * Constructor makes sure some things are set.
 */
StringCommand::StringCommand()
  : commandList(NULL),
    commandCount(0),
    defaultHandler(NULL),
    term('\n'),           // default terminator for commands, newline character
    last(NULL)
{
  strcpy(delim, " "); // strtok_r needs a null-terminated string
  clearBuffer();
}

/**
 * Adds a "command" and a handler function to the list of available commands.
 * This is used for matching a found token in the buffer, and gives the pointer
 * to the handler function to deal with it.
 */
void StringCommand::addCommand(const char *command, void (*function)()) {
  #ifdef SERIALCOMMAND_DEBUG
    Serial.print("Adding command (");
    Serial.print(commandCount);
    Serial.print("): ");
    Serial.println(command);
  #endif

  commandList = (StringCommandCallback *) realloc(commandList, (commandCount + 1) * sizeof(StringCommandCallback));
  strncpy(commandList[commandCount].command, command, SERIALCOMMAND_MAXCOMMANDLENGTH);
  commandList[commandCount].function = function;
  commandCount++;
}

/**
 * This sets up a handler to be called in the event that the receveived command string
 * isn't in the list of commands.
 */
void StringCommand::setDefaultHandler(void (*function)(const char *)) {
  defaultHandler = function;
}


/**
 * This checks the Serial stream for characters, and assembles them into a buffer.
 * When the terminator character (default '\n') is seen, it starts parsing the
 * buffer for a prefix command, and calls handlers setup by addCommand() member
 */
void StringCommand::readStr(String sBuffer ) {
	sBuffer.toCharArray(buffer, SERIALCOMMAND_BUFFER);
	  #ifdef SERIALCOMMAND_DEBUG
        Serial.print("Received: ");
        Serial.println(buffer);
      #endif

      char *command = strtok_r(buffer, delim, &last);   // Search for command at start of buffer
      if (command != NULL) {
        boolean matched = false;
        for (int i = 0; i < commandCount; i++) {
          #ifdef SERIALCOMMAND_DEBUG
            Serial.print("Comparing [");
            Serial.print(command);
            Serial.print("] to [");
            Serial.print(commandList[i].command);
            Serial.println("]");
          #endif



          // Compare the found command against the list of known commands for a match
          if (strncmp(command, commandList[i].command, SERIALCOMMAND_MAXCOMMANDLENGTH) == 0) {
            #ifdef SERIALCOMMAND_DEBUG
              Serial.print("Matched Command: ");
              Serial.println(command);
            #endif

            // Execute the stored handler function for the command
            (*commandList[i].function)();
            matched = true;
            break;
          }
        }


        if (!matched && (defaultHandler != NULL)) {
          (*defaultHandler)(command);
        }


      clearBuffer();
    }
}

/*
 * Clear the input buffer.
 */
void StringCommand::clearBuffer() {
  buffer[0] = '\0';
  bufPos = 0;
}

/**
 * Retrieve the next token ("word" or "argument") from the command buffer.
 * Returns NULL if no more tokens exist.
 */
char *StringCommand::next() {
  return strtok_r(NULL, delim, &last);
}
