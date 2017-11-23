
#ifndef SRCS_H
#define SRCS_H

# include <systemc-ams.h>

//using namespace std;
 
SCA_TDF_MODULE (srcs) {

    sca_tdf::sc_out<double> out_tdf_de;         // output port

    srcs( sc_core::sc_module_name nm, double ampl_, double freq_, sca_core::sca_time Tm_)
        : out_tdf_de ("out_tdf_de")

    {
        ampl = ampl_;
        freq =freq_;
        Tm =Tm_;
    }

    double t;
    void set_attributes();
    void processing();

    private:
        double ampl;           // amplitude
        double freq;           // frequency
        sca_core::sca_time Tm; // module time step
};
#endif // srcs_H
