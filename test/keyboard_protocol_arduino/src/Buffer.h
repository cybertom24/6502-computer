#ifndef BUFFER_H
#define BUFFER_H

#include "Arduino.h"



class BufferManipulator
{
public:
    static uint8_t read(volatile Buffer *buffer) {
        // Read
        uint8_t d = buffer->block[buffer->readPtr];
        
        // Increase read ptr if needed
        if (buffer->readPtr != buffer->writePtr)
            buffer->readPtr = (buffer->readPtr + 1) % BUFFER_SIZE;
        
        return d;
    };

    static void discard(volatile Buffer *b) {
        read(b);
    }
    
    static uint8_t peek(volatile Buffer *b) {
        return b->block[b->readPtr];
    };

    /**
     * Returns:
     * - true = write pointer has been increased
     * - false = write pointer has not been increased
    */
    static bool write(volatile Buffer *b, uint8_t data) {
        // Write
        b->block[b->writePtr] = data;
        
        // Increase pointer if necessary
        if (b->writePtr - b->readPtr == -1 || b->writePtr - b->readPtr == (BUFFER_SIZE - 1))
            return false;
        
        b->writePtr = (b->writePtr + 1) % BUFFER_SIZE;
        return true;
    };

    static int available(Buffer *b) {
        if (b->writePtr > b->readPtr)
            return b->writePtr - b->readPtr;
        
        return (b->writePtr + BUFFER_SIZE) - b->readPtr;
    }
};

#endif