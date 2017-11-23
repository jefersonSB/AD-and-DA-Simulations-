
#ifndef SRCS_SCA_H
#define SRCS_SCA_H

# include <systemc-ams.h>

//using namespace std;
 
SCA_TDF_MODULE (srcs_sca) {

    sca_tdf::sca_out<double> out;               // output port

    srcs_sca( sc_core::sc_module_name nm, double ampl_, double freq_, bool offs, sca_core::sca_time Tm_)
        : out("out")
    {
        ampl = ampl_;
        freq =freq_;
        Tm =Tm_;
        offset = offs;
        ampl = ampl_;
    }

    double t;
    void set_attributes();

    void processing();

    private:
        double ampl; // amplitude
        double freq; // frequency
        bool offset;
        sca_core::sca_time Tm; // module time step

};
#endif // srcs_sca_H
