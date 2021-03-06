#pragma once

enum class syscall_codes
{
    NULL_SYSCALL = 0,                 // don't use >:^(
    SEND_SERVICE_SYSCALL = 1,         // send a message to a service (later non-service process may be forced to use send message with pid)
    READ_SERVICE_SYSCALL = 2,         // read all message that you have
    GET_RESPONSE_SERVICE_SYSCALL = 3, // if the message sended has been responded
    GET_PROCESS_GLOBAL_DATA = 4,      // get process global data, if arg1 (target) is nullptr, return self global data, else return a process global data return -1 if there is an error
    SEND_PROCESS_SYSCALL_PID = 5,     // send a message to a process with pid
    MEMORY_ALLOC = 6,                 // pmm alloc
    MEMORY_FREE = 7,                  // pmm free
    FILE_OPEN = 8,                    // open a file
    FILE_CLOSE = 9,                   // close a file
    FILE_READ = 10,                   // read file
    FILE_WRITE = 11,                  // write file
    FILE_SEEK = 12,                   // seek a file
    NANO_SLEEP = 13,                  // sleep the current process
    GET_PID = 14,                     // get current pid
    EXEC = 15,                        // execute a new programm
};
