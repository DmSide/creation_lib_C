#ifndef RESULTS
#define RESULTS

enum Results
{
    END_OF_INPUT = -1, //end of input has been set
    SET_END_OF_INPUT = -10, //will no more input data
    END_OF_OUTPUT = -2, //no more output data
    ERROR_PUT_DATA = -4, //put data finished with error
    PROCESS_ERROR = -30
};

#endif // RESULTS

