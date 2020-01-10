#ifndef RNNOISEMODULE_H
#define RNNOISEMODULE_H

extern "C"
{
    #include <rnnoise.h>
}
#include "taskappendmodule.h"
#include <cstdint>


class RNnoiseModule:public TaskAppendModule,public TaskJoinModule
{
private:

    size_t data_size = 960;
    DenoiseState *den_state;
    bool is_first;

public:

    RNnoiseModule(TaskJoinModule* next);
    ~RNnoiseModule()
    {
        if (den_state) {
            rnnoise_destroy(den_state);
        }
    }
    bool init();
    bool rnnoise_buffer(TaskOutBuffer& buffer,std::ostringstream& error_stream);
    bool start(TaskOutBuffer& buffer,std::ostringstream& error_stream);
    bool process(TaskOutBuffer& buffer,std::ostringstream& error_stream);
    bool finish(TaskOutBuffer& buffer,std::ostringstream& error_stream);

};

#endif // RNNOISEMODULE_H
